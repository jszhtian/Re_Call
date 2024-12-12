using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.ComponentModel;
using System.Threading;

namespace AinDecompiler
{
    public class MyBackgroundWorker : BackgroundWorker
    {
        Thread myThread = null;

        protected override void OnDoWork(DoWorkEventArgs e)
        {
            myThread = Thread.CurrentThread;
            try
            {
                base.OnDoWork(e);
            }
            catch (ThreadAbortException ex)
            {
                e.Cancel = true;
                Thread.ResetAbort();
            }
        }

        public void Abort()
        {
            if (myThread != null)
            {
                myThread.Abort();
                myThread = null;
            }
        }
    }
}
