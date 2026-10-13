using System.ComponentModel;
using System.Net;
using System.Net.Sockets;
using TcpChat.Client.Transport;

namespace TcpChat.Client.ViewModels;

public sealed class MainWindowViewModel : INotifyPropertyChanged, IDisposable
{
    private readonly StartupOptions options;
    private TcpConnection? connection;
    private bool closed;
    private string status = "Disconnected";
    private string buttonText = "Connect";
    private bool canToggle = true;

    public event PropertyChangedEventHandler? PropertyChanged;
    public string Endpoint { get; }
    public string Status => status;
    public string ConnectionButtonText => buttonText;
    public bool CanToggleConnection => canToggle;

    internal MainWindowViewModel(StartupOptions options)
    {
        this.options = options;
        Endpoint = $"Configured endpoint: {options.Host}:{options.Port}";
    }

    // Called on the UI thread; awaits retain the UI synchronization context.
    public async Task ToggleConnectionAsync()
    {
        if (closed) return;
        if (connection is not null)
        {
            var previous = connection;
            connection = null;
            SetState("Disconnecting", "Disconnect", false);
            previous.Dispose();
            await previous.Completion;
            if (!closed) SetState("Disconnected", "Connect", true);
            return;
        }
        if (!canToggle) return;
        var current = new TcpConnection();
        connection = current;
        SetState("Connecting", "Cancel", true);
        try
        {
            await current.ConnectAsync(IPAddress.Parse(options.Host), options.Port, CancellationToken.None);
            if (closed || !ReferenceEquals(connection, current)) return;
            SetState("Connected / Not Joined", "Disconnect", true);
            _ = ObserveTerminationAsync(current);
        }
        catch (Exception error) when (error is OperationCanceledException or SocketException
            or IOException or ObjectDisposedException)
        {
            if (closed || !ReferenceEquals(connection, current)) return;
            connection = null;
            SetState(error is OperationCanceledException
                ? "Disconnected — connection timed out or was cancelled."
                : $"Disconnected — {error.Message}", "Connect", true);
        }
    }

    private async Task ObserveTerminationAsync(TcpConnection current)
    {
        var result = await current.Completion;
        if (closed || !ReferenceEquals(connection, current)) return;
        connection = null;
        SetState(result, "Connect", true);
    }

    private void SetState(string newStatus, string text, bool enabled)
    {
        status = newStatus;
        buttonText = text;
        canToggle = enabled;
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Status)));
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ConnectionButtonText)));
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(CanToggleConnection)));
    }

    public void Dispose()
    {
        if (closed) return;
        closed = true;
        connection?.Dispose();
        connection = null;
    }
}
