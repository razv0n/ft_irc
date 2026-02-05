# 🖥️ TCP Server Learning Project

> **Preparation for ft_irc @ 1337 School**

This project is a learning exercise to understand TCP socket programming in C++ before starting the **ft_irc** project with my team at 1337.

---

## 📚 Concepts Covered

### 1. Socket Creation
```cpp
int listening = socket(AF_INET, SOCK_STREAM, 0);
```
| Parameter | Value | Description |
|-----------|-------|-------------|
| `AF_INET` | Address Family | IPv4 Internet protocols |
| `SOCK_STREAM` | Socket Type | TCP (reliable, connection-oriented) |
| `0` | Protocol | Auto-select (TCP for SOCK_STREAM) |

**Other options:**
- `AF_INET6` - IPv6
- `AF_UNIX` - Local communication
- `SOCK_DGRAM` - UDP (unreliable, connectionless)

---

### 2. Socket Options
```cpp
int opt = 1;
setsockopt(listening, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
```
- `SO_REUSEADDR` - Allows reuse of address/port immediately after server closes
- Prevents "Address already in use" error

---

### 3. Binding a Socket
```cpp
sockaddr_in hint;
hint.sin_family = AF_INET;
hint.sin_port = htons(55000);
inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
bind(listening, (sockaddr*)&hint, sizeof(hint));
```

| Function | Description |
|----------|-------------|
| `htons()` | Host to Network Short - converts 16-bit to network byte order |
| `inet_pton()` | Converts IP from text ("0.0.0.0") to binary form |
| `bind()` | Associates socket with IP:port |

**Why bind?**
- Server needs a known address for clients to connect to
- Reserves the port for this application
- `0.0.0.0` means listen on all network interfaces

---

### 4. Listening for Connections
```cpp
listen(listening, SOMAXCONN);
```
- Marks socket as passive (waiting for connections)
- `SOMAXCONN` - Maximum pending connections in queue

---

### 5. Accepting Connections
```cpp
int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
```
- Blocks until a client connects
- Returns a **new socket** for communicating with that specific client
- Original socket continues listening (in multi-client scenarios)

---

### 6. Getting Client Info
```cpp
getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0);
inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
ntohs(client.sin_port);
```

| Function | Description |
|----------|-------------|
| `getnameinfo()` | Resolves client address to hostname |
| `inet_ntop()` | Converts IP from binary to text form |
| `ntohs()` | Network to Host Short - converts 16-bit from network byte order |

---

### 7. Sending & Receiving Data
```cpp
// Receive data from client
int bytesReceived = recv(clientSocket, buf, 4096, 0);

// Send data to client
int sendResult = send(clientSocket, buf, bytesReceived, 0);
```

| Return Value | Meaning |
|--------------|---------|
| `> 0` | Number of bytes received/sent |
| `0` | Client disconnected (for recv) |
| `-1` | Error occurred |

---

### 8. Closing Sockets
```cpp
close(listening);     // Close listening socket
close(clientSocket);  // Close client connection
```
Always close sockets to free resources!

---

## 🔧 Build & Run

### Compile
```bash
make        # Build the server
make clean  # Remove object files
make fclean # Remove all generated files
make re     # Rebuild everything
```

### Run Server
```bash
make run
# or
./server
```

### Test with Python (no netcat needed)
```bash
python3 -c "
import socket
s = socket.socket()
s.connect(('localhost', 55000))
s.send(b'Hello Server!\n')
print('Response:', s.recv(1024).decode())
s.close()
"
```

### Test with netcat (if available)
```bash
nc localhost 55000
```

---

## 📁 Project Structure
```
test/
├── main.cpp      # TCP Echo Server implementation
├── Makefile      # Build configuration
└── README.md     # This file
```

---

## 🎯 Next Steps for ft_irc

After understanding these basics, the ft_irc project will require:

1. **Multiple Clients** - Using `poll()` or `select()` for I/O multiplexing
2. **Non-blocking Sockets** - `fcntl(fd, F_SETFL, O_NONBLOCK)`
3. **IRC Protocol** - Implementing NICK, USER, JOIN, PRIVMSG, etc.
4. **Channels** - Managing chat rooms and user lists
5. **Authentication** - Password handling
6. **Error Handling** - Robust error management

---

## 📖 Useful Man Pages
```bash
man socket
man bind
man listen
man accept
man recv
man send
man poll
man select
```

---

## 👥 Team Project: ft_irc @ 1337

This learning project prepares for the **ft_irc** project - building a fully functional IRC server compatible with real IRC clients.

---

*Made with 💻 by mowardan - 1337 School*
