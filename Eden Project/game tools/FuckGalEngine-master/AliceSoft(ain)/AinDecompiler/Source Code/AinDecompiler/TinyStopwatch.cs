using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    public class TinyStopwatch
    {
        int lastTick;
        public void Start()
        {
            lastTick = Environment.TickCount;
        }
        public int ElapsedTime
        {
            get
            {
                unchecked
                {
                    int difference = Environment.TickCount - lastTick;
                    difference &= 0x3FFFFFFF;
                    return difference;
                }
            }
        }

    }
}
