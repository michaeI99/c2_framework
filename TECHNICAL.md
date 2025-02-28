This project implements a basic Command and Control (C2) framework with the following components:
Server Component (Python/Flask)

RESTful API for agent communications
Agent management and tracking
Command queuing system
Basic authentication and encryption
CLI interface for operator control

Agent Component (C)

Lightweight client written in C
Periodic beaconing to C2 server
Command execution capabilities
Output capture and exfiltration
Configurable communication intervals

Communication Protocol

HTTP/HTTPS based communication
JSON formatted data exchange
Minimalist protocol design for reliability
Command/response architecture

**Base64 encoding** for simple data obfuscation
- **XOR encryption** for message confidentiality (not secure for production)
- Agent identification and tracking
- Session management
- Command validation

### Future Updates
- **DNS tunneling** for stealthy communication (to bypass network restrictions)
- **AES encryption** for stronger message confidentiality (to improve security)

