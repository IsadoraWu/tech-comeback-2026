using System.Net;
using System.Net.Sockets;

namespace TcpChat.Client.Transport;

public sealed class TcpConnection : IDisposable
{
    private readonly TcpClient client = new();
    private readonly CancellationTokenSource lifetime = new();
    private int disposed;
    public Task<string> Completion { get; private set; } = Task.FromResult("Disconnected");

    public async Task ConnectAsync(IPAddress address, int port, CancellationToken cancellationToken)
    {
        using var timeout = CancellationTokenSource.CreateLinkedTokenSource(cancellationToken, lifetime.Token);
        timeout.CancelAfter(TimeSpan.FromSeconds(10));
        try
        {
            await client.ConnectAsync(address, port, timeout.Token).ConfigureAwait(false);
            timeout.Token.ThrowIfCancellationRequested();
            Completion = ReceiveAsync(lifetime.Token);
        }
        catch
        {
            Dispose();
            throw;
        }
    }

    private async Task<string> ReceiveAsync(CancellationToken token)
    {
        try
        {
            var buffer = new byte[1024];
            // No application payload is processed in T1.2.
            while (await client.GetStream().ReadAsync(buffer, token).ConfigureAwait(false) != 0) { }
            return "Disconnected — server closed the connection.";
        }
        catch (OperationCanceledException) when (token.IsCancellationRequested)
        {
            return "Disconnected";
        }
        catch (Exception error) when (error is IOException or SocketException or ObjectDisposedException)
        {
            return token.IsCancellationRequested ? "Disconnected" : $"Disconnected — {error.Message}";
        }
        finally
        {
            Dispose();
        }
    }

    public void Dispose()
    {
        if (Interlocked.Exchange(ref disposed, 1) != 0) return;
        lifetime.Cancel();
        client.Dispose();
        lifetime.Dispose();
    }
}
