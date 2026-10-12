# TCP Message Protocol — 尚未實作

[Project home](../README.md) · [Architecture](architecture.md) · [Roadmap](roadmap.md)

## Current state

T1.1 沒有 TCP listener、連線、讀寫或序列化程式。[Shared](../src/TcpChat.Shared/TcpChat.Shared.csproj) 只有專案設定，尚無協定契約或 codec。因此目前沒有可供 Client／Server 互通的 wire format、訊息範例或協定 API。

設定端點 `127.0.0.1:5000` 不代表已監聽該位址。Client 的 Disconnected 是固定 UI 文字，不是協定狀態機的執行結果。

## Planned responsibility

依專案原有 README，Shared 預留給協定契約、codec 與純驗證規則；Connect、Join、Chat、Leave 與 timeout 都仍未實作。這些名稱只表示預留功能，不能直接推定為 wire message type 或命令字串。

## Decisions still requiring confirmation

| 項目 | 文件需補齊的契約 |
|---|---|
| Framing | 如何從 TCP byte stream 辨識訊息邊界；header、長度單位、byte order。 |
| Payload | 文字編碼、序列化格式、欄位、版本與最大長度。 |
| Messages | 實際訊息種類、必要欄位、請求／回應或事件關係。 |
| Session | Connect／Join／Chat／Leave 的合法順序與成功、失敗行為。 |
| Validation | 暱稱、訊息內容與其他輸入限制及錯誤回報。 |
| Timeout | 連線與各操作的期限、取消及逾時後狀態。 |
| Stream errors | 部分讀寫、EOF、截斷、非法或超大訊息的處理方式。 |
| Compatibility | 版本相容性及是否需要與 C 專案互通。 |

這些項目是待決問題，沒有在本次文件整理中指定協定值。

## Relation to the C project

C 專案已有 4-byte big-endian 長度前綴與最大 512-byte payload 的 Echo 協定。C# 目前沒有引用該實作或相容性程式碼，因此不能將該格式、`/q` 命令、Echo 行為或 C 專案驗收結果視為 C# 契約。本文件沿用文件分類，實際協定需依 C# 藍圖另行確認。

## Verification boundary

目前可檢查啟動參數與骨架畫面，無法驗證協定互通。後續實作 codec 與傳輸層時，應依確認後的契約補上實際 byte 範例、API 結果、錯誤語意與測試證據。
