using PET;
using PETCAT.Simulation;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace PETCmp
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            nudTInt.Value = 10;
            CanCalculate = false;
        }

        protected bool CanCalculate
        {
            get
            {
                return btnCalculate.Enabled;
            }
            set
            {
                btnCalculate.Enabled = value;
            }
        }

        private void btnOpen_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            // dlg.InitialDirectory = "c:\\";
            dlg.Filter = "Файлы данных ПЭТ (*.dat3)|*.dat3|Все файлы (*.*)|*.*";
            //dlg.FilterIndex = 2;
            dlg.RestoreDirectory = true;

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                tbFileName.Text = dlg.FileName;
                CanCalculate = true;
            }
            else
            {
                // Всё без изменений
            }
        }

        private void btnCalculate_Click(object sender, EventArgs e)
        {
            //  ShowGaus();
            //  return;

            PETEventCmp cmp = new PETEventCmp((double)nudWindow.Value, (double)nudTInt.Value, (uint)nudN.Value, (byte)nudChANumber.Value, (byte)nudChBNumber.Value);
            //PETEventCmp2 cmp2 = new PETEventCmp2((byte)nudChANumber.Value, (byte)nudChBNumber.Value, 1024);

            PETEvent petEvent = new PETEvent();
            int size = Marshal.SizeOf(petEvent);

            FileStream fs = new FileStream(tbFileName.Text, FileMode.Open);
            byte[] buf = new byte[size];

            cmp.CorrectionA = (int)nudChACorr.Value;
            cmp.CorrectionB = (int)nudChBCorr.Value;

            EventSim sim = new EventSim(-120, 50);

            if (true)
            {
                try
                {
                    while (fs.Read(buf, 0, size) > 0)
                    {
                        GCHandle handle = GCHandle.Alloc(buf, GCHandleType.Pinned);
                        petEvent = (PETEvent)Marshal.PtrToStructure(handle.AddrOfPinnedObject(), typeof(PETEvent));
                        handle.Free();

                        cmp.ProcEvent(petEvent);
                        //     cmp2.ProcEvent(petEvent);
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("ОШИБКА: " + ex.Message);
                }
                finally
                {
                    fs.Close();
                }
            }
            else
            {
                fs.Close();
                for (int i = 0; i < 1000000; i++)
                {
                    cmp.ProcEvent(sim.Next());
                }
                MessageBox.Show(cmp.tmp.ToString());
            }

            tbTotalEvents.Text = cmp.EventTotal.ToString();
            tbCmpNum.Text = String.Format("{0} ({1}%)", cmp.CmpNum.ToString(), (100.0 * cmp.CmpNum / cmp.EventTotal).ToString("F2"));

            chart.Series.Clear();

            chart.Series.Add(new Series("Series1"));
            chart.Series["Series1"].ChartArea = "Default";
            chart.Series["Series1"].ChartType = SeriesChartType.FastPoint;//.Spline;//  ;   //.FastLine;
            chart.Series["Series1"].MarkerStyle = MarkerStyle.Circle;
            chart.Series["Series1"].MarkerSize = 4;

            chart.Series["Series1"].Enabled = true;
            chart.Series["Series1"].LegendText = "P(∆t)";

            //chart.Series["Series1"].Color = System.Drawing.Color.Purple;

            //MessageBox.Show(spectr.Points.Length.ToString());
            //  chart.Series["Series1"].Points.Add((spectr.Points);
            //  chart.Series["Series1"].Color = System.Drawing.Color.Purple;
            //timeStep
            //	double delta = timeStep;//cmp.CmpWindow * timeStep / (cmp.Points.Length - 1);
            //  double start = -1.0 * cmp.CmpWindow * timeStep / 2;

            //chart.
            //chart.ChartAreas["Default"].AxisY.LineColor = Color.Red;
            //chart.ChartAreas["Default"].AxisY.MajorGrid.LineColor = Color.Gray;
            //chart.ChartAreas["Default"].AxisX.Crossing = 0;
            //chart.ChartAreas["Default"].AxisX.IntervalType = DateTimeIntervalType.Number;
            //chart.ChartAreas["Default"].AxisX.IntervalAutoMode = IntervalAutoMode.FixedCount;
            //chart.ChartAreas["Default"].AxisX.IntervalOffset = 2;

            //chart.ChartAreas["Default"].AxisX.MajorTickMark.Interval = 1.0;
            //chart.ChartAreas["Default"].AxisX.IntervalOffset = 0;

            chart.ChartAreas["Default"].AxisX.LineDashStyle = ChartDashStyle.Dot;

            chart.ChartAreas["Default"].AxisX.Minimum = -0.5 * cmp.CmpWindow	/** timeStep / 2*/;
            chart.ChartAreas["Default"].AxisX.Maximum = 0.5 * cmp.CmpWindow		/** timeStep / 2*/;
            chart.ChartAreas["Default"].AxisX.Interval = cmp.CmpWindow			/** timeStep*/ / 20; //0.5;

            //chart.ChartAreas["Default"].AxisX.

            /* 
            for (int i = 0; i < cmp2.Pints.Length; i++)
            {
                 chart.Series["Series1"].Points.Add(cmp2.Pints[i]);
            }
            */

            double start = -0.5 * cmp.CmpWindow;
            double delta = cmp.CmpWindow / (cmp.Points.Length - 1);

            for (int i = 0; i < cmp.Points.Length; i++)
            {
                chart.Series["Series1"].Points.AddXY(start + i * delta, cmp.Points[i]);
                //chart.Series["Series1"].Points.AddXY(cmp.Points[i], start + i * delta);

                //chart.Series["Series1"].Points.Add(cmp.Points[i]);
            }

            chart.ChartAreas["Default"].AxisX.RoundAxisValues();

            //////////////////////////////////////////////////////////////////
            Series errSeries = new Series("Error");
            errSeries.ChartArea = "Default";
            errSeries.ChartType = SeriesChartType.Spline;
            errSeries.Enabled = true;
            //errSeries.LegendText = "AAA";

            for (int i = 0; i < cmp.Points.Length; i += 32)
            {
                errSeries.Points.AddXY(start + i * delta, cmp.Points[i]);
            }

            /*
             for (int i = 1; i < nudN.Value / 2; i += 32)
             {
                 errSeries.Points.AddXY(start + i * delta, cmp.Points[i]);
             }

             errSeries.Points.AddXY(start + ((int)nudN.Value / 2) * delta, cmp.Points[512]);

             for (int i = ((int)nudN.Value/2 - 1) +32; i < nudN.Value; i += 32)
             {
                 errSeries.Points.AddXY(start + i * delta, cmp.Points[i]);
             }
            */

            //     chart.Series.Add(errSeries);

            // 

            //    MessageBox.Show(cmp.tmp.ToString());

            double mu, sigma;
            mu = cmp.Mean();
            sigma = cmp.Variance();

            tbM.Text = (mu * delta).ToString("F3");
            tbSKO.Text = (sigma * delta).ToString("F3");

            if (cbShowWave.Checked)
            {
                ShowGaus(cmp.CmpNum, mu, sigma);
            }

            //	MessageBox.Show(String.Format("Max ix = {0} ({1})", cmp.MaxIx, cmp.MaxIx));

            chart.Titles["Title"].Text = String.Format("Совпадения каналов {0}-{1} (µ = {2} нс, σ = {3} нс)",
                nudChANumber.Value, nudChBNumber.Value, (mu * delta).ToString("F3"), (sigma * delta).ToString("F3")); //²
        }

        private void ShowGaus(long NNN, double mu, double sigma)
        {
            chart.Series.Add(new Series("Gaus"));
            chart.Series["Gaus"].ChartArea = "Default";
            chart.Series["Gaus"].ChartType = SeriesChartType.FastLine;

            double timeStep = 1000.0 * (double)nudTInt.Value / (double)UInt16.MaxValue;

            for (int i = -(int)nudN.Value / 2; i < (int)nudN.Value / 2; i++)
            {

                double x = (double)i;
                double p = (double)NNN * (1.0 / (sigma * Math.Sqrt(2.0 * Math.PI))) * Math.Exp(-(Math.Pow(x - mu, 2)) / (2.0 * Math.Pow(sigma, 2)));

                chart.Series["Gaus"].Points.AddXY(x * timeStep, p);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {

            Geverate(); return;

            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = "Изображение (*.png)|*.png";
            dlg.RestoreDirectory = true;
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                chart.SaveImage(dlg.FileName, ChartImageFormat.Png);
            }
        }

        private void nudTInt_ValueChanged(object sender, EventArgs e)
        {
            double timeStep = 1000.0 * (double)nudTInt.Value / (double)UInt16.MaxValue;
            tbTts.Text = timeStep.ToString();
        }

        private void Geverate()
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = "Файл данных (*.dat3)|*.dat3";
            dlg.RestoreDirectory = true;
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                FileStream fs = new FileStream(dlg.FileName, FileMode.OpenOrCreate);

                int num = 100000;
                PETEvent[] events = EventCoincidenceGen.Generate(num, 0, 2, 0.2, 0.1);

                int size = Marshal.SizeOf(events[0]);
                byte[] arr = new byte[size];

                for (int i = 0; i < num; i++)
                {
                    IntPtr ptr = Marshal.AllocHGlobal(size);
                    Marshal.StructureToPtr(events[i], ptr, true);
                    Marshal.Copy(ptr, arr, 0, size);
                    Marshal.FreeHGlobal(ptr);

                    fs.Write(arr, 0, size);
                }
                fs.Close();
            }
        }
    }
}