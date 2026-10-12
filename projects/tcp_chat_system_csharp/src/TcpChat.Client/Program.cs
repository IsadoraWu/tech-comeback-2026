using Avalonia;

namespace TcpChat.Client;

internal static class Program
{
    internal static StartupOptions Options { get; private set; } = new();

    [STAThread]
    public static int Main(string[] args)
    {
        if (!StartupOptions.TryParse(args, out var options, out var error))
        {
            Console.Error.WriteLine(error);
            Console.Error.WriteLine("Usage: TcpChat.Client [--host <IP address>] [--port <1-65535>]");
            return 1;
        }

        Options = options;
        return AppBuilder.Configure<App>()
            .UsePlatformDetect()
            .StartWithClassicDesktopLifetime([]);
    }
}
