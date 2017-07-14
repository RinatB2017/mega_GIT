using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace PETAnalyzer
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    internal struct PETEvent
    {
        /*
        public PETEvent()
        {
            Source = 0;
            Flags = 0;
            Reserve = 0;
            TS = 0;
            XP = 0;
            XN = 0;
            YP = 0;
            YN = 0;
        }
        */

        public Byte Source;         /* Источник события */
        public Byte Flags;          /* Информационное поле (флаги) события */
        public UInt16 Reserve;       /* Резерв для дальнейшего использования */
        public UInt32 TS;            /* Метка времени */
        public UInt16 XP;            /* Безразмерное значение координатного сигнала X+ */
        public UInt16 XN;            /* Безразмерное значение координатного сигнала X- */
        public UInt16 YP;            /* Безразмерное значение координатного сигнала Y+ */
        public UInt16 YN;            /* Безразмерное значение координатного сигнала Y- */
    }
}
