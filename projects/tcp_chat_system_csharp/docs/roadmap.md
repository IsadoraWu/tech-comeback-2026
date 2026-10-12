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
| Server 啟停 | 先前 Linux 程序啟停 PASS | Ctrl+C 取消等待；無 bind/listen。 |
| Avalonia Client 視窗 | Windows GUI Runtime Smoke Test PASS（SA） | SDK 10.0.401；Windows 本機複本重新建置；WSL UNC 直接執行遇到 UriFormatException，詳見 T1.1 紀錄。 |
| Shared | 已建立空白專案 | 原有 README 預留協定契約、codec、純驗證規則。 |
| Connect | 未實作 | 尚無 Client 連線與 Server accept。 |
| Join／Chat／Leave | 未實作 | 尚無流程、狀態、訊息或 UI 命令。 |
| Wire protocol | 未實作；詳細契約待確認 | 見 [Protocol](protocol.md)。 |
| Timeout | 未實作；數值與行為待確認 | 原有 README 明確列為未實作。 |
| 自動化測試 | 只有專案骨架 | 尚無單元／整合測試案例。 |

## Milestone mapping

目前唯一可從程式核對的 milestone 是 **T1.1**。在文件目錄中用 `sprint-01.md` 記錄，對應「Sprint 1 / T1.1：專案骨架」；這是文件組織方式，不推定完整交流紀錄中的 Sprint 1 範圍只有 T1.1。

後續 milestone 的順序、編號與完成條件需以完整藍圖確認，再新增相應 Spec / AD / AC 與 RTM。現階段未建立虛構的 Sprint 2 完成紀錄，也未將 C 專案的單用戶 Echo 規格移植為 C# 需求。
