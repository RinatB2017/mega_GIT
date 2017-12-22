using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PETCAT.Simulation
{
    /// <summary>
    ///  метод полярных координат, предложенный Джорджем Боксом, Мервином Мюллером и Джорджем Марсальей в 1958 году
    /// </summary>
    public class Gauss
    {
        double Mean = 0;
        double Dev = 1.0;

        double second = 0.0;

        bool ready = false;

        Random rnd;

        public Gauss(double mean, double dev)
        {
            Mean = mean;
            Dev = dev;
            rnd = new Random();
        }


        public double Next()
        {
            if (this.ready)
            {
                this.ready = false;
                return this.second * Dev + Mean;
            }
            else
            {
                double u, v, s;
                do
                {
                    u = 2.0 * rnd.NextDouble() - 1.0;
                    v = 2.0 * rnd.NextDouble() - 1.0;
                    s = u * u + v * v;
                } while (s > 1.0 || s == 0.0);

                double r = Math.Sqrt(-2.0 * Math.Log(s) / s);
               
                
                this.second = r * u;
                this.ready = true;
                return r * v * Dev + Mean;
            }
        }
    }
}
