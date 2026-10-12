# TCP Message Protocol

[Project home](../README.md) · [Implementation](../src/protocol.c)

## Wire format

```text
4-byte unsigned payload length (network byte order / big-endian) | payload
```

The length excludes the header and C-string terminator. The current API transmits C strings using `strlen()`; it is not an arbitrary binary-data API. No message-type field is serialized, despite the enum in `protocol.h`.

Example: `Cava` is transmitted as `00 00 00 04 43 61 76 61` in hexadecimal.

## API contract

| API | Success | Failure |
|---|---|---|
| `protocol_send(fd, message)` | `0` | `-1` |
| `protocol_recv(fd, buffer, size)` | Payload byte count, including `0` for an empty frame | `-1` for disconnect, socket error, or rejected frame |

Payloads may contain up to `MAX_MESSAGE_LENGTH` (512) bytes. Receivers need an additional byte for the terminating null character. Internal transfer loops handle partial sends/receives and retry interrupted calls. TCP reads do not define message boundaries; the length prefix does.

## CLI mapping

- Supported input is printable ASCII; this is an input contract, not a character-validation feature.
- Linux `\n` ends the logical input line and is not transmitted. Other characters, including leading/trailing spaces, are preserved.
- A Message is sent in segments of at most 512 bytes, each in its own frame. The CLI waits for and displays each Echo before sending the next segment.
- There is no whole-line length limit in the CLI. Segment echoes are concatenated on screen, followed by one display newline at line completion.
- Empty input lines are ignored; the CLI does not send an extra empty frame at exact segment boundaries.
- Lines whose first character is `/` are local commands and produce no frames. `/q` closes the Client connection locally; it is not a protocol shutdown message.

## Existing limitations

Plain-text `nc` input does not implement this framing. Disconnect and receive errors share the same `-1` result. An oversized rejected frame is not drained for recovery. These behaviors are unchanged by Sprint 2.
