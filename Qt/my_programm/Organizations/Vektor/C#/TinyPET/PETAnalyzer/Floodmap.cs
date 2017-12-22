using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PETAnalyzer
{
    public class Floodmap
    {
        int mSize = 512;

        public int Size
        {
            get { return mSize; }
        }

        protected UInt32 maxCount;
        protected UInt32[,] Map;

        public Floodmap(int size)
        {
            mSize = size;

            Map = new UInt32[size, size];
            maxCount = 0;
        }

        public void AddFlash(double x, double y)
        {
            double k = (Size-1) / 2;
            AddFlash((int)(k * (x + 1)), (int)(k * (y + 1)));
        }

        public void AddFlash(int x, int y)
        {
            if (x < 0 | x > Size-1) throw new ArgumentOutOfRangeException("x");
            if (y < 0 | y > Size-1) throw new ArgumentOutOfRangeException("y");

            if (++Map[x, y] > maxCount) maxCount++;
        }

        public UInt32 MaxCount
        {
            get
            {
                return maxCount;
            }
        }

        public UInt32 GetCount(int x, int y)
        {
            return Map[x, y];
        }

        public double GetValue(int x, int y)
        {
            return Map[x, y] / maxCount;
        }

        //public double
    }
}
