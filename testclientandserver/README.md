# 🔌 TCP Client-Server Test

> A simple TCP client-server implementation in C++ as a warm-up exercise before starting the **ft_irc** project at **1337 (42 Network)**.

## 📚 About This Project

This is a practice project created to understand the fundamentals of **socket programming** in C++ before diving into the ft_irc project. The goal is to grasp the core concepts of network communication that will be essential for building an IRC server.

## 👥 Team Practice

This repository serves as a learning playground for our team at **1337** to:
- Understand TCP/IP socket programming
- Learn client-server architecture
- Practice with system calls for network communication
- Prepare for the ft_irc project requirements

---

## 🧠 Concepts Covered

### Socket Programming Basics

| Concept | Description |
|---------|-------------|
| `socket()` | Creates a new socket (communication endpoint) |
| `bind()` | Assigns an address (IP + port) to the socket |
| `listen()` | Marks the socket as passive, ready to accept connections |
| `accept()` | Accepts an incoming connection request |
| `connect()` | Initiates a connection to a server |
| `send()` | Sends data through the socket |
| `recv()` | Receives data from the socket |
| `close()` | Closes the socket connection |

### Network Structures Used

```cpp
struct sockaddr_in {
    sa_family_t    sin_family;  // Address family (AF_INET for IPv4)
    in_port_t      sin_port;    // Port number (use htons() to convert)
    struct in_addr sin_addr;    // IP address
};
```

### Important Functions

| Function | Purpose |
|----------|---------|
| `htons()` | Converts port number from host to network byte order |
| `htonl()` | Converts address from host to network byte order |
| `inet_addr()` | Converts IP string to binary form |

---

## 📁 Project Structure

```
testclientandserver/
├── server.cpp    # Server-side implementation
├── client.cpp    # Client-side implementation
├── Makefile      # Build automation
└── README.md     # This file
```

---

## 🛠️ Compilation

```bash
# Build both server and client
make

# Build only server
make server

# Build only client
make client

# Clean executables
make clean

# Rebuild everything
make re
```

---

## 🚀 Usage

### Step 1: Start the Server
```bash
./server
```
Expected output:
```
socket creation successful .........
socket binding successful .........
looking for clients .........
```

### Step 2: Start the Client (in another terminal)
```bash
./client
```
Expected output:
```
socket creation successful .........
socket connection successful .........
client :
```

### Step 3: Chat!
- Type a message in the **client** terminal and press Enter
- The message appears on the **server** terminal
- Type a reply in the **server** terminal and press Enter
- The reply appears on the **client** terminal
- Type `exit` in the client to disconnect

---

## 🔄 Communication Flow

```
┌────────────┐                      ┌────────────┐
│   CLIENT   │                      │   SERVER   │
└─────┬──────┘                      └─────┬──────┘
      │                                   │
      │  1. connect() ─────────────────►  │ accept()
      │                                   │
      │  2. send(message) ─────────────►  │ recv()
      │                                   │
      │  3. recv() ◄───────────────────  │ send(reply)
      │                                   │
      │  4. Repeat steps 2-3...           │
      │                                   │
      │  5. close() ─────────────────────►│ close()
      │                                   │
```

---

## ⚙️ Configuration

| Parameter | Value | Description |
|-----------|-------|-------------|
| Port | `1500` | The port number for communication |
| Buffer Size | `1024` | Maximum message size in bytes |
| IP Address | `127.0.0.1` | Localhost (same machine) |

To change these values, modify the variables at the top of `main()` in both files.

---

## 📝 Key Learnings

1. **Socket Creation**: Both client and server need to create a socket using `socket(AF_INET, SOCK_STREAM, 0)`

2. **Server Setup**:
   - `bind()` - Attach socket to a specific port
   - `listen()` - Start listening for connections
   - `accept()` - Accept incoming connections

3. **Client Setup**:
   - `connect()` - Connect to the server's IP and port

4. **Data Exchange**:
   - `send()` - Transmit data
   - `recv()` - Receive data
   - Always clear the buffer with `memset()` before receiving

5. **Byte Order**: Network uses big-endian, so use `htons()`/`htonl()` for conversion

---

## ⚠️ Common Issues & Solutions

| Issue | Cause | Solution |
|-------|-------|----------|
| "Address already in use" | Port still bound after crash | Wait ~30 seconds or use `SO_REUSEADDR` |
| Connection refused | Server not running | Start server before client |
| No message received | Both waiting for each other | Client should send first |
| Garbage in messages | Buffer not cleared | Use `memset()` before `recv()` |

---

## 🎯 Next Steps: ft_irc

This practice prepares us for ft_irc, where we'll need to:
- Handle **multiple clients** simultaneously (using `poll()` or `select()`)
- Implement the **IRC protocol** (NICK, JOIN, PRIVMSG, etc.)
- Manage **channels** and **user authentication**
- Handle **partial messages** and **message queuing**

---

## 👨‍💻 Authors

mowardan at **1337** - 42 Network

---
