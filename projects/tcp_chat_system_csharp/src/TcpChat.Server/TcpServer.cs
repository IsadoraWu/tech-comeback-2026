using System.Net;
using System.Net.Sockets;

namespace TcpChat.Server;

public sealed class TcpServer
{
    private readonly TcpListener listener;
    public int Port => ((IPEndPoint)listener.LocalEndpoint).Port;

    public TcpServer(IPAddress address, int port) => listener = new TcpListener(address, port);

    public async Task RunAsync(CancellationToken cancellationToken)
    {
        try
        {
            listener.Start();
            Console.WriteLine($"Server listening on {listener.LocalEndpoint}. Press Ctrl+C to stop.");
            while (!cancellationToken.IsCancellationRequested)
            {
                using var client = await listener.AcceptTcpClientAsync(cancellationToken);
                Console.WriteLine($"Session connected: {client.Client.RemoteEndPoint}");
                try
                {
                    // T1.2 only detects termination. Protocol parsing starts in T1.3.
                    var buffer = new byte[1024];
                    while (await client.GetStream().ReadAsync(buffer, cancellationToken) != 0) { }
                }
                catch (IOException error)
                {
                    Console.WriteLine($"Session connection error: {error.Message}");
                }
                catch (SocketException error)
                {
                    Console.WriteLine($"Session socket error: {error.Message}");
                }
                finally
                {
                    Console.WriteLine("Session ended; connection resources released.");
                }
            }
        }
        catch (OperationCanceledException) when (cancellationToken.IsCancellationRequested) { }
        finally
        {
            listener.Stop();
        }
    }
}
