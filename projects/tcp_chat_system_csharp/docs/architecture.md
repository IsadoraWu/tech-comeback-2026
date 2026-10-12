# Architecture

[Project home](../README.md) · [Roadmap](roadmap.md) · [Protocol](protocol.md)

## Components

| 元件 | 目前責任 |
|---|---|
| [TcpChatSystem.sln](../TcpChatSystem.sln) | 收錄三個產品專案與兩個測試專案。 |
| [Directory.Build.props](../Directory.Build.props) / [global.json](../global.json) | 共用編譯設定與 SDK 選擇。 |
| [Server Program.cs](../src/TcpChat.Server/Program.cs) | 驗證參數、註冊 Ctrl+C、等待取消、解除事件訂閱、結束。 |
| [Server StartupOptions.cs](../src/TcpChat.Server/StartupOptions.cs) | Server 端點預設值與啟動參數解析。 |
| [Client Program.cs](../src/TcpChat.Client/Program.cs) | 驗證參數、保存 Options、啟動 Avalonia desktop lifetime。 |
| [Client StartupOptions.cs](../src/TcpChat.Client/StartupOptions.cs) | Client 端點預設值與啟動參數解析。 |
| [App.axaml.cs](../src/TcpChat.Client/App.axaml.cs) / [App.axaml](../src/TcpChat.Client/App.axaml) | 建立 MainWindow、指定 ViewModel、載入 FluentTheme。 |
| [MainWindow.axaml](../src/TcpChat.Client/Views/MainWindow.axaml) / [code-behind](../src/TcpChat.Client/Views/MainWindow.axaml.cs) | 靜態桌面畫面與編譯式繫結；code-behind 僅呼叫 InitializeComponent。 |
| [MainWindowViewModel.cs](../src/TcpChat.Client/ViewModels/MainWindowViewModel.cs) | 提供唯讀端點字串與固定狀態字串；無命令或可變連線狀態。 |
| [TcpChat.Shared](../src/TcpChat.Shared/TcpChat.Shared.csproj) | 空白類別庫；預留協定契約、codec、純驗證規則。 |
| [UnitTests](../tests/TcpChat.UnitTests/TcpChat.UnitTests.csproj) / [IntegrationTests](../tests/TcpChat.IntegrationTests/TcpChat.IntegrationTests.csproj) | xUnit 骨架；兩者均參考 Client、Server、Shared，沒有測試案例。 |

## Dependencies

```text
Client ───→ Shared ←─── Server
  ↑            ↑           ↑
  └──── each test project ─┘
```

Client 與 Server 彼此沒有 ProjectReference。Shared 尚未包含型別，所以啟動參數解析目前分別存在於兩個 executable；沒有共用的網路或驗證服務。

## Server lifecycle and ownership

```text
TryParse → failure: stderr + usage → exit 1
         → success: CancellationTokenSource
                    → subscribe Console.CancelKeyPress
                    → print configured endpoint
                    → await cancellable infinite delay
                    → Ctrl+C: cancel, suppress default termination
                    → catch expected cancellation, print stopped
                    → unsubscribe handler → dispose source → exit 0
```

CancellationTokenSource 由入口函式的 `using` 管理，事件 handler 在 `finally` 解除。此生命週期沒有 listener、accepted socket、client collection 或網路清理流程；其他非預期例外沒有通用 catch。

## Client lifecycle

```text
TryParse → failure: stderr + usage → return 1
         → success: Program.Options
                    → AppBuilder / platform detection
                    → desktop lifetime / App initialization
                    → MainWindow + MainWindowViewModel
                    → close window → desktop lifetime ends
```

`Main` 標示 `[STAThread]`。自訂參數先由 TryParse 消耗，再以空陣列啟動 Avalonia lifetime。ViewModel 建構時產生端點文字，Status 永遠回傳相同字串；尚無 INotifyPropertyChanged、Connect/Send 命令、接收迴圈或背景網路工作。

## Future boundaries

原有 README 將協定契約、codec、純驗證規則預留在 Shared，並列出 Connect、Join、Chat、Leave、timeout 為未實作功能。服務分層、執行緒／非同步模型、UI 更新方式、連線資源 ownership 與伺服器多用戶策略仍需完整藍圖或後續決策確認。不能由目前的空白專案推定已採用某一種實作。
