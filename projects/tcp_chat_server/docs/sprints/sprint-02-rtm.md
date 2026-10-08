# Sprint 2 — Requirements Traceability Matrix

[Specification and decisions](sprint-02.md) · [Project home](../../README.md)

Implementation references point to functions rather than fixed line numbers. PASS entries summarize the completed acceptance run; temporary test scripts are not checked into this repository.

| Requirement | Decisions | Implementation | Acceptance evidence | Result |
|---|---|---|---|---|
| FR-01: automatic connection | AD-01, AD-02, AD-09 | [CLI main](../../test/main.c), [client_connect_to_server](../../src/client.c) | AC-01: connect succeeds; refusal prints failure and exits 1. | PASS |
| FR-02: message input | AD-05, AD-06 | [run_message_loop](../../test/main.c) | AC-02/05: spaces, blank lines, long lines, next input. | PASS |
| FR-03: Echo display | AD-03, AD-04, AD-08 | [echo_segment](../../test/main.c), [protocol API](../../src/protocol.c) | AC-02/05: exact content, continuous segment display, one line-ending newline. | PASS |
| FR-04: `/q` disconnect | AD-07, AD-09, AD-10 | [run_message_loop and main](../../test/main.c), [client_destroy](../../src/client.c) | AC-03: local command, no frame, normal exit; later input not sent. | PASS |
| FR-05: status output | AD-02, AD-10, AD-11 | [CLI main](../../test/main.c) | AC-01/03; error injection: `Unexpected error`, then `Disconnected`. | PASS |
| TR-01: complete-line segmentation | AD-03, AD-05, AD-08 | [run_message_loop / echo_segment](../../test/main.c) | AC-05: 511/512/513/1024/1025/10000 bytes; frame sizes at most 512. | PASS |
| TR-02: classify once per line | AD-06 | [line_start / command state](../../test/main.c) | Message continuation `/q` echoed; Command continuation `/q` discarded. | PASS |
| TR-03: exact command and overflow discard | AD-07 | [command_length / command_overflow](../../test/main.c) | AC-04/06: undefined commands, 512-byte boundary, oversized lines followed by valid Message. | PASS |
| EOF completion and exit | AD-10 | [run_message_loop and main](../../test/main.c) | Partial Message flushed, commands processed locally, PTY Ctrl-D exits normally. | PASS |
| Client SIGPIPE handling | AD-11 | [signal setup](../../test/main.c) | Peer reset before sending: error output and exit 1, not signal termination. | PASS |
| Cleanup status propagation | AD-12 | Not implemented | Explicitly moved to TODO; not an acceptance claim. | Deferred |

Server integration uses [server_echo_client](../../src/server.c) and the existing [Server lifecycle](../../src/main.c). Current normal disconnect exits the Server; future multi-client lifecycle is not implemented.
