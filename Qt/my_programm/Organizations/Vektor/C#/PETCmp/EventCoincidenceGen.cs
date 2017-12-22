using PET;
using PETCAT.Simulation;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PETCmp
{
    public class EventCoincidenceGen
    {
        //public EventCoincidenceGen

        public static PETEvent[] Generate(int num, byte srcA, byte srcB, double mean, double variance)
        {
            PETEvent[] events = new PETEvent[num];

            double ts = 0;

            Gauss g = new Gauss(mean, variance);
            //Random r = new Random();

            for (int i = 0; i < num; )
            {
                /**/
                ts = (i + 1) * 1000.0;
                events[i].Source = srcA;
                events[i].Flags = 0;
                events[i].TS = (UInt32)ts;
                events[i].TS_Fract = (UInt16)((double)UInt16.MaxValue * (double)(ts - (UInt32)ts));
                events[i].XP = 0;
                events[i].XN = 0;
                events[i].YP = 0;
                events[i].YN = 0;
                i++;

                /**/
                ts += g.Next()/* + 0.75*/;
                events[i].Source = srcB;
                events[i].Flags = 0;
                events[i].TS = (UInt32)ts;
                events[i].TS_Fract = (UInt16)((double)UInt16.MaxValue * (double)(ts - (UInt32)ts));
                events[i].XP = 0;
                events[i].XN = 0;
                events[i].YP = 0;
                events[i].YN = 0;
                i++;
            }
            return events;
        }
    }
}
