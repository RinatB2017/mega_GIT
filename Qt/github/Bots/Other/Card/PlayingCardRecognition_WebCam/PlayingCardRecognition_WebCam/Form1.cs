using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.Windows.Forms;

using AForge.Video;
using AForge.Video.DirectShow;
using AForge.Imaging.Filters;
using HeatonResearchNeural.Feedforward;
using PlayingCardRecognition;

namespace PlayingCardRecognition_WebCam
{
    public partial class Form1 : Form
    {
        private const int CameraWidth = 800;  // constant Width
        private const int CameraHeight = 600; // constant Height

        private FilterInfoCollection cameras; //Collection of Cameras that connected to PC
        private VideoCaptureDevice device; //Current chosen device(camera) 
        private Dictionary<string, string> cameraDict = new Dictionary<string, string>();
        private Pen pen = new Pen(Brushes.Orange, 4); //is used for drawing rectangle around card
        public Font font = new Font("Tahoma", 15, FontStyle.Bold); //is used for writing string on card
        private CardRecognizer recognizer = new CardRecognizer();
        private CardCollection cards;

        private int frameCounter = 0;

        public Form1()
        {
            InitializeComponent();

            //Fetch cameras 
            this.cameras = new FilterInfoCollection(AForge.Video.DirectShow.FilterCategory.VideoInputDevice);
            int i = 1;
            foreach (AForge.Video.DirectShow.FilterInfo camera in this.cameras)
            {
                if (!this.cameraDict.ContainsKey(camera.Name))
                    this.cameraDict.Add(camera.Name, camera.MonikerString);
                else
                {
                    this.cameraDict.Add(camera.Name + "-" + i.ToString(), camera.MonikerString);
                    i++;
                }
            }
            this.cbCamera.DataSource = new List<string>(cameraDict.Keys); //Bind camera names to combobox

            if (this.cbCamera.Items.Count == 0)
                button1.Enabled = false;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (button1.Text == "Start")
            {
                this.button1.Text = "Stop";
                this.device = new VideoCaptureDevice(this.cameraDict[cbCamera.SelectedItem.ToString()]);
                this.device.NewFrame += videoNewFrame;
                this.device.DesiredFrameSize = new Size(CameraWidth, CameraHeight);

                device.Start(); //Start Device
            }
            else
            {
                this.StopCamera();
                button1.Text = "Start";
                this.pictureBox1.Image = null;
            }
        }
        private Bitmap ResizeBitmap(Bitmap bmp)
        {
            ResizeBilinear resizer = new ResizeBilinear(pictureBox1.Width, pictureBox1.Height);

            return resizer.Apply(bmp);
        }

        private void videoNewFrame(object sender, NewFrameEventArgs args)
        {

            Bitmap temp = args.Frame.Clone() as Bitmap;

            try
            {
                frameCounter++;

                if (frameCounter > 10)
                {
                    cards = recognizer.Recognize(temp);
                    frameCounter = 0;
                }

                //Draw Rectangle around cards and write card strings on card
                using (Graphics graph = Graphics.FromImage(temp))
                {
                    foreach (Card card in cards)
                    {
                        graph.DrawPolygon(pen, card.Corners); //Draw a polygon around card
                        PointF point = CardRecognizer.GetStringPoint(card.Corners); //Find Top left corner
                        point.Y += 10;
                        graph.DrawString(card.ToString(), font, Brushes.White, point); //Write string on card
                    }
                }
            }
            catch { }
            this.pictureBox1.Image = ResizeBitmap(temp);
        }
        private void StopCamera()
        {
            if (device != null && device.IsRunning)
            {
                device.SignalToStop(); //stop device
                device.WaitForStop();
                device = null;
            }
        }
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.StopCamera();
        }
    }
}
