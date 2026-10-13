using System.Net;
using System.Net.Sockets;
using TcpChat.Server;

if (!StartupOptions.TryParse(args, out var options, out var error))
{
    Console.Error.WriteLine(error);
    Console.Error.WriteLine("Usage: TcpChat.Server [--host <IP address>] [--port <1-65535>]");
    return 1;
}

using var shutdown = new CancellationTokenSource();
ConsoleCancelEventHandler onCancel = (_, eventArgs) =>
{
    eventArgs.Cancel = true;
    shutdown.Cancel();
};
Console.CancelKeyPress += onCancel;
try
{
    var server = new TcpServer(IPAddress.Parse(options.Host), options.Port);
    await server.RunAsync(shutdown.Token);
    Console.WriteLine("Server stopped.");
}
catch (SocketException socketError)
{
    Console.Error.WriteLine($"Server startup/listener error: {socketError.Message}");
    return 1;
}
finally
{
    Console.CancelKeyPress -= onCancel;
}
return 0;
