# TCP Chat Server

A command-line TCP chat server written in C for Linux.

## Features

### Completed

- Protocol module with framed send/receive flow
- Message formatting helpers for text, system, and error messages
- Socket send/receive wrapper for client-side communication
- Partial `send()` / `recv()` handling with retry logic
- Basic client-side socket connection helper
- Basic client data structure and message wrapper functions

### In Progress

- TCP server socket lifecycle (`socket()` / `bind()` / `listen()` / `accept()`)
- Multi-client connection management
- Broadcast and per-client message dispatch
- POSIX thread integration for client handlers
- Protocol validation and system-message test flow

### Planned

- Multi-client chat room behavior
- Thread synchronization and client list management
- `/list` and `/quit` commands
- Unit/integration testing for full end-to-end flow
- Docker support
- GitHub Actions CI

## Tech Stack

- C
- Linux / WSL2
- POSIX Socket API
- POSIX Threads
- Git
- Docker

## Roadmap

- [x] Protocol module
- [ ] TCP server foundation
- [x] TCP client connection helpers
- [ ] Multi-client chat
- [ ] Thread synchronization
- [ ] Testing
- [ ] Docker
- [ ] CI/CD

## Purpose

Rather than following isolated tutorials, this project is developed incrementally in a workplace-style workflow:

> Learn → Build → Test → Review → Refactor

Every new concept is immediately applied to a real project.

---
Part of **tech-comeback-2026**