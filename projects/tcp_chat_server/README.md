# TCP Chat Server — Single-client Blocking Echo

## Project Overview

A C networking project for Linux, currently implemented as a single-client TCP echo server. This completed sprint focuses on message framing, explicit socket ownership, resource cleanup, and acceptance-driven development as a foundation for a future chat server.

## Tech Stack

C · Linux/WSL2 · POSIX Sockets · TCP/IP · Make · Git · Codex

## Current Scope

One TCP connection per process, blocking I/O, and repeated text Echo on the same connection. After the Client disconnects, the Server cleans up and exits.

## Protocol

**4-byte network-order (big-endian) payload length + payload.** The C-string API handles partial TCP transfers; plain-text `nc` input is not compatible.

## Key Engineering Decisions

- **Explicit socket ownership:** the caller owns an accepted socket until registration succeeds.
- **Server-owned clients:** registration uses `client_create()` and stores each `st_Client *`; Echo borrows it. Cleanup uses `client_destroy()` to close its socket and free the object, and also releases the collection and listener.
- **Explicit mutex state:** `mutex_initialized` tracks initialization independently of Client storage, so cleanup only destroys an initialized mutex.
- **Deliberate scope:** establish single-client, blocking behavior and resource ownership before introducing concurrency.

`initialize → listen → accept once → register → Echo → disconnect → cleanup → exit`

## Architecture / Component Responsibilities

| Component | Responsibility |
|---|---|
| [main.c](src/main.c) | Application lifecycle and failure cleanup. |
| [server.c](src/server.c) | Listening, registration, Client collection, and Echo. |
| [client.c](src/client.c) | Client objects, connection helpers, and message wrappers. |
| [protocol.c](src/protocol.c) | Message framing and socket transfers. |

## Build / Run

Requires GCC (C11), Make, and POSIX development libraries on Linux/WSL2.

From the repository root:

```bash
cd projects/tcp_chat_server
make -B
./bin/server
```

The Server listens silently on **0.0.0.0:12345**. Use a client implementing the framing above to connect to `127.0.0.1:12345`, send messages, and disconnect. Restart the Server for another session.

`make server` builds only the Server. `bin/client` is the older two-client demonstration from `test/main.c`, **not the acceptance client**, and can block against this Server.

## Acceptance Test Results

Verified with a temporary Python client using the same framing:

- **PASS** — Build with `make -B`.
- **PASS** — Single Client TCP connection.
- **PASS** — `Cava → Cava`.
- **PASS** — `Hello → Hello` on the same connection.
- **PASS** — Disconnect → normal Server exit.

## AI-assisted Development Workflow

**Human:** requirements, scope control, acceptance criteria, architecture decisions, code review / request changes, and final acceptance.

**Codex:** repository inspection, implementation within approved scope, build verification, and acceptance test execution.

## Future Work

Planned: multi-client support, multi-threading, and broadcast.
