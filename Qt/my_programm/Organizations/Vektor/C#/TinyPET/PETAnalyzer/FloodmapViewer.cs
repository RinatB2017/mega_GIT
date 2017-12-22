using System;
using System.ComponentModel;
using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace PETAnalyzer
{
    /// <summary>
    /// 
    /// </summary>
    public class FloodmapViewer : Control
    {
        public static DependencyProperty FloodmapProperty;
        static FloodmapViewer()
        {
            // Регистрация свойств зависимости
            FloodmapProperty = DependencyProperty.Register("Floodmap", typeof(Floodmap), typeof(FloodmapViewer),
                new FrameworkPropertyMetadata(null, new PropertyChangedCallback(OnFloodmapChanged)));

            //
            DefaultStyleKeyProperty.OverrideMetadata(typeof(FloodmapViewer), new FrameworkPropertyMetadata(typeof(FloodmapViewer)));
        }

        public Floodmap Floodmap
        {
            get { return (Floodmap)GetValue(FloodmapProperty); }
            set { SetValue(FloodmapProperty, value); }
        }

        private static void OnFloodmapChanged(DependencyObject sender, DependencyPropertyChangedEventArgs e)
        {
            FloodmapViewer floodmapViewer = (FloodmapViewer)sender;

            floodmapViewer.Floodmap = (Floodmap)e.NewValue;

            floodmapViewer.Draw();
            floodmapViewer.InvalidateVisual();

            /*
            Color color = colorPicker.Color;
            if (e.Property == RedProperty)
                color.R = (byte)e.NewValue;
            else if (e.Property == GreenProperty)
                color.G = (byte)e.NewValue;
            else if (e.Property == BlueProperty)
                color.B = (byte)e.NewValue;

            colorPicker.Color = color;
            */
        }

        //public Floodmap Floodmap
        //{
        //    get { return mFloodmap; }
        //    set
        //    {
        //        mFloodmap = value;
        //        Draw();
        //    }
        //}

        private DrawingVisual mapVisual;
        private DrawingVisual pixelVisual;
        private DrawingVisual barVisual;

        public FloodmapPalette Palette {
            get; set;
        }       

        Rect mapRect;
        Rect barRect;

        Pen gridPen = new Pen(Brushes.Black, 1.0);
        Thickness mapMargin = new Thickness(20);

        double mapSize = 10;

        double Min(double a, double b)
        {
            return (a < b) ? a : b;
        }

        protected void CalcSizes()
        {
            //double Width = ActualWidth == 0;

            mapSize = Min(ActualWidth - (mapMargin.Left + mapMargin.Right + 50), ActualHeight - (mapMargin.Top + mapMargin.Bottom)); 
            
            if (ActualWidth == 0 || ActualHeight == 0) return;

            mapRect = new Rect(new Point(mapMargin.Top, mapMargin.Left), new Size(mapSize, mapSize));
            barRect = new Rect(new Point(mapRect.Right + mapMargin.Right, mapRect.Top), new Size(20, mapSize));
        }

        protected override void OnRenderSizeChanged(SizeChangedInfo sizeInfo)
        {
            base.OnRenderSizeChanged(sizeInfo);

            CalcSizes();
        }

        Floodmap mFloodmap;

        protected override void OnInitialized(EventArgs e)
        {
            Palette = new FloodmapGrayscalePalette(0);

            base.OnInitialized(e);

            // RenderOptions.SetBitmapScalingMode(this, BitmapScalingMode.NearestNeighbor);
            RenderOptions.SetBitmapScalingMode(this, BitmapScalingMode.HighQuality);
            //RenderOptions.SetEdgeMode(this, EdgeMode.Aliased);
            //RenderOptions.SetEdgeMode(this, EdgeMode.);

            this.MouseLeftButtonDown += DoMouseLeftButtonDown;
            this.MouseMove += DoMouseMove;
            this.MouseWheel += DoMouseWheel;

        //    Draw();

            mapVisual = new DrawingVisual();
            pixelVisual = new DrawingVisual();
            barVisual = new DrawingVisual();
            CalcSizes();
            Paint();
            InvalidateVisual();
        }

        private void DoMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            //throw new NotImplementedException();
            //DrawPixel(e);
        }

        private void DoMouseMove(object sender, MouseEventArgs e)
        {
            //throw new NotImplementedException();
        }

        private void DoMouseWheel(object sender, MouseWheelEventArgs e)
        {
            //throw new NotImplementedException();
        }

        /// <summary>
        /// 
        /// </summary>
        private void Paint()
        {
            PaintMap();
            PaintPixel();
            PaintBar();
        }

        WriteableBitmap writeableBitmap;

        void Draw()
        {
            //	https://msdn.microsoft.com/ru-ru/library/system.windows.media.imaging.writeablebitmap(v=vs.110).aspx

            //	if (Palette == null) return;

           	// 	int floodmapSize = 512;

            writeableBitmap = new WriteableBitmap(
                Floodmap.Size,
                Floodmap.Size,
                96,
                96,
                PixelFormats.Bgr32,
                null);

           	// 	int column = 10;
          	//  int row = 10;

            // Reserve the back buffer for updates.
            writeableBitmap.Lock();

         	//	Random r = new Random();

            unsafe
            {
                for (int column = 0; column < Floodmap.Size; column++)
                    for (int row = 0; row < Floodmap.Size; row++)
                    {
                        // Get a pointer to the back buffer.
                        int pBackBuffer = (int)writeableBitmap.BackBuffer;

                        // Find the address of the pixel to draw.
                        pBackBuffer += row * writeableBitmap.BackBufferStride;
                        pBackBuffer += column * 4;

                      	//  int color_data = 255 << 16; // R
                      	//  color_data |= 128 << 8;   // G
                     	//   color_data |= 255 << 0;   // B

                        // Assign the color data to the pixel.
                        *((int*)pBackBuffer) = Palette.GetColor((int)Floodmap.GetCount(column, row));
                    }
            }

            // Specify the area of the bitmap that changed.
            writeableBitmap.AddDirtyRect(new Int32Rect(0, 0, Floodmap.Size, Floodmap.Size));

            // Release the back buffer and make it available for display.
            writeableBitmap.Unlock();
        }

        /// <summary>
        /// 
        /// </summary>
        private void PaintMap()
        {
            if (mapVisual == null) return;

            using (DrawingContext dc = mapVisual.RenderOpen())
            {
                dc.DrawRectangle(null, gridPen, mapRect);


                // Маркеры
                dc.DrawLine(gridPen, new Point(mapRect.Left, mapRect.Top - 5), new Point(mapRect.Left, mapRect.Top - 10));
                dc.DrawLine(gridPen, new Point(mapRect.Left + mapRect.Width / 2, mapRect.Top - 5), new Point(mapRect.Left + mapRect.Width / 2, mapRect.Top - 10));
                dc.DrawLine(gridPen, new Point(mapRect.Right, mapRect.Top - 5), new Point(mapRect.Right, mapRect.Top - 10));

                dc.DrawLine(gridPen, new Point(mapRect.Left, mapRect.Bottom + 5), new Point(mapRect.Left, mapRect.Bottom + 10));
                dc.DrawLine(gridPen, new Point(mapRect.Left + mapRect.Width / 2, mapRect.Bottom + 5), new Point(mapRect.Left + mapRect.Width / 2, mapRect.Bottom + 10));
                dc.DrawLine(gridPen, new Point(mapRect.Right, mapRect.Bottom + 5), new Point(mapRect.Right, mapRect.Bottom + 10));

                dc.DrawLine(gridPen, new Point(mapRect.Left - 10, mapRect.Top), new Point(mapRect.Left - 5, mapRect.Top));
                dc.DrawLine(gridPen, new Point(mapRect.Left - 10, mapRect.Top + mapRect.Height / 2), new Point(mapRect.Left - 5, mapRect.Top + mapRect.Height / 2));
                dc.DrawLine(gridPen, new Point(mapRect.Left - 10, mapRect.Bottom), new Point(mapRect.Left - 5, mapRect.Bottom));

                dc.DrawLine(gridPen, new Point(mapRect.Right + 5, mapRect.Top), new Point(mapRect.Right + 10, mapRect.Top));
                dc.DrawLine(gridPen, new Point(mapRect.Right + 5, mapRect.Top + mapRect.Height / 2), new Point(mapRect.Right + 10, mapRect.Top + mapRect.Height / 2));
                dc.DrawLine(gridPen, new Point(mapRect.Right + 5, mapRect.Bottom), new Point(mapRect.Right + 10, mapRect.Bottom));

                //	dc.DrawLine(gridPen, pt0, pt1);
                // 	dc.DrawText(new FormattedText((pointNum - i).ToString(), CultureInfo.CurrentCulture, FlowDirection.LeftToRight, new Typeface("Verdana"), TextHeight, Brushes.Black), pt3);

              	// 	BitmapImage bi = new BitmapImage();
                //	BitmapImage.UriSource must be in a BeginInit/EndInit block.
              	//  bi.BeginInit();
             	//  bi.UriSource = new Uri(@"tst.jpg", UriKind.RelativeOrAbsolute);

               	// bi.EndInit();

                dc.DrawImage(writeableBitmap, mapRect);
            }
        }

        public struct PixelMarker
        {
            public double X;
            public double Y;
            public double Radius;

            public PixelMarker(double x, double y, double radius)
            {
                X = x;
                Y = y;
                Radius = radius;
            }
        }

        public PixelMarker pm = new PixelMarker(0.2, -0.3, 0.05);

        private void PaintPixel()
        {
            if (pixelVisual == null || Floodmap == null) return;

            using (DrawingContext dc = pixelVisual.RenderOpen())
            {
                dc.PushClip(new RectangleGeometry(mapRect));
                dc.PushTransform(new TranslateTransform((mapRect.Left+ mapRect.Right) / 2.0, (mapRect.Top + mapRect.Bottom) / 2.0));
                dc.PushTransform(new ScaleTransform(mapSize / 2, mapSize / 2/*, (mapRect.Left + mapRect.Right) / 2.0, (mapRect.Top + mapRect.Bottom) / 2.0)*/));
                //ScaleTransform()

                Pen pixelPen = new Pen(Brushes.Red, 0.005);

                //   double radiusX = 10.0;
                //  double radiusY = 10.0;

                dc.DrawEllipse(null, pixelPen, new Point(pm.X, pm.Y), pm.Radius, pm.Radius);

                //dc.DrawRectangle(null, gridPen, mapRect);

                // Маркеры
                //dc.DrawLine(gridPen, new Point(mapRect.Left, mapRect.Top - 5), new Point(mapRect.Left, mapRect.Top - 10));
            }
        }

        /// <summary>
        /// Отрисовка области товой шкалы
        /// </summary>
        private void PaintBar()
        {
            if (barVisual == null) return;
            using (DrawingContext dc = barVisual.RenderOpen())
            {
                // Rectangle barRectangle = new Rectangle();
                // barRectangle.Width = 50;
                // barRectangle.Height = 200;

                // Rect barRect = new Rect(new Point(0, 0), new Size(20, 0.8 * ActualHeight));
                
                LinearGradientBrush brush = new LinearGradientBrush();
                brush.StartPoint = new Point(0, 1);
                brush.EndPoint = new Point(0, 0);

                foreach (FloodmapPalettePoint p in Palette.Points)
                {
                    brush.GradientStops.Add(new GradientStop(Color.FromRgb(p.R, p.G, p.B), p.Offset));
                }

                /*
                brush.GradientStops.Add(new GradientStop(Color.FromRgb(0, 0, 0), 0.0));
                brush.GradientStops.Add(new GradientStop(Color.FromRgb(85, 0, 170), 0.2));
                brush.GradientStops.Add(new GradientStop(Color.FromRgb(170, 0, 170), 0.4));
                brush.GradientStops.Add(new GradientStop(Color.FromRgb(255, 85, 0), 0.6));
                brush.GradientStops.Add(new GradientStop(Color.FromRgb(255, 170, 0), 0.8));
                brush.GradientStops.Add(new GradientStop(Color.FromRgb(255, 255, 255), 1.0));
                */

                dc.DrawRectangle(brush, gridPen, barRect);

                double TextHeight = 10;

                Point pt0 = new Point(barRect.Right, barRect.Top);
                Point pt1 = new Point(barRect.Right + 5, barRect.Top);

                Point pt3 = new Point(barRect.Right + 10, barRect.Top - TextHeight / 2);

                int pointNum = (int)(0.2 * mapSize / TextHeight);
                double offset = mapSize / pointNum;

                for (int i = 0; i <= pointNum; i++)
                {
                    dc.DrawLine(gridPen, pt0, pt1);

                    if (Floodmap != null)
                    {
                        dc.DrawText(new FormattedText((Floodmap.MaxCount - i * Floodmap.MaxCount / pointNum).ToString(), CultureInfo.CurrentCulture, FlowDirection.LeftToRight, new Typeface("Verdana"), TextHeight, Brushes.Black), pt3);
                    }

                    //dc.DrawText(new FormattedText((pointNum - i).ToString(), CultureInfo.CurrentCulture, FlowDirection.LeftToRight, new Typeface("Verdana"), TextHeight, Brushes.Black), pt3);

                    pt0.Offset(0, offset);
                    pt1.Offset(0, offset);
                    pt3.Offset(0, offset);
                }
            }
        }

        /// <summary>
        /// 
        /// </summary>
        protected override void OnRender(DrawingContext drawingContext)
        {
            base.OnRender(drawingContext);
            if (DesignerProperties.GetIsInDesignMode(this))
            {
                // режим дизайнера
                // _dpkValue = 0xFFa42312;
                //Paint();
            }

            CalcSizes();
            Paint();

            drawingContext.DrawDrawing(mapVisual.Drawing);
            drawingContext.DrawDrawing(pixelVisual.Drawing);
            drawingContext.DrawDrawing(barVisual.Drawing);
        }
    }
}
