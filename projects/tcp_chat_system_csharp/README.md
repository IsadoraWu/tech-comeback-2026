# TCP Chat System — C#

## Overview

以 C# 建立的 TCP 聊天系統，目前版本為 **T1.2 TCP Connect／Disconnect（Completed / Accepted）**。Solution 包含 Avalonia 桌面 Client、Console Server、Shared 類別庫，以及兩個 xUnit 測試專案。Windows 是 GUI 驗收平台。

Server 監聽設定端點並依序服務單一 TCP Session；Client 提供 Connect／Disconnect、Connecting 期間的 Cancel 與連線狀態。尚未實作封包協定、Join、Chat 或 Leave。

## Features

- Client、Server 預設端點為 `127.0.0.1:5000`。
- 支援 `--host` 與 `--port`，在應用程式啟動前驗證參數。
- Server 可透過 Ctrl+C 取消等待並正常結束。
- Client 使用 Avalonia FluentTheme 與編譯式繫結顯示端點、狀態。
- Client、Server 分別參考 Shared，彼此沒有專案參考。
- IntegrationTests 包含 TCP 連線、終止、取消與 Server 清理測試；UnitTests 仍為骨架。

## Tech Stack

C# · .NET 10 · Avalonia 11.3.21 · xUnit 2.9.3 · Git

版本以專案檔為準。[global.json](global.json) 指定 SDK `10.0.100`，使用 `latestFeature` roll-forward 且不接受預覽版；[Directory.Build.props](Directory.Build.props) 統一設定 `net10.0`、Nullable 與 ImplicitUsings。

## Architecture

```text
TcpChat.Client (Avalonia) ──reference──→ TcpChat.Shared ←──reference── TcpChat.Server (Console)
```

Shared 目前沒有原始碼，預留給協定契約、codec 與純驗證規則。圖中的箭頭代表專案參考；目前兩個程序可建立／終止 TCP 連線，但不處理應用層訊息。詳見 [Architecture](docs/architecture.md) 與 [Protocol](docs/protocol.md)。

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

Server 啟動後顯示 `Server listening on 127.0.0.1:5000. Press Ctrl+C to stop.`，接受連線後顯示 Session connected，EOF 或 I/O 錯誤後釋放該連線並繼續 accept。Ctrl+C 停止監聽及目前 Session，正常退出為 0；端點占用等監聽錯誤退出為 1。

Client 視窗標題為 `TCP Chat System — T1.2`，初始顯示 Disconnected／Connect：

1. 啟動 Server，再按 Connect，成功後顯示 Connected / Not Joined／Disconnect。
2. Connecting 時按 Cancel 取消；Connect 最長等待 10 秒。
3. 按 Disconnect 後恢復 Disconnected；可再按 Connect。
4. Server 關閉或連線失敗時，Client 恢復 Disconnected 並顯示原因。
5. 關閉 Client 視窗會釋放連線；Server 依自身的 EOF／錯誤偵測清理，不假設双方同步完成。

Server 一次服務一個 Session；額外 TCP 連線可能排在 OS backlog，不代表支援多人。T1.2 收到的 bytes 僅被讀取及丟棄，以偵測 EOF；沒有 Protocol、Echo 或聊天室操作。網路黑洞／無通知拔線不保證立即偵測，尚未實作 heartbeat。

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

IntegrationTests 已有 TCP 生命週期測試，UnitTests 尚無案例。測試通過不代表 Windows GUI 或聊天驗收通過。先前 Linux Release 建置成功（SDK 10.0.401，0 errors、130 個環境相關 NETSDK1188 warnings）；Windows GUI Runtime Smoke Test 已由 SA 實際驗收為 **PASS**，詳見 [T1.1 驗收紀錄與 Windows 測試方式](docs/sprints/sprint-01.md#windows-gui-runtime-smoke-test)。

Connect／Disconnect 及 10 秒 Connect Timeout 已實作；Join、Chat、Leave、wire protocol 與 Request Timeout 尚未實作。完整交流藍圖的來源與目前可確認範圍見 [Roadmap](docs/roadmap.md)。

## Windows GUI smoke test

**SA 驗收結果：PASS**，Windows .NET SDK 10.0.401。從 Windows 本機暫存目錄重新建置後，GUI 成功開啟，顯示 `TCP Chat System — T1.1`、Endpoint `127.0.0.1:5000` 與初始 `Disconnected` 狀態。本次只驗收 GUI 啟動，未驗收 TCP Chat。

已知問題：SA 從 WSL UNC 路徑直接執行時遇到 `System.UriFormatException`，GUI 無法啟動。根因尚未確認；本階段使用 Windows 本機複本建置／驗收，不修改產品程式碼。詳細步驟見 [Windows GUI Runtime Smoke Test](docs/sprints/sprint-01.md#windows-gui-runtime-smoke-test)。

正式 **Source of Truth 是 WSL 中的 Git Repository**。Windows 暫存複本僅供測試，不在複本開發、提交或反向覆蓋 WSL 原始碼；每次驗收使用最新 WSL 來源建立測試複本。

## T1.2 Windows acceptance checklist

沿用上述 Windows 本機暫存複本建置方式。T1.2 已於 2026-10-13 由 SA 正式接受；本清單保留作為後續回歸測試參考，不宣稱每一延伸項目均包含在此次五項人工測試中。

- Server 啟動後 Connect 成功，狀態為 Connected / Not Joined。
- Disconnect 後按鈕回到 Connect；Server 記錄 Session ended 後再次 Connect 成功。
- 停止 Server，GUI 恢復 Disconnected；重啟 Server 後可手動 Connect。
- Server 未啟動時 Connect 顯示失敗原因且可再次操作。
- Connecting 期間可 Cancel；連線期間關窗後 Server 清理。
- GUI Connecting／Disconnecting 期間不重複提交連線操作。

T1.2 不包含 Join／Leave／Chat、Membership、Protocol Framing 或多人處理。

## T1.2 verification result

RD 在 Linux 使用 SDK 10.0.401 完成 Release 建置：**0 errors、0 warnings**。IntegrationTests：**5 passed、0 failed、0 skipped**。另以實際 Console Server 驗證三次依序接受／釋放 Session、端點占用退出 1、已有 Session 時 Ctrl+C 正常退出 0，以及同端點重啟。

本環境缺少 ICU，驗證命令使用僅限程序的 `DOTNET_SYSTEM_GLOBALIZATION_INVARIANT=1` 與 `DOTNET_SYSTEM_GLOBALIZATION_PREDEFINEDCULTURESONLY=0`；未修改產品全球化設定。測試需允許本機 Socket。**T1.2 Windows GUI Manual Smoke Test 已由 SA 驗收，五項全部 PASS，Task 狀態為 Completed / Accepted。** 詳見 [T1.2 驗收與 T1.3 Review](docs/sprints/t1-2-acceptance.md)。

### SA acceptance — 2026-10-13

Connect、Disconnect、Reconnect、Server Unavailable、Recover 五項 Windows GUI Manual Smoke Test 全部 PASS。SA 結合既有 Release Build（0 errors／0 warnings）與 TCP Integration Tests（5 passed），正式接受 T1.2。

下一項為 T1.3「最小封包收送」，目前僅 Task Review，尚未授權實作；[範圍、交付與验收](docs/sprints/t1-2-acceptance.md#t13-task-review)。
