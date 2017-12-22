using PET;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PETCmp
{
    /// <summary>
    /// 
    /// </summary>
    public class PETEventCmp
    {
        long[] mPoints;
        public long mMax = 0;

        public long[] Points { get { return mPoints; } }

        double mCmpWindow;
        double mTimeScale;

        public double CmpWindow
        {
            get { return mCmpWindow; }
        }

        double mStep;
        uint mIntNum;

        public Int32 CorrectionA = 0;
        public Int32 CorrectionB = 0;

        public PETEventCmp(double cmpWindow, double timeScale, uint intNum, byte sourceA, byte sourceB)
        {
            mPoints = new long[intNum];
            mSourceA = sourceA;
            mSourceB = sourceB;
            mCmpWindow = cmpWindow;
            mTimeScale = timeScale;

            mStep = (double)cmpWindow / ((double)intNum - 1.0);
            mIntNum = intNum;
        }

        byte mSourceA, mSourceB;

        public long EventTotal = 0;
        public long CmpNum = 0;

        public long tmp = 0;

        double tsA = 0;
        double tsB = 0;

        public uint MaxIx = 0;

        public void ProcEvent(PETEvent petEvent)
        {
            double ts = mTimeScale * ((double)petEvent.TS + (double)petEvent.TS_Fract / (double)UInt16.MaxValue);

            if (petEvent.Source == mSourceA)
            {
                tsA = ts;
            }
            else if (petEvent.Source == mSourceB)
            {
                tsB = ts;
            }
            else return;

            int ix = (int)(((mPoints.Length - 1.0) / 2.0) + (tsB - tsA) / mStep);

            if (ix >= 0 && ix < mPoints.Length)
            {
                if (++mPoints[ix] > mMax)
                {
                    mMax++;
                    MaxIx = (uint)ix;
                }
                CmpNum++;
            }

            EventTotal++;
        }

        public double Mean()
        {
            double mean = 0;
            //double p;
            //double timeStep = 1.0;
            //double delta = CmpWindow * timeStep / (Points.Length - 1);
            //double start = -1.0 * CmpWindow * timeStep / 2;

            for (int i = 0; i < mIntNum; i++)
            {
                mean += (double)(i - (mPoints.Length - 1) / 2) * (double)mPoints[i] / (double)CmpNum;
				/*
                if(mean > 0)
                {
                    p = mean;
                }
                */
            }
            return mean;
        }

        public double Variance()
        {
            double mean2 = 0;

            for (int i = 0; i < mPoints.Length; i++)
            {
                mean2 += Math.Pow((double)(i - (mPoints.Length - 1) / 2), 2) * (double)mPoints[i] / (double)CmpNum;
            }
            return Math.Sqrt(mean2 - Math.Pow(Mean(), 2));
        }
    }
}
