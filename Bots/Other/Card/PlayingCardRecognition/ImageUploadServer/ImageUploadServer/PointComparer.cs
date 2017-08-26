using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Web;

namespace ImageUploadServer
{
    public class PointComparer : IComparer<Point>
    {
        public int Compare(Point p1, Point p2)
        {
            return p2.Y.CompareTo(p1.Y);
        }
    }
}