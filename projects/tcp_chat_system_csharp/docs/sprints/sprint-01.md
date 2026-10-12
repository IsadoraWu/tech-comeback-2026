# Sprint 1 / T1.1 — 專案骨架

[Project home](../../README.md) · [Architecture](../architecture.md) · [Protocol](../protocol.md) · [RTM](sprint-01-rtm.md) · [Roadmap](../roadmap.md)

## Goal and scope

記錄目前 T1.1 的 Solution、Console Server、Avalonia Client、Shared 與測試骨架。T1.1 名稱來自現有 README 與 UI／程式文字；Sprint 1 是沿用 C 專案結構的文件分類，完整 Sprint 範圍待交流藍圖補齊。

以下需求與決策 ID 是此次文件整理的本地識別碼，依現有實作歸納，不宣稱是交流紀錄中的原始編號或已完成的正式驗收。

## Functional requirements

| ID | 現有實作對應需求 |
|---|---|
| FR-01 | Client、Server 可設定端點，預設 127.0.0.1:5000。 |
| FR-02 | 啟動前拒絕無效、未知、重複或缺值參數，回傳 1。 |
| FR-03 | Server 顯示設定端點並等待，Ctrl+C 可結束骨架。 |
| FR-04 | Client 顯示設定端點與 Disconnected，關閉視窗結束。 |

## Technical requirements

| ID | 現有設定對應需求 |
|---|---|
| TR-01 | Solution 包含 Client、Server、Shared、UnitTests、IntegrationTests。 |
| TR-02 | 統一 net10.0、Nullable、ImplicitUsings，使用穩定版 .NET 10 SDK。 |
| TR-03 | Client、Server 各自參考 Shared，彼此無 ProjectReference。 |
| TR-04 | Client 使用 Avalonia desktop lifetime、FluentTheme 與編譯式繫結；GUI 驗收平台為 Windows。 |
| TR-05 | 測試專案具 xUnit 與 Test SDK 設定；T1.1 尚無案例。 |

## Architecture decisions reflected in code

| ID | 已存在的選擇 |
|---|---|
| AD-01 | 使用三個產品專案與兩個測試專案，共用 Directory.Build.props。 |
| AD-02 | Client／Server 各自以 StartupOptions record 解析成對參數；IP 正規化後儲存，port 使用 invariant culture 與 NumberStyles.None。 |
| AD-03 | Server 使用 CancellationTokenSource 與可取消的無限 Task.Delay；Ctrl+C 設定 Cancel 為 true 並取消 token。 |
| AD-04 | Server 在 finally 解除 CancelKeyPress 訂閱，由 using 釋放 CancellationTokenSource。 |
| AD-05 | Client 以 STAThread 入口啟動平台偵測與 desktop lifetime；App 建立 View 與 ViewModel。 |
| AD-06 | ViewModel 只提供唯讀資料；Compiled Binding 使用 x:DataType。 |
| AD-07 | Shared 保持空白；不在骨架中假設協定、聊天或 timeout 已完成。 |

## Input and lifecycle

| 輸入／操作 | 原始碼預期行為 |
|---|---|
| 無參數 | 使用 127.0.0.1:5000。 |
| `--host 127.0.0.1 --port 5001` | 採用指定端點；參數順序可交換。 |
| `--host ::1` | 接受 IPv6；顯示字串未加入 URI 格式的方括號。 |
| `--host localhost` | 不接受 DNS 名稱，回傳 1。 |
| `--port 0` / `--port 65536` / `--port abc` | 回傳 1；port 不接受正負號或空白。 |
| `--port` / `--unknown x` / 重複 `--port` | 回傳 1。 |
| Server Ctrl+C | 顯示停止文字、解除事件訂閱、釋放取消來源、回傳 0。 |
| Client 關窗 | 結束 desktop lifetime；沒有斷線訊息或網路清理。 |

## Acceptance criteria and evidence

| ID | 待執行驗收 | 本次結果 |
|---|---|---|
| AC-01 | `dotnet build TcpChatSystem.sln` 成功。 | PASS：先前 Linux Release 建置成功，SDK 10.0.401；SA 亦回報 Windows 本機重新建置後 GUI 成功啟動。 |
| AC-02 | Client／Server 預設、自訂、IPv6 與無效參數行為符合規格。 | 靜態核對完成；程序驗收未執行。 |
| AC-03 | Server 顯示啟動文字，Ctrl+C 後正常結束且 exit code 為 0。 | PASS：先前 Linux 預設／自訂端點啟停檢查，exit code 0。 |
| AC-04 | Windows Client 視窗顯示端點與固定狀態，可正常關閉。 | Windows GUI Runtime Smoke Test：PASS（SA 回報，SDK 10.0.401）；已確認視窗啟動、端點及 Disconnected 顯示，回報未另列關窗結果。 |
| AC-05 | 專案結構與參考方向符合 TR-01／02／03／05。 | PASS：五個 Projects 建置成功；Client／Server 各自只參考 Shared。 |

本次更新只涉及文件。以上區分先前 RD 執行結果與本次 SA Windows Smoke Test 回報；未重新執行建置或 GUI 測試。AC-02 的 IPv6 等完整案例仍沿用待驗收狀態。

## Verification method

在專案目錄執行：

```sh
dotnet build TcpChatSystem.sln
dotnet test TcpChatSystem.sln
dotnet run --project src/TcpChat.Server -- --host 127.0.0.1 --port 5001
dotnet run --project src/TcpChat.Server -- --port 0
dotnet run --project src/TcpChat.Client -- --host 127.0.0.1 --port 5001
```

Server 正常案例需另按 Ctrl+C，再檢查程序 exit code；無效參數案例應回傳 1。Client 需在 Windows 檢查視窗與關窗行為；參數失敗應發生在 GUI 啟動前。其他邊界案例見上表，兩個入口都需驗證。

`dotnet test` 目前沒有案例可執行，不能作為聊天或 GUI 的接受證據。將來記錄結果時應包含平台、SDK、執行命令及實際輸出／exit code。

## Completion boundary

T1.1 骨架已建置，Server 啟停已驗證，Windows GUI Runtime Smoke Test 已由 SA 驗收為 PASS。TCP listener、Client connection、Connect／Join／Chat／Leave、codec、timeout 與測試案例尚未完成。下一步是已規劃的 T1.2；未在本次文件更新中開始實作。

## Windows GUI Runtime Smoke Test

**Result：PASS（SA 實際驗收回報）**

- Windows .NET SDK：10.0.401。
- 在 Windows 本機暫存目錄重新建置後，Avalonia GUI 成功開啟。
- 顯示視窗標題 `TCP Chat System — T1.1`、Endpoint `127.0.0.1:5000`、初始 `Disconnected` 狀態。
- 僅驗收 Skeleton GUI 啟動；未測試 TCP Chat，亦不代表其他未實作功能通過。

### Known issue: WSL UNC execution

SA 從 WSL UNC 路徑直接執行時遇到 `System.UriFormatException`，Avalonia GUI 無法啟動。這是本次測試觀察，根因尚未確認，不推定所有 UNC 路徑或所有 Avalonia 程式都會失敗。T1.1 使用 Windows 本機建置作為驗收方式，本次不修改程式碼處理此問題。

### Windows local build procedure

在 Windows PowerShell 操作。將 `$source` 改為實際 WSL 發行版與專案路徑；每次建立新的本機暫存複本，排除跨平台建置產物：

```powershell
$source = '\\wsl.localhost\<Distro>\home\aineko\projects\tech-comeback-2026\projects\tcp_chat_system_csharp'
$testDirectory = Join-Path $env:TEMP ('tcp-chat-t11-' + [guid]::NewGuid().ToString('N'))
robocopy $source $testDirectory /E /XD bin obj .git .vs
if ($LASTEXITCODE -ge 8) { throw 'Project copy failed.' }
Set-Location $testDirectory
dotnet --version
dotnet build TcpChatSystem.sln --configuration Release
if ($LASTEXITCODE -ne 0) { throw 'Build failed.' }
dotnet run --project src/TcpChat.Client --configuration Release --no-build -- --host 127.0.0.1 --port 5000
```

確認視窗標題、端點與 Disconnected 顯示，並關窗確認程序結束。上述命令為後續可重現驗收方式，不宣稱 SA 原先逐字執行了相同命令。`robocopy` 的 exit code 0～7 不視為複製失敗。

正式 Source of Truth 維持 WSL Git Repository；本機暫存目錄只供測試，不作為開發或提交來源，不將本機產物覆蓋回 WSL。

## Next planned task: T1.2

- Server bind/listen，接受單一 TCP Session；各次連線建立獨立 Session，不硬編碼使用者。
- GUI Connect／Disconnect、Connection State、基本操作互斥與取消。
- 本地 Disconnect 與 Server 自身 EOF／I/O 終止偵測、清理。
- 驗收真實 TCP 建立、終止，以及 Server 清理後可接受下一次連線。
- 不實作 Framing／JSON、Join／Chat／Leave、Membership 或多人管理；這些留在後續子任務。

目前僅記錄規劃，等待 T1.2 實作授權。
