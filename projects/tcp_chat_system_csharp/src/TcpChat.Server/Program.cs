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
    Console.WriteLine($"Server skeleton started. Configured endpoint: {options.Host}:{options.Port}");
    Console.WriteLine("TCP listener is not implemented in T1.1. Press Ctrl+C to stop.");
    await Task.Delay(Timeout.InfiniteTimeSpan, shutdown.Token);
}
catch (OperationCanceledException) when (shutdown.IsCancellationRequested)
{
    Console.WriteLine("Server skeleton stopped.");
}
finally
{
    Console.CancelKeyPress -= onCancel;
}
return 0;
