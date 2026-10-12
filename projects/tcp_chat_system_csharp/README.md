# TCP Chat System — C#

## Overview

以 C# 建立的 TCP 聊天系統，目前版本為 **T1.1 專案骨架**。Solution 包含 Avalonia 桌面 Client、Console Server、Shared 類別庫，以及兩個 xUnit 測試專案。Windows 是 GUI 驗收平台。

目前 Server 只處理啟動參數與 Ctrl+C 結束流程；Client 只顯示設定端點與固定的 Disconnected 狀態。尚未實作 TCP 監聽、連線或聊天。

## Features

- Client、Server 預設端點為 `127.0.0.1:5000`。
- 支援 `--host` 與 `--port`，在應用程式啟動前驗證參數。
- Server 可透過 Ctrl+C 取消等待並正常結束。
- Client 使用 Avalonia FluentTheme 與編譯式繫結顯示端點、狀態。
- Client、Server 分別參考 Shared，彼此沒有專案參考。
- 已建立單元與整合測試專案，但尚無測試案例。

## Tech Stack

C# · .NET 10 · Avalonia 11.3.21 · xUnit 2.9.3 · Git

版本以專案檔為準。[global.json](global.json) 指定 SDK `10.0.100`，使用 `latestFeature` roll-forward 且不接受預覽版；[Directory.Build.props](Directory.Build.props) 統一設定 `net10.0`、Nullable 與 ImplicitUsings。

## Architecture

```text
TcpChat.Client (Avalonia) ──reference──→ TcpChat.Shared ←──reference── TcpChat.Server (Console)
```

Shared 目前沒有原始碼，預留給協定契約、codec 與純驗證規則。圖中的箭頭代表專案參考；目前兩個程序沒有 TCP 通訊。詳見 [Architecture](docs/architecture.md) 與 [Protocol](docs/protocol.md)。

## Quick Start

需要符合 `global.json` 的 .NET 10 SDK；首次建置需要還原 NuGet 套件。GUI 執行需要桌面環境，正式 GUI 驗收在 Windows 進行。

從 repository root 進入專案並建置：

```sh
cd projects/tcp_chat_system_csharp
dotnet build TcpChatSystem.sln
```

若已位於 `projects`，使用 `cd tcp_chat_system_csharp`。在專案目錄的兩個終端分別執行：

```sh
dotnet run --project src/TcpChat.Server -- --host 127.0.0.1 --port 5000
```

```sh
dotnet run --project src/TcpChat.Client -- --host 127.0.0.1 --port 5000
```

可省略所有參數以使用預設值。`--host` 接受 `IPAddress.TryParse` 可解析的 IP 位址，包含 IPv4、IPv6；不做 DNS 名稱解析。`--port` 接受 1–65535 的整數。未知、重複、缺值或無效參數會在啟動前回傳 exit code 1。參數名稱區分大小寫，不支援 `--host=value` 或 `--help`。

Windows Client 的 OutputType 為 `WinExe`，沒有主控台；參數錯誤診斷寫至 stderr，請透過可擷取輸出的 CLI 或工具檢查。

## Demo

以下列出骨架行為；Server 啟停已有先前程序驗證，Windows Client 啟動已由 SA 實際驗收，結果見下方。

Server 啟動後：

```text
Server skeleton started. Configured endpoint: 127.0.0.1:5000
TCP listener is not implemented in T1.1. Press Ctrl+C to stop.
```

按 Ctrl+C 後顯示 `Server skeleton stopped.`，正常路徑回傳 0。這個端點只是設定值，Server 不會 bind/listen。

Client 視窗標題為 `TCP Chat System — T1.1`，顯示設定端點，以及：

```text
Disconnected — T1.1 skeleton. TCP connection and chat are not implemented.
```

關閉視窗即可結束 Client。啟動 Client 不需要先啟動 Server；目前沒有 Connect 按鈕或可操作的聊天流程。

## Documentation

- [架構、生命週期與責任](docs/architecture.md)
- [協定現狀與待決事項](docs/protocol.md)
- [藍圖與實作狀態](docs/roadmap.md)
- [Sprint 1 / T1.1：專案骨架 — Spec / AD / AC](docs/sprints/sprint-01.md)
- [Sprint 1 / T1.1：Requirements Traceability Matrix](docs/sprints/sprint-01-rtm.md)

## Verification and limits

```sh
dotnet test TcpChatSystem.sln
```

兩個測試專案都還沒有測試案例。建置或測試命令成功，不代表 TCP 或聊天驗收通過。先前 Linux Release 建置成功（SDK 10.0.401，0 errors、130 個環境相關 NETSDK1188 warnings）；Windows GUI Runtime Smoke Test 已由 SA 實際驗收為 **PASS**，詳見 [T1.1 驗收紀錄與 Windows 測試方式](docs/sprints/sprint-01.md#windows-gui-runtime-smoke-test)。

Connect、Join、Chat、Leave、wire protocol 與 timeout 行為尚未實作。完整交流藍圖的來源與目前可確認範圍見 [Roadmap](docs/roadmap.md)。

## Windows GUI smoke test

**SA 驗收結果：PASS**，Windows .NET SDK 10.0.401。從 Windows 本機暫存目錄重新建置後，GUI 成功開啟，顯示 `TCP Chat System — T1.1`、Endpoint `127.0.0.1:5000` 與初始 `Disconnected` 狀態。本次只驗收 GUI 啟動，未驗收 TCP Chat。

已知問題：SA 從 WSL UNC 路徑直接執行時遇到 `System.UriFormatException`，GUI 無法啟動。根因尚未確認；本階段使用 Windows 本機複本建置／驗收，不修改產品程式碼。詳細步驟見 [Windows GUI Runtime Smoke Test](docs/sprints/sprint-01.md#windows-gui-runtime-smoke-test)。

正式 **Source of Truth 是 WSL 中的 Git Repository**。Windows 暫存複本僅供測試，不在複本開發、提交或反向覆蓋 WSL 原始碼；每次驗收使用最新 WSL 來源建立測試複本。
