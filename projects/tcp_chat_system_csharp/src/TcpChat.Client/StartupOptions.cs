using System.Globalization;
using System.Net;

namespace TcpChat.Client;

internal sealed record StartupOptions(string Host = "127.0.0.1", int Port = 5000)
{
    public static bool TryParse(string[] args, out StartupOptions options, out string? error)
    {
        options = new StartupOptions();
        error = null;
        var seen = new HashSet<string>(StringComparer.Ordinal);
        for (var index = 0; index < args.Length; index += 2)
        {
            var key = args[index];
            if (key is not ("--host" or "--port") || !seen.Add(key) || index + 1 >= args.Length)
            {
                error = $"Unknown, repeated, or incomplete argument: {key}";
                return false;
            }
            var value = args[index + 1];
            if (key == "--host")
            {
                if (!IPAddress.TryParse(value, out var address))
                {
                    error = "--host must be an IP address.";
                    return false;
                }
                options = options with { Host = address.ToString() };
            }
            else
            {
                if (!int.TryParse(value, NumberStyles.None, CultureInfo.InvariantCulture, out var port)
                    || port is < 1 or > 65535)
                {
                    error = "--port must be an integer from 1 to 65535.";
                    return false;
                }
                options = options with { Port = port };
            }
        }
        return true;
    }
}
