### Prerequisites

Python 3.8+ for the server component
GCC or compatible C compiler for the agent
Required libraries: Flask, Requests, libcurl, OpenSSL

### Server Setup

Clone the repository:
git clone https://github.com/michaeI99/c2_framework.git
cd c2-framework/server

Set up a virtual enviornment as it is best practive for Python projects

On macOS/Linux
```
python3 -m venv venv
source venv/bin/activate
```
On Windows
```
python -m venv venv
venv\Scripts\activate
```
Install Python dependencies:
```
pip install -r requirements.txt
```
Start the C2 server:
```
python3 main.py
The server will listen on http://localhost:5050 by default.
```
### Agent Compilation

Navigate to the agent directory:
```
cd c2-framework/agent
```
Compile the agent:
```
make
```
Run the agent in a test environment:
```
./agent
```

### Using the CLI

In a separate terminal, start the CLI:
```
cd c2-framework/server
python3 cli.py
```
Available commands in the CLI:
```
1: List connected agents
2: Send command to an agent
3: Exit
```


### Security Considerations for Testing

Always run in a isolated/sandboxed environment <br/>
Never deploy on production or public-facing systems <br/>
Use strong credentials for authentication <br/>
Monitor all communications for research purposes <br/>

### Project Structure
```
c2_framework/
├── server/                      # Python-based C2 server
│   ├── main.py                  # Entry point for server
│   ├── cli.py                   # Command line interface
│   ├── requirements.txt         # Python dependencies
│   └── ...
├── agent/                       # C-based agent
│   ├── src/                     # Source code
│   │   ├── main.c               # Main agent code
│   │   ├── http.c/h             # HTTP communications
│   │   ├── crypto.c/h           # Encryption implementations
│   │   └── ...
│   └── Makefile                 # Build configuration
├──  LICENSE                      # Legal documentation
├──  README.md                    # This documentation
├──  TECHNICAL.md                 # TECHNICAL documentation
└──  USAGE.md                     # How to setup and use this software
```
