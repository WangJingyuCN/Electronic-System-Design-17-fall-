using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace CP16
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private List<Task> tasks = new List<Task>();
        private List<StackPanel> stackPanels = new List<StackPanel>();
        private string lastSendedStr = "";

        private Thread backThread;

        private bool isRunning = true;
        private bool isRepeat = false;

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            string text2send = $"{Txt1.Text}-DE-{Txt2.Text}-{InputTextBox.Text}";

            var stackPanel = new StackPanel {Orientation = Orientation.Horizontal};
            stackPanel.Children.Add(new Label {Content = text2send});
            stackPanel.Children.Add(new Label {Content = "发送中...", Foreground = Brushes.Red});
            TextListBox.Items.Add(stackPanel);

            if (!TextListBox.Items.IsEmpty)
            {
                TextListBox.ScrollIntoView(TextListBox.Items[TextListBox.Items.Count - 1]);
            }

            var sound = CP16Library1.CP16Library1.getSound(text2send);
            tasks.Add(PlaySound(sound,65535));
            stackPanels.Add(stackPanel);

            if (backThread == null)
            {
                backThread = new Thread(backFunc);
                backThread.Start();
            }

            lastSendedStr = InputTextBox.Text;
            repeatCheckBox.Content = $"重复发送:\"{lastSendedStr}\"";
            InputTextBox.Text = "";
        }

        private void backFunc()
        {
            while (isRunning)
            {
                if (tasks.Any() && stackPanels.Any())
                {
                    tasks.First().Wait();
                    tasks.RemoveAt(0);
                    Dispatcher.Invoke(delegate
                    {
                        ((Label) stackPanels.First().Children[1]).Content = "已发送";
                        ((Label) stackPanels.First().Children[1]).Foreground = Brushes.LawnGreen;
                    });
                    stackPanels.RemoveAt(0);
                }
                else
                {
                    if (isRepeat && lastSendedStr != "")
                    {
                        Dispatcher.Invoke(delegate
                        {
                            InputTextBox.Text = lastSendedStr;
                            Button_Click(null, null);
                        });
                    }
                    Thread.Sleep(100);
                }
            }
        }

        private Task PlaySound(float[] sound, UInt16 volume = 16383)
        {          
            return Task.Run(() => {
                var mStrm = new MemoryStream();
                BinaryWriter writer = new BinaryWriter(mStrm);

                int formatChunkSize = 16;
                int headerSize = 8;
                short formatType = 1;
                short tracks = 1;
                int samplesPerSecond = 8000;
                short bitsPerSample = 16;
                short frameSize = (short)(tracks * ((bitsPerSample + 7) / 8));
                int bytesPerSecond = samplesPerSecond * frameSize;
                int waveSize = 4;
                int samples = sound.Length;
                int dataChunkSize = samples * frameSize;
                int fileSize = waveSize + headerSize + formatChunkSize + headerSize + dataChunkSize;
                // var encoding = new System.Text.UTF8Encoding();
                writer.Write(0x46464952); // = encoding.GetBytes("RIFF")
                writer.Write(fileSize);
                writer.Write(0x45564157); // = encoding.GetBytes("WAVE")
                writer.Write(0x20746D66); // = encoding.GetBytes("fmt ")
                writer.Write(formatChunkSize);
                writer.Write(formatType);
                writer.Write(tracks);
                writer.Write(samplesPerSecond);
                writer.Write(bytesPerSecond);
                writer.Write(frameSize);
                writer.Write(bitsPerSample);
                writer.Write(0x61746164); // = encoding.GetBytes("data")
                writer.Write(dataChunkSize);

                double amp = volume >> 2; // so we simply set amp = volume / 2
                foreach (float val in sound)
                {
                    short s = (short)(amp * val);
                    writer.Write(s);
                }

                mStrm.Seek(0, SeekOrigin.Begin);
                new System.Media.SoundPlayer(mStrm).PlaySync();
                //new System.Media.SoundPlayer("I:\\three.wav").Play();
                //Thread.Sleep(sound.Length/8 + 1000);
                writer.Close();
                mStrm.Close();
            });
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            isRunning = false;
        }

        private void repeatCheckBox_Unchecked(object sender, RoutedEventArgs e)
        {
            isRepeat = false;
        }

        private void repeatCheckBox_Checked(object sender, RoutedEventArgs e)
        {
            isRepeat = true;
        }
    }
}
