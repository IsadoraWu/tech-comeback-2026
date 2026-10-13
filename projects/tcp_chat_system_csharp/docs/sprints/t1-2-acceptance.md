# T1.2 Acceptance and T1.3 Task Review

[README](../../README.md) · [Roadmap](../roadmap.md)

## T1.2 — Completed / Accepted

接受日期：2026-10-13（Asia/Taipei）。證據來源：SA Acceptance Result；本次只更新文件，未重跑測試或修改程式碼。

| Windows GUI Manual Smoke Test | SA 結果 |
|---|---|
| Connect：連線成功，GUI 顯示 Connected | PASS |
| Disconnect：正常中斷，GUI 顯示 Disconnected | PASS |
| Reconnect：可再次建立 TCP 連線 | PASS |
| Server Unavailable：顯示錯誤，不卡在 Connecting | PASS |
| Recover：Server 重啟後可重新連線 | PASS |

既有 RD 證據：SDK 10.0.401 Release Build 0 errors／0 warnings；TCP Integration Tests 5 passed。SA 結合上述證據正式接受 T1.2。此結果不包含 Protocol、Join／Chat／Leave 或完整 Sprint 1 驗收。

正式 Source of Truth 維持 WSL Git Repository；Windows 暫存複本只供測試。WSL UNC 已知問題與本機建置方式沿用 T1.1 紀錄。

## T1.3 Task Review

狀態：Proposal / Awaiting Review；尚未授權 Coding。

### Scope

1. Shared 定義最小 version/type/requestId 與 join_request/join_response 契約、UTF-8 JSON Codec、4-byte Big-endian Framing、64 KiB Payload 限制。
2. 用完整讀取迴圈取代 T1.2 丟棄 bytes 的方式，處理 Header／Payload 分段及多封包連續到達。閒置 TCP 不因沒有訊息而關閉。
3. Server Protocol Handler 呼叫最小 Chat Room Service，驗證 Nickname、更新 Server Membership，回覆對應 requestId 的 Join Accepted／Rejected。權威狀態不放 Shared；不建立多人 Registry 框架。
4. Client 持續 Receive Loop 解析及分流，提供內部 Join 呼叫及最小 Pending Correlation，供整合測試使用；尚不新增 GUI Nickname／Join 控制項。
5. Join 等待採核准的 10 秒期限；本地逾時／取消／Disconnect 結束 Pending 與本地連線，Server 依自身終止偵測釋放 Session／Membership。

Nickname 原始 1～20 UTF-16 Code Units，在 Trim 前檢查；使用 string.Trim()，拒絕空白及非法字元，正規化後保存。Client 不先 Trim Nickname 再傳送。共用純規則不取代 Server 獨立驗證。

Join Notification、完整 GUI Join 操作留在 T1.4；Chat／Leave、廣播、Web API、Database 不屬於本 Task。T1.3 是部分 Protocol 交付，不代表完整 FR-02／FR-07 或 AC-01 已驗收。

### Incremental delivery

- T1.3a：確認最小欄位／錯誤契約；交付 Codec／Framing 與單元測試。
- T1.3b：Server 最小 Join 業務與真實 TCP 往返，交付 Server Integration Tests。
- T1.3c：Client Receive Loop／Correlation／Pending 結束與整合測試，回歸 T1.2。

每一步提供實際 diff、建置結果、測試結果供 Review。不新增測試專用 Echo Type；使用正式 Join Request／Response 完成往返。

### Deliverables

- Shared Protocol／Codec／純 Nickname 規則。
- Server Protocol Handler 與最小 Chat Room Service，保留 Transport／業務責任分離。
- Client 最小訊息接收／Correlation；GUI 維持 Connect／Disconnect。
- 單元與真實 TCP 整合測試、Protocol 文件及執行紀錄。

### Tests and acceptance

- Header 使用 Big-endian；Length 為 UTF-8 Payload bytes，不是 UTF-16 長度。
- 分段 Header、分段 Payload、合包及同連線多則 Request 均正確解析。
- 64 KiB 為所有封包 Payload 上限；0 或超長 Length 在配置 Buffer 前拒絕；JSON escaping 也納入 byte 計算。
- 合法 Isa 的 Join Response Accepted，requestId 一致；不合法 Nickname Rejected，原 TCP 可修改後重試。
- Server 獨立驗證原始 Nickname，Membership／Session 狀態由 Server 管理；斷線後可以新的 Session 再使用 Isa。
- Response 不得完成另一筆 Pending；Disconnect／EOF／Timeout 會結束 Pending；Receive Loop 不因等待結果停下。
- 測試 Request Timeout 的本地結果與 Server 後續清理，不假設兩者同步。
- Header／Payload 中途 EOF 不永久等待；基本不合法 UTF-8／JSON／Type／Version 路徑遵守 AD v0.2。完整安全邊界與異常矩陣仍在 T4 補齊。
- Release Build 成功，逐項提供 warnings／errors；T1.2 五個自動測試與 Windows Connect／Disconnect 行為保持正常。

對應：TR-02、TR-04、TR-05；FR-02／FR-07 與 AC-02 的部分 Protocol 路徑。T1.4 才完成 GUI Join 與加入通知驗收。

### Review decisions

請 Review T1.3a～c 的交付邊界，以及最小 Join Pending／10 秒期限納入 T1.3 的安排。核准 Task 範圍不等同 Coding 授權；取得明確實作指示後才開始。
