# Blueprint and Implementation Status

[Project home](../README.md) · [Architecture](architecture.md) · [T1.1](sprints/sprint-01.md)

## Sources and boundary

本文件依 C# 專案原有 README、Solution、專案設定與原始碼整理；文件章節參考 `tcp_chat_server_C/README.md` 與 `docs`。

使用者另提供 [Codex 交流紀錄](https://chatgpt.com/s/cx_6accd62fc7a481918edb77554d98f0ba) 作為藍圖來源。此次讀取只取得驗證頁，未取得交流正文；因此完整藍圖、後續 Sprint 編號、協定數值與驗收要求尚待補齊。以下僅列本地檔案可確認的方向，不代表已重建完整交流藍圖。

## Status map

| 項目 | 目前狀態 | 依據／下一步 |
|---|---|---|
| .NET 10 solution 與共用設定 | 先前 Release 建置 PASS | 三個產品專案、兩個測試專案、global.json、Directory.Build.props。 |
| 啟動端點參數 | 已實作，執行驗收待完成 | Client／Server 的 StartupOptions。 |
| Server 啟停／監聽 | T1.2 Completed / Accepted | 已 bind/listen；依序 accept，Ctrl+C 取消並清理。 |
| Avalonia Client 視窗 | Windows GUI Runtime Smoke Test PASS（SA） | SDK 10.0.401；Windows 本機複本重新建置；WSL UNC 直接執行遇到 UriFormatException，詳見 T1.1 紀錄。 |
| Shared | 已建立空白專案 | 原有 README 預留協定契約、codec、純驗證規則。 |
| Connect／Disconnect | T1.2 Completed / Accepted（SA，2026-10-13） | Windows Connect／Disconnect／Reconnect／Server Unavailable／Recover 五項 PASS；Connect 最長 10 秒。 |
| Join／Chat／Leave | 未實作 | 尚無流程、狀態、訊息或 UI 命令。 |
| Wire protocol | 未實作；詳細契約待確認 | 見 [Protocol](protocol.md)。 |
| Timeout | T1.2 實作 10 秒 Connect Timeout | Join／Leave／Chat Request Timeout 尚未實作。 |
| 自動化測試 | IntegrationTests 已有 TCP 生命週期案例 | 5 passed；UnitTests 尚無案例；SA 另完成 Windows GUI 五項驗收。 |

## Milestone mapping

| Task | 狀態 | 證據／下一步 |
|---|---|---|
| T1.1：可啟動骨架 | Completed / Accepted | 既有建置及 SA Windows GUI 啟動驗收。 |
| T1.2：真實 TCP Connect／Disconnect | Completed / Accepted | 2026-10-13 SA 正式接受；Release Build 0 errors／0 warnings、TCP Integration Tests 5 passed、Windows 五項 PASS。 |
| T1.3：最小封包收送 | Task Review；未授權實作 | [範圍、交付成果、測試及驗收](sprints/t1-2-acceptance.md#t13-task-review)。 |
| T1.4～T1.6、T2～T7 | 尚未實作 | 維持已規劃順序，不因 T1.2 接受而自動取得實作授權。 |

本次進度更新以本次 SA Acceptance Result 與既有規劃為依據，不代表完整 Sprint 1 已接受。WSL Git Repository 維持正式 Source of Truth。
