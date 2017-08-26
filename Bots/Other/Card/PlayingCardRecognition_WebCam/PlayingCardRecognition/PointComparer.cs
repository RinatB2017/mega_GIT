using System;
using System.Collections.Generic;
using System.Drawing;

namespace PlayingCardRecognition
{
    /// <summary>
    /// Comparer class for sorting points
    /// </summary>
    public class PointComparer : IComparer<Point>
    {
        public int Compare(Point p1, Point p2)
        {
            return p2.Y.CompareTo(p1.Y);
        }
    }
}
