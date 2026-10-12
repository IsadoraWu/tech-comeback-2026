namespace TcpChat.Client.ViewModels;

public sealed class MainWindowViewModel
{
    public string Endpoint { get; }
    public string Status => "Disconnected — T1.1 skeleton. TCP connection and chat are not implemented.";

    internal MainWindowViewModel(StartupOptions options)
    {
        Endpoint = $"Configured endpoint: {options.Host}:{options.Port}";
    }
}
