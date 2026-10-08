# Sprint 2 — Interactive CLI Echo Client

[Project home](../../README.md) · [Architecture](../architecture.md) · [Protocol](../protocol.md) · [RTM](sprint-02-rtm.md)

## Goal and scope

Let a user type printable ASCII text into a CLI, send it to the existing Echo Server, and see the unchanged response. The Client uses one blocking TCP connection; commands are local. Multi-client support, multi-threading, broadcast, and cleanup status API changes are excluded.

## Functional requirements

| ID | Requirement |
|---|---|
| FR-01 | Automatically connect when the Client starts. |
| FR-02 | Allow the user to enter messages. |
| FR-03 | Display echoed messages. |
| FR-04 | Allow `/q` to actively disconnect. |
| FR-05 | Display connection and disconnection status. |

## Technical requirements

| ID | Requirement |
|---|---|
| TR-01 | Preserve a whole logical line, including spaces; read and process further segments when it exceeds one buffer. |
| TR-02 | Classify a line only once, from whether its first character is `/`; subsequent segments retain that type. |
| TR-03 | Only the complete command `/q` is defined. Other slash-prefixed lines are not sent or executed. Commands exceeding 512 bytes are discarded through line end; this whole-line limit does not apply to Messages. |

## Architecture decisions

| ID | Decision |
|---|---|
| AD-01 | Use `test/main.c` as the CLI entry point; static helpers separate the interaction loop and segment exchange. |
| AD-02 | Connect once to `127.0.0.1:12345`; display `Connected` on success or `Connected Failed...` and exit on failure. No reconnect loop. |
| AD-03 | Single-threaded blocking send → receive → display for each segment. No immediate disconnect detection while waiting for stdin. |
| AD-04 | Reuse length-prefixed framing. One Message segment per frame; commands remain local. |
| AD-05 | Message payload segments contain at most 512 bytes, plus local terminator storage. Remove only `\n`, preserve spaces, ignore empty lines. |
| AD-06 | Classify from the first character and retain that state until line end. |
| AD-07 | Command maximum is 512 bytes including `/`, excluding `\n` and any local terminator. Execute only exact `/q` after line completion; discard excess command content. |
| AD-08 | Display each Echo immediately and flush; add a newline only at logical-line completion, without accumulating a whole response line. |
| AD-09 | The caller owns the connected socket until Client creation succeeds; afterwards `client_destroy()` owns close/free duties. |
| AD-10 | `/q` and stdin EOF are user-requested termination. Process any unfinished input normally at EOF, then disconnect. On communication error, display `Unexpected error`, clean up, display `Disconnected`, and exit with failure. Do not wait for Server cleanup. |
| AD-11 | Ignore SIGPIPE in the Client entry point so send errors can follow the error/cleanup path. |
| AD-12 (deferred) | Proposed cleanup status propagation was explicitly moved to TODO, not implemented in this sprint. |

## Input state and lifecycle

`connect → create Client → Connected → read/classify → send/receive/display or local command → cleanup → Disconnected → exit`

The input loop uses `fgetc()` and bounded storage. Messages flush at 512 bytes or logical-line completion. Command bytes are collected up to 512; overflow switches to discard behavior until newline/EOF. The Command buffer is length-tracked, not used as a null-terminated string.

| Input | Behavior |
|---|---|
| `Hello world` | Echo with the space preserved. |
| Empty line | Ignore. A spaces-only line remains a Message. |
| `/q` | Close locally and exit; no command frame is sent. |
| `/q `, `/Q`, `/quit` | Ignore and await another line. |
| ` /q` | Message because the first character is a space. |
| Message continuation beginning `/q` | Remains Message data. |
| Oversized Command with `/q` in its remainder | Discard; do not execute the remainder. |
| EOF after unfinished Message | Echo the remaining data, finish display, and exit. |
| EOF after `/q` | Execute the complete command and exit. |

`Disconnected` is printed after the existing `client_destroy()` returns success. That API currently does not propagate `close()` failures. This message is not a Server cleanup acknowledgment. The existing connection helper may additionally print a `perror()` diagnostic to stderr.

## Acceptance criteria and recorded results

| ID | Criterion | Result |
|---|---|---|
| AC-01 | Automatically connect and display `Connected`; failure displays `Connected Failed...`. | PASS — success and connection-refused paths. |
| AC-02 | Enter text, send it, and display the identical Server Echo. | PASS — `Cava`, `Hello`, and preserved spaces. |
| AC-03 | Exact `/q` closes the connection and displays `Disconnected`. | PASS — Client and Server exit status 0. |
| AC-04 | Undefined slash-prefixed commands are not sent/executed; input continues. | PASS — frame-level observation and subsequent Message. |
| AC-05 | Long Messages are segmented, echoed per segment, and followed by another input line. | PASS — lengths 511, 512, 513, 1024, 1025, and 10000 bytes. |
| AC-06 | Oversized Commands are discarded through line end; input continues. | PASS — command lengths 511/512 at or below the limit and 513/1025/10000 above it. |

Additional checks passed: no extra empty frame at an exact 512-byte boundary; classification retained across segments; EOF with empty/partial/long input and commands; real PTY Enter and Ctrl-D; peer disconnect/reset producing `Unexpected error` then `Disconnected` with exit status 1 rather than SIGPIPE termination.

## Verification method

- `make -B` built both executables successfully.
- Strict syntax check passed without errors or warnings:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -pthread \
    -Iheader -fsyntax-only \
    src/main.c src/server.c src/client.c src/protocol.c test/main.c
```

Actual Server/Client integration checked content, display, and process exit. A temporary Python TCP peer separately observed frame lengths and command non-transmission, enforced request-response order, and injected connection failures. PTY testing checked terminal input behavior. Scripts ran from standard input and were not committed; this document records completed results, not an installed automated test suite. Source files were unchanged by acceptance execution.

## Deferred work

Multi-client support, multi-threading, and broadcast. Separately, the proposed cleanup contract would make `server_cleanup()` return `0` on success or `-1` for invalid input/any cleanup failure, continue other cleanup after failures, propagate close failures through `server_stop()` and `client_destroy()`, retain mutex state if destruction fails, and let Server main report failure. None of those API changes are part of Sprint 2.
