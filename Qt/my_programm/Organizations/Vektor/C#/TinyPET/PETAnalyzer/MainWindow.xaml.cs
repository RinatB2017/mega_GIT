using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms.DataVisualization.Charting;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace PETAnalyzer
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            this.floodmapViewer.Palette = new FloodmapGrayscalePalette(0);


            // Настройка спектрометра
            // Все графики находятся в пределах области построения ChartArea, создадим ее
            chart.ChartAreas.Add(new ChartArea("Default"));
            chart.ChartAreas[0].AxisX.LineColor = System.Drawing.Color.Black;
            //chart.ChartAreas[0].BorderColor = System.Drawing.Color.Green;
            //chart.ChartAreas[0].AxisX.InterlacedColor = System.Drawing.Color.Gray;
            //chart.ChartAreas[0].AxisX.LineDashStyle = ChartDashStyle.Dot;

            //// Добавим линию, и назначим ее в ранее созданную область "Default"
            //chart.Series.Add(new Series("Series1"));
            //chart.Series["Series1"].ChartArea = "Default";
            //chart.Series["Series1"].ChartType = SeriesChartType.Line;

            //// добавим данные линии
            //string[] axisXData = new string[] { "a", "b", "c" };
            //double[] axisYData = new double[] { 0.1, 1.5, 1.9 };
            //chart.Series["Series1"].Points.DataBindXY(axisXData, axisYData);


            //this.fl
        }
        EnergySpectrum spectr;


        void Refresh()
        {
            Floodmap floodmap = new Floodmap(1024);
            spectr = new EnergySpectrum(0xffff * 4, 4 * 1024);

            PETEvent petEvent = new PETEvent();
            int size = Marshal.SizeOf(petEvent);

            FileStream fs = new FileStream(this.fileNameBox.Text, FileMode.Open);
            byte[] buf = new byte[size];

            double minEnergy = spectr.MaxEnergy * minSlider.Value / minSlider.Maximum;
            double maxEnergy = spectr.MaxEnergy * maxSlider.Value / maxSlider.Maximum;

            int flagsMask = 0x80;// 0x58;

            long eventCnt = 0;
            long skipCnt = 0;

            try
            {
                while (fs.Read(buf, 0, size) > 0)
                {
                    GCHandle handle = GCHandle.Alloc(buf, GCHandleType.Pinned);
                    petEvent = (PETEvent)Marshal.PtrToStructure(handle.AddrOfPinnedObject(), typeof(PETEvent));
                    handle.Free();

                    eventCnt++;

                    //   if (petEvent.Flags != 0) MessageBox.Show(petEvent.Flags.ToString());
                    if (petEvent.Flags != 0) { skipCnt++; continue; }
                    //if ((petEvent.Flags & flagsMask) != 0) { skipCnt++; continue; }
                    //if (petEvent.Source != 2) continue;

                    /*
                    double R = 40.0;

                    petEvent.XP = (ushort)(petEvent.XP / R);
                    petEvent.XN = (ushort)(petEvent.XN / R);
                    petEvent.YP = (ushort)(petEvent.YP / R);
                    petEvent.YN = (ushort)(petEvent.YN / R);
                    */

                    if (petEvent.XP < 0 || petEvent.XN < 0 || petEvent.YP < 0 || petEvent.YN < 0) { skipCnt++; continue; }
                    if (petEvent.XP + petEvent.XN == 0 || petEvent.YP + petEvent.YN == 0) { skipCnt++; continue; }

                    double dx = ((double)petEvent.XP - (double)petEvent.XN) / ((double)petEvent.XP + (double)petEvent.XN);
                    double dy = ((double)petEvent.YP - (double)petEvent.YN) / ((double)petEvent.YP + (double)petEvent.YN);

                    //double dx = ((double)((int)(petEvent.XP / R)) - (double)((int)(petEvent.XN / R))) / ((double)((int)(petEvent.XP / R)) + (double)((int)(petEvent.XN / R)));
                    //double dy = ((double)((int)(petEvent.YP / R)) - (double)((int)(petEvent.YN / R))) / ((double)((int)(petEvent.YP / R)) + (double)((int)(petEvent.YN / R)));

                    double energy = (double)petEvent.XP + (double)petEvent.XN + (double)petEvent.YP + (double)petEvent.YN;

                    spectr.AddFlash(energy);

                    if (energy < minEnergy || energy > maxEnergy) continue;
                    floodmap.AddFlash(dx, dy);

                    if (checkBox.IsChecked == true)
                        if (MessageBox.Show(String.Format("EVENT XP = {0} XN = {1} YP = {2} YN = {3}",
                        petEvent.XP.ToString(), petEvent.XN.ToString(), petEvent.YP.ToString(), petEvent.YN.ToString()), "", MessageBoxButton.OKCancel) == MessageBoxResult.Cancel) break;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("ОШИБКА: " + ex.Message);
                MessageBox.Show(String.Format("XP = {0} XN = {1} YP = {2} YN = {3}",
                    petEvent.XP.ToString(), petEvent.XN.ToString(), petEvent.YP.ToString(), petEvent.YN.ToString()));
                throw;
            }
            finally
            {
                fs.Close();
            }

            //this.floodmapViewer.Palette = new FloodmapGrayscalePalette((int)floodmap.MaxCount + 1);
            this.floodmapViewer.Palette = new FloodmapGradientPalette((int)floodmap.MaxCount + 1);
            this.floodmapViewer.Floodmap = floodmap;

            if (skipCnt != 0)
            {
                MessageBox.Show(String.Format("Пропущено {0} событий из {1} ({2}%)", skipCnt, eventCnt, (double)(100.0 * (double)skipCnt / (double)eventCnt)));
            }

            SSS();
        }



        private void button1_Click(object sender, RoutedEventArgs e)
        {

            Refresh();


        }





        private void button_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();
            dlg.ShowDialog();

            this.fileNameBox.Text = dlg.FileName;

            Refresh();


            //Floodmap floodmap = new Floodmap(1024);
            //spectr = new EnergySpectrum(65536 * 4, 4*1024);

            //PETEvent petEvent = new PETEvent();

            //int size = Marshal.SizeOf(petEvent);

            //Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();
            //dlg.ShowDialog();

            //this.fileNameBox.Text = dlg.FileName;

            //FileInfo fi = new FileInfo(dlg.FileName);
            //long eventTotal = fi.Length / size;

            //progressBar.Value = 0.0;

            //FileStream fs = new FileStream(dlg.FileName, FileMode.Open);
            //byte[] buf = new byte[size];

            //double minEnergy = spectr.MaxEnergy * minSlider.Value / minSlider.Maximum;
            //double maxEnergy = spectr.MaxEnergy * maxSlider.Value / maxSlider.Maximum;

            //try
            //{
            //    while (fs.Read(buf, 0, size) > 0)
            //    {
            //        GCHandle handle = GCHandle.Alloc(buf, GCHandleType.Pinned);
            //        petEvent = (PETEvent)Marshal.PtrToStructure(handle.AddrOfPinnedObject(), typeof(PETEvent));
            //        handle.Free();

            //        double dx = ((double)petEvent.XP - (double)petEvent.XN) / ((double)petEvent.XP + (double)petEvent.XN);
            //        double dy = ((double)petEvent.YP - (double)petEvent.YN) / ((double)petEvent.YP + (double)petEvent.YN);

            //   //     if (petEvent.Source != 2) continue;



            //        if (petEvent.XP + petEvent.XN == 0 || petEvent.YP + petEvent.YN == 0) continue;

            //        double energy = petEvent.XP + petEvent.XN + petEvent.YP + petEvent.YN;

            //        spectr.AddFlash(energy);

            //        if (energy < minEnergy || energy > maxEnergy) continue;


            //        floodmap.AddFlash(dx, dy);



            //    }
            //}
            //catch (Exception ex)
            //{
            //    MessageBox.Show("ОШИБКА: " + ex.Message);
            //    MessageBox.Show(String.Format("XP = {0} XN = {1} YP = {2} YN = {3}",
            //        petEvent.XP.ToString(), petEvent.XN.ToString(), petEvent.YP.ToString(), petEvent.YN.ToString()));
            //    throw;
            //}
            //finally
            //{
            //    fs.Close();
            //}

            ////this.floodmapViewer.Palette = new FloodmapGrayscalePalette((int)floodmap.MaxCount + 1);
            //this.floodmapViewer.Palette = new FloodmapGradientPalette((int)floodmap.MaxCount + 1);
            //this.floodmapViewer.Floodmap = floodmap;


            //SSS();

            // Spectrometer sp = new Spectrometer();
            // sp.Show();
        }

        //using System.

        private void SSS()
        {
            //throw new NotImplementedException();

            // Добавим линию, и назначим ее в ранее созданную область "Default"
            chart.Series.Clear();

            chart.Series.Add(new Series("Series1"));
            chart.Series["Series1"].ChartArea = "Default";
            chart.Series["Series1"].ChartType = SeriesChartType.FastLine;

           // MessageBox.Show(spectr.Points.Length.ToString());

            chart.Series["Series1"].Points.Add(spectr.Points);

            chart.Series["Series1"].Color = System.Drawing.Color.Purple;


            for (int i = 0; i < spectr.Points.Length; i++)
            {
                chart.Series["Series1"].Points.Add(spectr.Points[i]);
                //chart.Series["Series1"].Points.AddXY((int)((i + 1) * spectr.Step), spectr.Points[i]);
            }

            //chart.Series["Series1"].AxisLabel = "fddfdf";

            // добавим данные линии
            //string[] axisXData = new string[] { "a", "b", "c" };
            //double[] axisYData = new double[] { 0.1, 1.5, 1.9 };
            // chart.Series["Series1"].Points.DataBindXY(axisXData, axisYData);
        }

        private void radiusSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            Slider s = (Slider)sender;
            if (s == xSlider)
            {
                floodmapViewer.pm.X = e.NewValue;
            }

            if (s == ySlider)
            {
                floodmapViewer.pm.Y = e.NewValue;
            }
            if (s == radiusSlider)
            {
                floodmapViewer.pm.Radius = e.NewValue;
            }
            floodmapViewer.InvalidateVisual();
        }

        private void minSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            //chart.ChartAreas["Default"].CursorX = new System.Windows.Forms.DataVisualization.Charting.Cursor();

            chart.ChartAreas["Default"].CursorX.AxisType = AxisType.Primary;

            //chart.ChartAreas["Default"].CursorX.LineColor = System.Drawing.Color.Red;
            //chart.ChartAreas["Default"].CursorX.Position = 1000.0;
            //chart.ChartAreas["Default"].CursorX.IsUserEnabled = true;
            //chart.ChartAreas["Default"].CursorX.IsUserSelectionEnabled = true;

           // chart.ChartAreas["Default"].CursorX.SelectionColor = System.Drawing.Color.Red;
          //  chart.ChartAreas["Default"].CursorX.SelectionStart = 100.0;
          //  chart.ChartAreas["Default"].CursorX.SelectionEnd = 1000.0;

            if (sender == minSlider)
            {
                chart.ChartAreas["Default"].CursorX.SelectionStart = spectr.Points.Length * minSlider.Value / minSlider.Maximum;
            }

            if (sender == maxSlider)
            {
                chart.ChartAreas["Default"].CursorX.SelectionEnd = spectr.Points.Length * maxSlider.Value / maxSlider.Maximum;
            }


           // double minEnergy = spectr.MaxEnergy * minSlider.Value / minSlider.Maximum;
           // double maxEnergy = spectr.MaxEnergy * maxSlider.Value / maxSlider.Maximum;
        }
    }
}
