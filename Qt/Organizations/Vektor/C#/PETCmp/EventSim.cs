using PET;
using PETCAT.Simulation;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PETCmp
{
    public class EventSim
    {
        Gauss g;
        Random r;
        byte src = 2;
        public EventSim(int m, double dev)
        {
            g = new Gauss(m, dev);
            r = new Random();
        }

        uint tsA = 10000;
        uint tsB = 10000;

        uint dd = 5000;

        public PETEvent Next()
        {
            dd = (uint)r.Next(5000);

            PETEvent e = new PETEvent();
            e.Source = src;
            if (src == 2)
            {
                //tsA = (uint)(tsB + (int)g.Next());
                tsA += 10000;
                e.Source = src;
                e.TS = tsA;
                src = 3;
            }
            else
            {
                tsB = (uint)((int)tsA + g.Next());
                // tsA += dd;
                e.Source = src;
                e.TS = tsB;
                src = 2;
            }
            return e;
        }
    }
}
