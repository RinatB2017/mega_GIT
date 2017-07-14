using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace readsingles
{
    struct pet_event
    {
        public byte src;           /* Источник события */
        public byte flags;         /* Информационное поле (флаги) события */
        public UInt16 ts_fract;      /* Метка времени - дробная часть */
        public UInt32 ts;            /* Метка времени - целая часть */
        public UInt16 xp;            /* Безразмерное значение координатного сигнала X+ */
        public UInt16 xn;            /* Безразмерное значение координатного сигнала X- */
        public UInt16 yp;            /* Безразмерное значение координатного сигнала Y+ */
        public UInt16 yn;            /* Безразмерное значение координатного сигнала Y- */
        public int Energy { get { return xp + xn + yp + yn; } }

        public override string ToString()
        {
            return string.Format("{0} {1} {2} {3} {4}", src, flags, ts_fract, ts, (int)xp + xn + yp + yn );
        }
    }

    class Program
    {
        public static int Nx = 15; // 15 crystal rows
        public static int Ny = 15; // 15 crystals in a row
        public static int Nsrc = 2;

        static void Main(string[] args)
        {
            var flagCnt = new int[256];
            var sz = 16;
            var enbins = 256;
            var histenc = new int[enbins, Nsrc];
            var histfracc = new int[256, 2, Nsrc];
            var histtimec = new int[256, 4, Nsrc];
            var eff = new int[Nx, Ny, Nsrc];

            //uncomment to dump to specified file
            //Console.SetOut(File.CreateText("out.csv"));

            //open file passed in program arguments
            using (var f = File.OpenRead(args[0]))
            {
                var len = f.Length;
                var pos = 0;
                using (var r = new BinaryReader(f))
                {
                    while (pos < len)
                    {
                        var evt = new pet_event();
                        evt.src = r.ReadByte();
                        evt.flags = r.ReadByte();
                        evt.ts_fract = r.ReadUInt16();
                        evt.ts = r.ReadUInt32();
                        evt.xp = r.ReadUInt16();
                        evt.xn = r.ReadUInt16();
                        evt.yp = r.ReadUInt16(); 
                        evt.yn = r.ReadUInt16();
                        
                        flagCnt[evt.flags]++;

                        //use only "clean" events
                        if (evt.flags == 0)
                        {
                            histenc[evt.Energy * enbins / (65536 * 4), evt.src]++;

                            histfracc[evt.ts_fract & 0xFF, 0, evt.src]++;
                            histfracc[(evt.ts_fract >> 8) & 0xFF, 1, evt.src]++;
                            
                            histtimec[(evt.ts) & 0xFF, 0, evt.src]++;
                            histtimec[(evt.ts >> 8) & 0xFF, 1, evt.src]++;
                            histtimec[(evt.ts >> 16) & 0xFF, 2, evt.src]++;
                            histtimec[(evt.ts >> 24) & 0xFF, 3, evt.src]++;

                            var i = iForX(evt.xp, evt.xn);
                            var j = jForY(evt.yp, evt.yn);
                            eff[i, j, evt.src]++;

                        }
                        //Console.WriteLine("{0}", evt);
                        pos += sz;
                    }
                }
            }
            Console.WriteLine("Flags distribution");
            for (int i = 0; i < 256; i++)
                if (flagCnt[i] > 0) Console.WriteLine("{0};{1}", i, flagCnt[i]);

            Console.WriteLine("Energy distribution");
            for (int i = 0; i < enbins; i++)
                Console.WriteLine("{0};{1};{2}", i, histenc[i, 0], histenc[i, 1]);


            for (int b = 0; b < 2; b++)
            {
                Console.WriteLine("Time (fractional) distribution (byte#{0})", b);
                for (int i = 0; i < 256; i++)
                    Console.WriteLine("{0};{1};{2}", i, histfracc[i, b, 0], histfracc[i, b, 1]);
            }

            for (int b = 0; b < 4; b++)
            {
                Console.WriteLine("Time distribution (byte#{0})", b);
                for (int i = 0; i < 256; i++)
                    Console.WriteLine("{0};{1};{2}", i, histtimec[i, b, 0], histtimec[i, b, 1]);
            }


            for (int s = 0; s < 2; s++)
            {
                Console.WriteLine("Spatial distribution (src#{0})", s);
                for (int i = 0; i < eff.GetLength(0); i++)
                {
                    for (int j = 0; j < eff.GetLength(0); j++)
                        Console.Write("{0};", eff[i, j, s]);
                    Console.WriteLine();
                }
            }

            Console.Out.Flush();
        }

        // Recalculate (X+, X-) to i
        public static UInt16 iForX(UInt16 XPlus, UInt16 XMinus)
        {
            // X in [-1, +1]
            var X = (float)(XPlus - XMinus) / (float)(XPlus + XMinus);

            // i in [0 .. Ny - 1]
            var i = Math.Round((float)(Nx - 1) * (X + 1.0f) / 2.0f);

            return (UInt16)i;
        }

        // Recalculate (Y+, Y-) to j
        public static UInt16 jForY(UInt16 YPlus, UInt16 YMinus)
        {
            // Y in [-1, +1]
            var Y = (float)(YPlus - YMinus) / (float)(YPlus + YMinus);

            // j in [0 .. Ny - 1]
            var j = Math.Round((float)(Ny - 1) * (Y + 1.0f) / 2.0f);

            return (UInt16)j;
        }
    }
}
