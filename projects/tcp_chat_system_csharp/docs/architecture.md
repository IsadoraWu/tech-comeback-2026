# Architecture

[Project home](../README.md) · [Roadmap](roadmap.md) · [Protocol](protocol.md)

## T1.2 components and dependencies

Client 與 Server 分別參考 Shared，彼此互不依賴。Shared 仍為空白，未放入 Session、Membership 或 Nickname 狀態。兩個測試專案可參考產品專案。

- Server Program：啟動參數、Ctrl+C 取消、監聽錯誤及 exit code。
- Server TcpServer：Listener、依序接受單一 Session、讀取至 EOF／錯誤、釋放 Socket、停止監聽。
- Client TcpConnection：10 秒非同步 Connect、可取消的讀取迴圈、遠端 EOF／錯誤回報、冪等 Dispose。
- Client MainWindowViewModel：UI 連線狀態與按鈕、本地取消／Disconnect、觀察終止；以 Connection 物件識別避免舊連線結果覆蓋新狀態。
- MainWindow：將 Click 委派給 ViewModel；Closed 時釋放連線。網路層不依賴 View。

## Lifecycle

Server：Start → Accept → Read until EOF/error → Dispose Session → Accept next；Ctrl+C 取消 Accept 或 Read，finally 停止 Listener。

Client：Disconnected → Connecting → Connected / Not Joined → Disconnecting → Disconnected。Connecting 可 Cancel；失敗／10 秒逾時回到 Disconnected。遠端終止由持續 Read 偵測；關窗立即取消及關閉 Socket。

GUI 操作與更新在 UI 執行緒，await 保留 UI synchronization context；Transport 使用 ConfigureAwait(false)。Dispose 可重複呼叫。取消後 Server 依自己的連線終止偵測清理，不假設與 Client 同步。

## Scope boundary

T1.2 不處理應用層 Payload；Read 僅消耗 bytes 與偵測 EOF。沒有 Framing、JSON、Join、Chat、Leave、Membership、Web API 或廣播。

單一 Session 限制位於 Server accept 流程，不硬編碼使用者。額外 TCP 連線可能留在 OS backlog；本版不提供多人服務或容量回覆。沒有 heartbeat，不保證即時偵測沒有 EOF／錯誤通知的網路中斷。

T1.1 歷史驗收見 [Sprint record](sprints/sprint-01.md)。T1.2 已於 2026-10-13 經 SA Windows GUI 五項驗收，狀態為 Completed / Accepted；見 [驗收與下項 Task Review](sprints/t1-2-acceptance.md)。
