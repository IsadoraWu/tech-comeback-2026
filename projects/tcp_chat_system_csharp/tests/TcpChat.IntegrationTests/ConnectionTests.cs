using System.Net;
using System.Net.Sockets;
using TcpChat.Client.Transport;
using TcpChat.Server;
using Xunit;

namespace TcpChat.IntegrationTests;

public sealed class ConnectionTests
{
    [Fact]
    public async Task LocalDisconnectClosesPeerAndIsRepeatable()
    {
        using var listener = new TcpListener(IPAddress.Loopback, 0);
        listener.Start();
        using var connection = new TcpConnection();
        await connection.ConnectAsync(IPAddress.Loopback, ((IPEndPoint)listener.LocalEndpoint).Port,
            CancellationToken.None);
        using var peer = await listener.AcceptTcpClientAsync();
        connection.Dispose();
        connection.Dispose();
        await connection.Completion.WaitAsync(TimeSpan.FromSeconds(5));
        Assert.Equal(0, await peer.GetStream().ReadAsync(new byte[1]).AsTask()
            .WaitAsync(TimeSpan.FromSeconds(5)));
    }

    [Fact]
    public async Task RemoteCloseCompletesReceiveLoop()
    {
        using var listener = new TcpListener(IPAddress.Loopback, 0);
        listener.Start();
        using var connection = new TcpConnection();
        await connection.ConnectAsync(IPAddress.Loopback, ((IPEndPoint)listener.LocalEndpoint).Port,
            CancellationToken.None);
        using var peer = await listener.AcceptTcpClientAsync();
        peer.Dispose();
        Assert.Contains("server closed", await connection.Completion.WaitAsync(TimeSpan.FromSeconds(5)));
    }

    [Fact]
    public async Task CancelledConnectCanBeDisposedRepeatedly()
    {
        using var listener = new TcpListener(IPAddress.Loopback, 0);
        listener.Start();
        using var connection = new TcpConnection();
        using var cancellation = new CancellationTokenSource();
        cancellation.Cancel();
        await Assert.ThrowsAnyAsync<OperationCanceledException>(() => connection.ConnectAsync(
            IPAddress.Loopback, ((IPEndPoint)listener.LocalEndpoint).Port, cancellation.Token));
        connection.Dispose();
    }

    [Fact]
    public async Task ServerHandlesSequentialConnectionsAndStopsWithActiveSession()
    {
        using var shutdown = new CancellationTokenSource(TimeSpan.FromSeconds(10));
        var server = new TcpServer(IPAddress.Loopback, 0);
        var running = server.RunAsync(shutdown.Token);
        try
        {
            using (var first = new TcpConnection())
            {
                await first.ConnectAsync(IPAddress.Loopback, server.Port, shutdown.Token);
                first.Dispose();
                await first.Completion.WaitAsync(TimeSpan.FromSeconds(5));
            }
            using var second = new TcpConnection();
            await second.ConnectAsync(IPAddress.Loopback, server.Port, shutdown.Token);
            shutdown.Cancel();
            await running.WaitAsync(TimeSpan.FromSeconds(5));
            await second.Completion.WaitAsync(TimeSpan.FromSeconds(5));
        }
        finally
        {
            shutdown.Cancel();
            await running.WaitAsync(TimeSpan.FromSeconds(5));
        }
    }

    [Fact]
    public async Task ServerCanStopWhileWaitingForConnection()
    {
        using var shutdown = new CancellationTokenSource();
        var server = new TcpServer(IPAddress.Loopback, 0);
        var running = server.RunAsync(shutdown.Token);
        shutdown.Cancel();
        await running.WaitAsync(TimeSpan.FromSeconds(5));
    }
}
