# Sprint 1 — Single-client Blocking Echo Server

[Project home](../../README.md) · [Architecture](../architecture.md) · [Protocol](../protocol.md)

## Goal and scope

Deliver a compilable, runnable Server that accepts one TCP Client and echoes its messages unchanged. Blocking I/O, one accepted connection per process, multiple messages on that connection. After disconnect, the Server cleans up and exits; it does not accept the next Client.

Multi-client operation, multi-threading, and broadcast were out of scope.

## Specification

| ID | Requirement |
|---|---|
| FR-01 | A single Client can connect over TCP and send messages. |
| FR-02 | The Server returns the same message to that Client. |
| TR-01 | After bind/listen failure closes the listener, set its descriptor to `-1`. |
| TR-02 | Server cleanup closes Client connections still owned by the Server. |
| AC-01 | The project compiles and runs. |
| AC-02 | One Client connects and sends messages successfully. |
| AC-03 | Sending `Cava` produces an identical `Cava` response. |
| AD-01 | The Server creates/registers Client objects and owns its Client collection. |

## Architecture decisions and review outcomes

- Store `st_Client *` entries in the Server collection; use existing `client_create()` / `client_destroy()` for individual objects.
- Registration failure leaves the accepted socket with the caller, which closes it. Successful registration transfers ownership to the Server.
- Retain `MAX_CLIENTS` capacity; the single-client scenario is enforced by accepting once, not by changing capacity to one.
- Reuse the length-prefixed protocol for a synchronous Echo loop.
- Track mutex initialization explicitly rather than inferring it from the collection pointer.
- Call Server cleanup from main only after successful initialization. Reset the listener after bind/listen failure.
- Build Server and Client separately, with one `-pthread` option per compiler invocation.

Implementation is in [server.c](../../src/server.c), [server.h](../../header/server.h), and [Server main](../../src/main.c).

## Acceptance results

| Check | Recorded result |
|---|---|
| AC-01: `make -B`, execution | PASS — successful build and Server execution, no compiler warnings. |
| AC-02: one TCP connection | PASS — connected to `127.0.0.1:12345`. |
| AC-03: `Cava → Cava` | PASS — exact 4-byte match. |
| Same connection: `Hello → Hello` | PASS — exact 5-byte match. |
| Disconnect lifecycle | PASS — Server exited within the 5-second test deadline, status 0, without a termination signal. |

These historical checks used a temporary Python client implementing the framing, executed from standard input. At Sprint 1 completion, `bin/client` was still the older two-client demonstration. Sprint 2 replaced it with the interactive CLI. No test script was checked in; individual resource releases were not instrumented.

## Completion boundary

Socket lifecycle, registration/ownership, blocking Echo, Server main, and basic build were completed. Cleanup status propagation, concurrency, and broadcast were not implemented. The current interactive CLI is documented in [Sprint 2](sprint-02.md).
