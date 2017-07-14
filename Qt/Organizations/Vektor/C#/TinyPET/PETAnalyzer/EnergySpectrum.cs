using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PETAnalyzer
{
    public class EnergySpectrum
    {
        double mMaxEnergy;

        public double MaxEnergy { get { return mMaxEnergy; } }

        //long mPointCount;

        double[] mPoints;
        long mMaxValue;

        public double[] Points { get { return mPoints; } }

        double mStep;

        public double Step { get { return mStep; } }

        public EnergySpectrum(double maxEnergy, long points)
        {
            mMaxEnergy = maxEnergy;
            //mPointCount = points;
            mPoints = new double[points];
            mMaxValue = 0;

            mStep = maxEnergy / (points - 1);
        }

        public void AddFlash(double energy)
        {
            if (energy > mMaxEnergy) throw new Exception();
            if (++mPoints[(int)(energy / mStep)] > mMaxValue) mMaxValue++;
        }
    }
}
