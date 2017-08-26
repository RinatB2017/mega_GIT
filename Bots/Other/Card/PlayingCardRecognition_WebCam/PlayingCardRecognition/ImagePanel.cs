using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

using AForge.Imaging.Filters;

namespace PlayingCardRecognition
{
    /// <summary>
    /// User control for displaying images side by side on a panel
    /// </summary>
    public partial class ImagePanel : UserControl
    {
        private List<Bitmap> imageList = null; //Bitmaps to be drawn on panel
        public int resizeW = 80, resizeH = 120, gap = 90; //Size values

        /// <summary>
        /// Gap between images on panel
        /// </summary>
        [Browsable(true)]
        public int Gap
        {
            get { return this.gap; }
            set { this.gap = value; }
        }
        /// <summary>
        /// Image Width on panel
        /// </summary>
        [Browsable(true)]
        public int ResizeWidth
        {
            get { return this.resizeW; }
            set { this.resizeW = value; }
        }
        /// <summary>
        /// Image Height on panel
        /// </summary>
        [Browsable(true)]
        public int ResizeHeight
        {
            get { return this.resizeH; }
            set { this.resizeH = value; }
        }

        public ImagePanel()
        {
            InitializeComponent();
        }
        /// <summary>
        /// Function that resizes image
        /// </summary>
        /// <param name="bmp">Image to be resized</param>
        /// <returns>Resized image</returns>
        private Bitmap ResizeBitmap(Bitmap bmp)
        {
            ResizeBilinear resizer = new ResizeBilinear(resizeW, resizeH);

            return resizer.Apply(bmp);
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            if (imageList == null)
                base.OnPaint(e);
            else
            {
                using (Graphics g = e.Graphics)  //Draw Images on panel side by side
                {
                    Point p = new Point(5, 5);

                    for (int i = 0; i < imageList.Count; i++)
                    {
                        g.DrawImage(ResizeBitmap(imageList[i]), p);
                        p.X += gap;
                    }
                }
            }
        }
        public void DrawImages(List<Bitmap> imageList)
        {
            this.imageList = imageList; //Set image list
            this.Refresh(); //Refresh so that OnPaint event will be called
        }
    }
}
