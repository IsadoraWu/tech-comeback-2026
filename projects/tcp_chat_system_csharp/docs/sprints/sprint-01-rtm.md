# Sprint 1 / T1.1 — Requirements Traceability Matrix

[Specification and decisions](sprint-01.md) · [Project home](../../README.md)

識別碼來自本次文件整理；原始藍圖編號尚待補齊。以下「靜態核對」只表示與檔案內容一致，不代表執行驗收 PASS。參照使用檔案／型別／函式，避免依賴固定行號。

| 需求 | 決策 | 實作位置 | 驗收對應 | 狀態 |
|---|---|---|---|---|
| FR-01：預設與自訂端點 | AD-02 | Client／Server [StartupOptions](../../src/TcpChat.Client/StartupOptions.cs) / [StartupOptions](../../src/TcpChat.Server/StartupOptions.cs) 的 TryParse | AC-02：預設、自訂、IPv6 | 靜態核對；執行待驗收 |
| FR-02：錯誤參數回傳 1 | AD-02 | 上述 TryParse；Client [Main](../../src/TcpChat.Client/Program.cs) / Server [入口](../../src/TcpChat.Server/Program.cs) | AC-02：未知、重複、缺值、IP 與 port 邊界 | 靜態核對；執行待驗收 |
| FR-03：Server 啟停 | AD-03、AD-04 | [Server Program.cs](../../src/TcpChat.Server/Program.cs)：onCancel、取消等待、finally | AC-03：輸出、Ctrl+C、exit code 0 | 先前 Linux 程序啟停 PASS |
| FR-04：Client 靜態畫面 | AD-05、AD-06 | [App](../../src/TcpChat.Client/App.axaml.cs)、[View](../../src/TcpChat.Client/Views/MainWindow.axaml)、[ViewModel](../../src/TcpChat.Client/ViewModels/MainWindowViewModel.cs) | AC-04：端點、狀態、關窗 | Windows GUI 啟動／端點／狀態 PASS（SA）；關窗未另列結果 |
| TR-01：五個專案 | AD-01 | [Solution](../../TcpChatSystem.sln) | AC-01、AC-05 | 設定已核對；先前 Release 建置 PASS |
| TR-02：.NET 設定 | AD-01 | [Directory.Build.props](../../Directory.Build.props)、[global.json](../../global.json) | AC-01、AC-05 | SDK 10.0.401；先前 Release 建置 PASS |
| TR-03：產品參考方向 | AD-01、AD-07 | [Client csproj](../../src/TcpChat.Client/TcpChat.Client.csproj)、[Server csproj](../../src/TcpChat.Server/TcpChat.Server.csproj)、[Shared csproj](../../src/TcpChat.Shared/TcpChat.Shared.csproj) | AC-05 | 專案參考已靜態核對 |
| TR-04：Avalonia 桌面 UI | AD-05、AD-06 | [Client Program](../../src/TcpChat.Client/Program.cs)、[App XAML](../../src/TcpChat.Client/App.axaml)、[MainWindow](../../src/TcpChat.Client/Views/MainWindow.axaml) | AC-01、AC-04 | 建置 PASS；Windows GUI Runtime Smoke Test PASS（SA） |
| TR-05：xUnit 骨架 | AD-01 | [UnitTests](../../tests/TcpChat.UnitTests/TcpChat.UnitTests.csproj)、[IntegrationTests](../../tests/TcpChat.IntegrationTests/TcpChat.IntegrationTests.csproj) | AC-05；尚無行為測試 | 設定已核對；沒有測試案例 |

## Deferred requirements

Connect、Join、Chat、Leave、wire protocol 與 timeout 沒有實作位置或驗收證據；Shared 尚無契約、codec 或純驗證規則。詳細需求、ID、決策與後續驗收需待完整藍圖確認，見 [Roadmap](../roadmap.md) 與 [Protocol](../protocol.md)。
