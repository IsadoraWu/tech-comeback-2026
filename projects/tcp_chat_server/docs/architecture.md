# Architecture

[Project home](../README.md)

## Components

| Component | Responsibility |
|---|---|
| [src/main.c](../src/main.c) | Server entry point: initialize, listen, accept once, register, Echo, cleanup, exit. |
| [src/server.c](../src/server.c) / [server.h](../header/server.h) | Listener, Client pointer collection, registration, Echo loop, and cleanup. |
| [test/main.c](../test/main.c) | Production CLI entry point for `bin/client`, despite its historical location under `test/`. Input state, local commands, display, and application cleanup. |
| [src/client.c](../src/client.c) / [client.h](../header/client.h) | Client allocation/destruction, numeric IPv4 connection helper, and protocol wrappers. |
| [src/protocol.c](../src/protocol.c) / [protocol.h](../header/protocol.h) | Shared framing and complete socket transfers. |
| [Makefile](../Makefile) | Separate Server and Client executables; links pthread support for existing mutex use. |

```text
Client process                           Server process
main → connect → client_create           main → init → start → accept once
       ↓                                        ↓
read line / classify                     server_add_client → client_create
       ↓ Message                                ↓
send segment ───── length-prefixed TCP ──→ receive frame
receive Echo ←─────────────────────────── send same text
       ↓                                        ↓
display; repeat                           repeat until disconnect/error
       ↓ /q or EOF                              ↓
client_destroy → exit                     server_cleanup → exit
```

## Ownership

Before a Client object is created, its caller owns the socket. Creation failure leaves the caller responsible for closing it. After successful creation, `client_destroy()` closes the socket and frees the object.

The Server stores registered `st_Client *` objects in an allocated pointer array. If registration fails, Server main closes the accepted socket. After success, the Server owns the object; `server_echo_client()` only borrows it. Server cleanup closes the listener, destroys an initialized mutex, destroys registered clients, and frees the collection.

Client and Server cleanup are independent. Client output `Disconnected` does not acknowledge completion of Server cleanup. The Server currently exits because its main accepts only once, not because the Client invokes Server cleanup.

## Initialization and execution model

`mutex_initialized` starts false, becomes true after successful mutex initialization, and is reset during cleanup. Initialization failure releases acquired resources internally; main does not call cleanup after failed initialization. Failed bind/listen closes and invalidates the listening descriptor (`-1`).

The existing capacity of 256 entries is retained, but only one connection is accepted. No threads are created. CLI execution is synchronous: send one segment, receive its Echo, display it, then process the next segment. The existing `client_receive_loop()` is not used by the CLI.

## Current limits and deferred work

- No next-client accept loop, concurrent clients, worker threads, or broadcast.
- Command handling is local to the CLI; the Server has no command parser.
- `server_cleanup()` and `server_stop()` still return `void`. `client_destroy()` does not propagate `close()` errors. Reporting actual cleanup failures is deferred.
- Client main ignores SIGPIPE; this does not change the Server's signal behavior.
- Existing future-facing API declarations and message-type enums do not imply implemented routing or threading.

## Development workflow

Human responsibilities: requirements, scope, acceptance criteria, architecture decisions, code review/request changes, and final acceptance. Codex responsibilities: repository inspection, implementation within approved steps, build verification, and acceptance execution.
