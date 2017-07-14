using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace PETAnalyzer
{
    public struct FloodmapPalettePoint
    {
        public double Offset;
        public byte R;
        public byte G;
        public byte B;

        public FloodmapPalettePoint(double offset, byte r, byte g, byte b)
        {
            Offset = offset;
            R = r;
            G = g;
            B = b;
        }
    }
    public abstract class FloodmapPalette
    {
        int mColorCount = 0;

        public abstract FloodmapPalettePoint[] Points { get; }

        public int ColorCount
        {
            get { return mColorCount; }
        }

        public FloodmapPalette(int colorCount)
        {
            mColorCount = colorCount;
        }

        public abstract int GetColor(int color);
    }

    public class FloodmapGrayscalePalette : FloodmapPalette
    {
        public override FloodmapPalettePoint[] Points
        {
            get { return mPoints; }
        }

        private FloodmapPalettePoint[] mPoints = new FloodmapPalettePoint[]
        {
            new FloodmapPalettePoint(0.0, 0, 0, 0),
            new FloodmapPalettePoint(1.0, 255, 255, 255),
        };

        private int[] colorTable;

        public FloodmapGrayscalePalette(int colorCount) : base(colorCount)
        {
            colorTable = new int[colorCount];

            for (int i = 0; i < colorCount; i++)
            {
                byte rgb = (byte)(255 * i / ColorCount);
                colorTable[i] = (rgb << 16) |    // R
                    (rgb << 8) |    // G
                    (rgb << 0);     // B
            }
        }

        public override int GetColor(int color)
        {
            return colorTable[color];
        }
    }

    public class FloodmapGradientPalette : FloodmapPalette
    {
        public override FloodmapPalettePoint[] Points
        {
            get { return mPoints; }
        }

        static double k = 0.5;

        private FloodmapPalettePoint[] mPoints = new FloodmapPalettePoint[]
        {
            new FloodmapPalettePoint(0.0, 0, 0, 0),
            new FloodmapPalettePoint(0.2 * k, 85, 0, 170),
            new FloodmapPalettePoint(0.4 * k, 170, 0, 170),
            new FloodmapPalettePoint(0.6 * k, 255, 85, 0),
            new FloodmapPalettePoint(0.8 * k, 255, 170, 0),
            new FloodmapPalettePoint(1.0, 255, 255, 255),
        };

        private int[] colorTable;

        public FloodmapGradientPalette(int colorCount) : base(colorCount)
        {
            colorTable = new int[colorCount];

            for (int i = 0; i < colorCount; i++)
            {
                double colorOffset = (double)i / (double)colorCount;

                int j;
                for (j = 0; j < Points.Length - 1; j++)
                {
                    if (colorOffset >= Points[j].Offset && colorOffset <= Points[j + 1].Offset)
                    {
                        break;
                    }
                }

                byte r = (byte)((Points[j + 1].R - Points[j].R) * (colorOffset - Points[j].Offset) + Points[j].R);
                byte g = (byte)((Points[j + 1].G - Points[j].G) * (colorOffset - Points[j].Offset) + Points[j].G);
                byte b = (byte)((Points[j + 1].B - Points[j].B) * (colorOffset - Points[j].Offset) + Points[j].B);

                colorTable[i] = (r << 16) |    // R
                    (g << 8) |    // G
                    (b << 0);     // B
            }
        }

        public override int GetColor(int color)
        {
            return colorTable[color];
        }
    }
}
