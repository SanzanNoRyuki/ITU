using System.Windows;
using WPFWeather.Services;
using WPFWeather.ViewModels;

namespace WPFWeather.Windows
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            WeatherViewModel viewmodel = new WeatherViewModel(new WeatherDiskService());
            DataContext = viewmodel;
        }
    }
}