# TCP Chat Server

## Overview

A TCP echo application written in C for Linux. The server handles one connection at a time within a single session, and an interactive CLI client sends text and displays the echoed response. The current implementation uses blocking I/O and exits the server after that client disconnects.

## Features

- Automatic client connection to `127.0.0.1:12345`.
- Interactive text input with spaces preserved and successive messages on one connection.
- Long messages split into 512-byte frames, with echoes displayed as one logical line.
- Local `/q` command and stdin EOF to disconnect; connection and error status messages.
- Explicit socket ownership and cleanup, with completed Sprint 1 and Sprint 2 acceptance tests.

## Tech Stack

C (C11) · Linux/WSL2 · POSIX TCP Sockets · Makefile · Git

## Architecture

```text
CLI (test/main.c) → Client helpers → Length-prefixed TCP → Server Echo
```

Both processes use the same protocol module. No worker threads are created. See [Architecture](docs/architecture.md) and [Protocol](docs/protocol.md).

## Quick Start

Requires GCC, Make, and POSIX development libraries on Linux/WSL2.

From the repository root, build and start the server in terminal 1:

```bash
cd projects/tcp_chat_server
make -B
./bin/server
```

In terminal 2, from the same project directory:

```bash
./bin/client
```

Type a message and press Enter. Enter `/q` or use Ctrl-D (stdin EOF) to end the session. The server listens silently on `0.0.0.0:12345`; restart it before starting another client session. `make server` and `make client` build each executable separately.

## Demo

With the server running, this reproducible CLI session sends two messages and quits:

```bash
printf 'Cava\nHello\n/q\n' | ./bin/client
```

Client output:

```text
Connected
Cava
Hello
Disconnected
```

In an interactive terminal, the terminal also displays what you type. No application prompt is printed.

## Documentation

- [Architecture and ownership](docs/architecture.md)
- [TCP message protocol](docs/protocol.md)
- [Sprint 1: Single-client Blocking Echo Server — Spec / AD / AC](docs/sprints/sprint-01.md)
- [Sprint 2: Interactive CLI Echo Client — Spec / AD / AC](docs/sprints/sprint-02.md)
- [Sprint 2: Requirements Traceability Matrix](docs/sprints/sprint-02-rtm.md)

Multi-client support, multi-threading, broadcast, and detailed cleanup status reporting remain future work.
