# ft_irc

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-98-blue.svg?style=for-the-badge&logo=c%2B%2B" alt="C++98" />
  <img src="https://img.shields.io/badge/School-42%20Network-black.svg?style=for-the-badge" alt="42 Network" />
  <img src="https://img.shields.io/badge/Protocol-RFC%201459%20%7C%202812-orange.svg?style=for-the-badge" alt="IRC RFC" />
  <img src="https://img.shields.io/badge/I%2FO-Multiplexing%20(poll)-green.svg?style=for-the-badge" alt="poll()" />
  <img src="https://img.shields.io/badge/Status-Completed-success.svg?style=for-the-badge" alt="Status" />
</p>

> A lightweight, robust, fully functional **Internet Relay Chat (IRC) server** implemented in **C++98**, developed as part of the 42 School curriculum. Built with non-blocking I/O multiplexing (`poll()`) to handle concurrent client sessions, channels, modes, operators, and real-time private/channel communication.

---

## 📑 Table of Contents

- [Overview](#-overview)
- [System Architecture](#-system-architecture)
- [Features](#-features)
- [Supported IRC Commands](#-supported-irc-commands)
- [Channel Modes](#-channel-modes)
- [RFC Error & Reply Codes](#-rfc-error--reply-codes)
- [Project Structure](#-project-structure)
- [Getting Started](#-getting-started)
  - [Prerequisites](#prerequisites)
  - [Compilation](#compilation)
  - [Running the Server](#running-the-server)
- [Connecting Clients](#-connecting-clients)
  - [1. Connecting with Netcat (`nc`)](#1-connecting-with-netcat-nc)
  - [2. Connecting with Irssi](#2-connecting-with-irssi)
  - [3. Connecting with HexChat / GUI Clients](#3-connecting-with-hexchat--gui-clients)
- [Internal Design & Implementation Highlights](#-internal-design--implementation-highlights)
  - [Non-Blocking Multiplexing](#1-non-blocking-multiplexing)
  - [Command Extraction & Buffer Management](#2-command-extraction--buffer-management)
  - [Client Lifecycle & State Machine](#3-client-lifecycle--state-machine)
  - [Channel & Operator Control](#4-channel--operator-control)
  - [Signal Handling & Graceful Exit](#5-signal-handling--graceful-exit)
- [Authors](#-authors)

---

## 🔭 Overview

The **ft_irc** project tasks students with creating an IRC server from the ground up, compliant with [RFC 1459](https://tools.ietf.org/html/rfc1459) and [RFC 2812](https://tools.ietf.org/html/rfc2812).

The server acts as a centralized communication daemon where multiple clients can:
1. Authenticate with a server password.
2. Register a unique nickname and user identity.
3. Join public, keyed, limited, or invite-only chat rooms (channels).
4. Send private direct messages or broadcast to full channels.
5. Manage channels with operator permissions (kick users, assign ops, modify channel topics, change modes).

Everything is executed in a **single process / single thread** without blocking socket operations, using `poll()` to achieve asynchronous event-driven I/O.

---

## 🏗 System Architecture

```
                    +---------------------------+
                    |    IRC Clients (TCP)      |
                    | (Irssi, HexChat, nc, etc.)|
                    +-------------+-------------+
                                  |
                                  v
+-------------------------------------------------------------------+
|                        ft_irc (ircserv)                           |
|                                                                   |
|   +-----------------------------------------------------------+   |
|   |                   Socket Listening & I/O                  |   |
|   |   - socket(), bind(), listen(), accept()                  |   |
|   |   - Non-blocking event dispatching using poll()           |   |
|   +-----------------------------+-----------------------------+   |
|                                 |                                 |
|                                 v                                 |
|   +-----------------------------------------------------------+   |
|   |                  Client Buffer & Parser                   |   |
|   |   - Per-client stream buffering (handles partial packets) |   |
|   |   - Command extraction (\r\n / \n boundary)               |   |
|   |   - Tokenizer & Command Routing                           |   |
|   +-----------------------------+-----------------------------+   |
|                                 |                                 |
|         +-----------------------+-----------------------+         |
|         |                                               |         |
|         v                                               v         |
|   +---------------------------+   +---------------------------+   |
|   |       Client Manager      |   |      Channel Manager      |   |
|   | - File descriptor mapping |   | - Channel list (#channel) |   |
|   | - Nickname mapping        |   | - Member & operator sets  |   |
|   | - Registration state      |   | - Topic, key, user limits |   |
|   |   (PASS -> NICK -> USER)  |   | - Invite-only whitelist   |   |
|   +---------------------------+   +---------------------------+   |
|                                                                   |
+-------------------------------------------------------------------+
```

---

## ⚡ Features

- **Non-blocking Event Loop**: All connections, incoming commands, and disconnections are handled through `poll()` on a single thread.
- **Robust Packet Parsing**: Reassembles fragmented TCP packets and parses commands cleanly regardless of chunk boundaries.
- **Strict Registration Flow**: Requires `PASS` before `NICK` and `USER` registration before standard IRC operations are unlocked.
- **Channel Operations**: Creation on first join, automatic destruction when the last member leaves, topic management, membership tracking, and broadcasting.
- **Complete Channel Modes (`+i`, `+t`, `+k`, `+o`, `+l`)**: Supports invite-only channels, protected topics, channel passwords, operator promotions/demotions, and user occupancy limits.
- **Signal Resilience**: Clean signal handling (`SIGINT`, `SIGTERM`, `SIGQUIT`) ensuring all sockets, memory allocations, and resources are cleanly freed upon shutdown.
- **Client Compatibility**: Tested and compatible with both command-line clients (e.g. `nc`, `irssi`, `weechat`) and GUI IRC clients (`HexChat`).

---

## 💬 Supported IRC Commands

| Command | Syntax | Description |
|:---|:---|:---|
| **PASS** | `PASS <password>` | Authenticates client connection against server password. |
| **NICK** | `NICK <nickname>` | Sets or updates client nickname. Checks for collisions across the server. |
| **USER** | `USER <username> <hostname> <servername> :<realname>` | Completes registration and triggers the `001 RPL_WELCOME` banner. |
| **JOIN** | `JOIN <#channel> [key]` | Joins an existing channel or creates a new one (creator becomes operator). |
| **PART** | `PART <#channel> [:reason]` | Leaves a channel. Destroys the channel if empty. |
| **PRIVMSG**| `PRIVMSG <target> :<message>` | Sends a message to a channel (broadcast to members) or another user (private). |
| **TOPIC** | `TOPIC <#channel> [:new_topic]` | Displays current channel topic, or updates it (honors `+t` operator mode). |
| **MODE** | `MODE <#channel> <+/-mode> [param]` | Queries or modifies channel modes and user operator statuses. |
| **KICK** | `KICK <#channel> <nick> [:reason]` | Operators can forcibly remove a user from the channel. |
| **INVITE**| `INVITE <nick> <#channel>` | Operators can invite a client into an invite-only (`+i`) channel. |
| **PING** | `PING <token>` | Responds with `PONG ircserv :<token>` to keep connection alive. |
| **QUIT** | `QUIT [:reason]` | Cleanly terminates client connection and notifies joined channels. |
| **CAP** | `CAP <subcommand>` | Negotiates client capabilities (handled gracefully for modern IRC clients). |

---

## ⚙️ Channel Modes

The server implements five fundamental IRC channel modes via the `MODE` command:

| Mode | Flag | Description | Example |
|:---:|:---:|:---|:---|
| **Invite-only** | `i` | Only invited users (`INVITE`) can join the channel. | `MODE #general +i`<br>`MODE #general -i` |
| **Topic Restriction** | `t` | Only channel operators can change the channel topic. | `MODE #general +t`<br>`MODE #general -t` |
| **Channel Key** | `k` | Protects the channel with a password/key. | `MODE #general +k secret123`<br>`MODE #general -k` |
| **Operator Privilege** | `o` | Grants or revokes operator rights (`@`) to a member. | `MODE #general +o alice`<br>`MODE #general -o alice` |
| **User Limit** | `l` | Sets the maximum number of concurrent users allowed. | `MODE #general +l 10`<br>`MODE #general -l` |

---

## 📋 RFC Error & Reply Codes

The server issues standard RFC-compliant numeric codes:

| Numeric | Macro Name | Description |
|:---:|:---|:---|
| `001` | `RPL_WELCOME` | Client successfully registered on the network |
| `324` | `RPL_CHANNELMODEIS` | Returns the active channel modes |
| `331` | `RPL_NOTOPIC` | No topic is currently set for the channel |
| `332` | `RPL_TOPIC` | Returns current topic of the channel |
| `353` | `RPL_NAMREPLY` | List of channel members (with `@` for operators) |
| `366` | `RPL_ENDOFNAMES` | Marks the end of `/NAMES` list |
| `401` | `ERR_NOSUCHNICK` | Target nickname does not exist |
| `403` | `ERR_NOSUCHCHANNEL` | Target channel name does not exist or is invalid |
| `431` | `ERR_NONICKNAMEGIVEN` | No nickname parameter provided |
| `433` | `ERR_NICKNAMEINUSE` | Nickname is already in use by another connected client |
| `442` | `ERR_NOTONCHANNEL` | Client is not a member of the requested channel |
| `451` | `ERR_NOTREGISTERED` | Command rejected because client is not fully registered |
| `461` | `ERR_NEEDMOREPARAMS` | Command syntax error (insufficient parameters) |
| `462` | `ERR_ALREADYREGISTRED` | Client attempted to re-authenticate or re-register |
| `464` | `ERR_PASSWDMISMATCH` | Incorrect server password provided |
| `471` | `ERR_CHANNELISFULL` | Cannot join channel: user limit exceeded (`+l`) |
| `472` | `ERR_UNKNOWNMODE` | Unknown mode flag character |
| `473` | `ERR_INVITEONLYCHAN` | Cannot join channel: invite-only channel (`+i`) |
| `475` | `ERR_BADCHANNELKEY` | Cannot join channel: incorrect key provided (`+k`) |
| `482` | `ERR_CHANOPRIVSNEEDED` | Action denied: channel operator rights required |

---

## 📂 Project Structure

```
ft_irc/
├── Makefile                # Build automation (-Wall -Wextra -Werror -std=c++98)
├── main.cpp                # Entry point, CLI argument validation, signal setup
├── includes/               # Header definitions
│   ├── ft_irc.hpp          # Global dependencies, includes, external signals
│   ├── server.hpp          # Server class declaration & command handler interfaces
│   ├── client.hpp          # Client session representation & packet buffer
│   └── Channel.hpp         # Channel class (members, operators, modes, topics)
├── src/                    # Core server implementation
│   ├── server.cpp          # Socket initialization, poll loop, dispatch, cleanup
│   ├── client.cpp          # Buffer accumulation, command slicing, getter/setters
│   └── Channel.cpp         # Channel state, message broadcast, member mutations
└── cmd/                    # IRC command handlers
    ├── parseCmd.cpp        # Validation utilities (channel/client existence, op checks)
    ├── pass.cpp            # PASS command handler
    ├── nick.cpp            # NICK command handler
    ├── user.cpp            # USER command handler
    ├── join.cpp            # JOIN command handler
    ├── part.cpp            # PART command handler
    ├── prvmsg.cpp          # PRIVMSG command handler
    ├── topic.cpp           # TOPIC command handler
    ├── kick.cpp            # KICK command handler
    ├── invite.cpp          # INVITE command handler
    ├── mode.cpp            # MODE command handler (+i, +t, +k, +o, +l)
    ├── ping.cpp            # PING command handler
    └── quit.cpp            # QUIT command handler & client teardown
```

---

## 🚀 Getting Started

### Prerequisites

- A POSIX-compliant operating system (Linux or macOS).
- A C++ compiler supporting C++98 (`clang++` or `g++`).
- GNU Make.

### Compilation

Clone the repository and build the executable using `make`:

```bash
git clone git@github.com:razv0n/ft_irc.git
cd ft_irc
make
```

Available build targets:
- `make` or `make all`: Compiles source files and produces `ircserv`.
- `make clean`: Removes intermediate `.o` object files.
- `make fclean`: Removes object files and the `ircserv` binary.
- `make re`: Rebuilds the entire project from scratch.

### Running the Server

```bash
./ircserv <port> <password>
```

- `<port>`: The listening port (must be between `1024` and `65535`).
- `<password>`: The server connection password required by clients.

**Example:**
```bash
./ircserv 6667 mysecretpass
```

---

## 🔌 Connecting Clients

### 1. Connecting with Netcat (`nc`)

You can test the server directly with raw socket tools like `nc`:

```bash
nc -C 127.0.0.1 6667
```

*(Note: The `-C` flag ensures carriage return line feeds `\r\n` are sent).*

Then, perform the registration handshake:

```irc
PASS mysecretpass
NICK alice
USER alice 0 * :Alice Wonderland
```

Upon successful registration, the server replies:
```irc
:ircserv 001 alice :Welcome to ft_irc!
```

Try channel commands:
```irc
JOIN #42
PRIVMSG #42 :Hello everyone!
MODE #42 +t
TOPIC #42 :42 Network IRC Room
PART #42 :Leaving now
QUIT :Goodbye!
```

---

### 2. Connecting with Irssi

[Irssi](https://irssi.org/) is a classic terminal IRC client:

1. Launch Irssi:
   ```bash
   irssi
   ```

2. Connect to your running server:
   ```irc
   /connect 127.0.0.1 6667 mysecretpass
   ```

3. Set your nickname:
   ```irc
   /nick alice
   ```

4. Join channels and chat:
   ```irc
   /join #mychannel
   /topic #mychannel Welcome to ft_irc
   /msg bob Hello Bob!
   ```

---

### 3. Connecting with HexChat / GUI Clients

1. Open **HexChat** (or any IRC client like Textual, Colloquy).
2. Add a new network:
   - **Host:** `127.0.0.1/6667`
   - **Server Password:** `mysecretpass`
3. Enter your Nickname and Username.
4. Click **Connect**.

---

## 🔍 Internal Design & Implementation Highlights

### 1. Non-Blocking Multiplexing
- Socket creation with `SO_REUSEADDR` for immediate port rebinding upon restarts.
- Single-threaded event loop driven by `poll(&poll_fds[0], poll_fds.size(), -1)`.
- When `POLLIN` fires on the server file descriptor, `accept()` handles the new connection and registers it in `poll_fds`.
- When `POLLIN` fires on client descriptors, `recv()` loads incoming data without blocking other active connections.

### 2. Command Extraction & Buffer Management
- TCP streams can arrive in fragmented pieces or concatenated batches.
- The `client::appendToBuffer()` method accumulates raw bytes into a private client buffer.
- `client::extractCommand()` looks for delimiter patterns (`\n` and `\r\n`), extracts complete commands sequentially, and preserves trailing data for subsequent cycles.

### 3. Client Lifecycle & State Machine
```
   [Connect]
       │
       ▼
 [Receive PASS] ──(Wrong Password)──> [Disconnect]
       │
       ▼
 [Receive NICK] ──(Collision)───────> [433 ERR_NICKNAMEINUSE]
       │
       ▼
 [Receive USER]
       │
       ▼
 [001 RPL_WELCOME] (Client Fully Registered)
       │
       ▼
 [Normal Operations: JOIN, PRIVMSG, MODE, KICK, etc.]
       │
       ▼
 [QUIT / Disconnect] ──> [Clean all channel memberships & free fd]
```

### 4. Channel & Operator Control
- The first user to `JOIN` a non-existent channel automatically instantiates the channel and receives **Channel Operator** status (`@`).
- Channels maintain separate sets for `members`, `operators`, and `invites`.
- Mode modifications (`+i`, `+t`, `+k`, `+o`, `+l`) check whether the requesting client has operator status.
- When all members leave or disconnect from a channel, the channel instance is deallocated and erased from the server registry.

### 5. Signal Handling & Graceful Exit
- Intercepts `SIGINT`, `SIGTERM`, and `SIGQUIT`.
- Flips `g_server_shutdown` to break cleanly out of the poll loop.
- The destructor `Server::~Server()` iterates through all connected client pointers and active channels, deallocating memory and closing file descriptors without leaks.

---

## 👥 Authors

- **Mouncif Fahmi** ([@razv0n](https://github.com/razv0n))
- **Mohamed Ouardan** ([@mowardan](https://github.com/mowardan))
- **Hicham Elfatihi** ([@ElfatihiHicham](https://github.com/ElfatihiHicham))

Developed with ❤️ at **1337 Coding School** (42 Network).
