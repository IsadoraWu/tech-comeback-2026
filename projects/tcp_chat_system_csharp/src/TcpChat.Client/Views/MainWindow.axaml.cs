using Avalonia.Controls;
using Avalonia.Interactivity;
using TcpChat.Client.ViewModels;

namespace TcpChat.Client.Views;

public partial class MainWindow : Window
{
    public MainWindow()
    {
        InitializeComponent();
        Closed += (_, _) => (DataContext as MainWindowViewModel)?.Dispose();
    }

    private async void OnConnectionClick(object? sender, RoutedEventArgs args)
    {
        if (DataContext is MainWindowViewModel viewModel)
            await viewModel.ToggleConnectionAsync();
    }
}
