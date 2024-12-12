using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;
using System.Globalization;

namespace TranslateParserThingy
{
    class BackgroundTranslation : IDisposable
    {
        /// <summary>
        /// A message for the exception triggered when a user cancels translation.
        /// </summary>
        const string UserCancelledMessage = "User Cancelled";

        /// <summary>
        /// The progress form, shows a progress bar which is updated as translation progresses.
        /// </summary>
        TranslationProgressDialogBox form = new TranslationProgressDialogBox();

        /// <summary>
        /// The background worker which executes the translation progress
        /// </summary>
        BackgroundWorker bw = new BackgroundWorker();

        /// <summary>
        /// The result text of translation.
        /// </summary>
        string result = null;

        #region IDisposable Members


        /// <summary>
        /// Destroys the form, and stops translation.  Called when the Stop button is clicked.
        /// </summary>
        public void Dispose()
        {
            form.Dispose();
            if (bw.IsBusy && !bw.CancellationPending)
            {
                bw.CancelAsync();
            }
        }

        #endregion

        /// <summary>
        /// Translates text in the background through use of BackgroundWorkers.  Blocks until translation is complete.  Shows a progress dialog box which can be cancelled.
        /// </summary>
        /// <param name="text">The text to be translated</param>
        /// <returns>The translated text, or the original text if translation was cancelled.</returns>
        public string TranslateText(string text)
        {
            bw.WorkerSupportsCancellation = true;
            bw.WorkerReportsProgress = true;

            bw.DoWork += new DoWorkEventHandler(bw_DoWork);
            bw.ProgressChanged += new ProgressChangedEventHandler(bw_ProgressChanged);
            bw.RunWorkerCompleted += new RunWorkerCompletedEventHandler(bw_RunWorkerCompleted);

            form.StopButtonClicked += new EventHandler(form_StopButtonClicked);

            try
            {
                form.LabelText = "Translating...";
                //var oldPriority = Thread.CurrentThread.Priority;
                //higher priority so it can redraw quicker
                //Thread.CurrentThread.Priority = ThreadPriority.AboveNormal;
                bw.RunWorkerAsync(text);
                form.ShowDialog();
                //Thread.CurrentThread.Priority = oldPriority;
                Application.DoEvents();
            }
            catch (Exception ex)
            {

            }
            form.Dispose();

            if (result == null)
            {
                result = text;
            }

            return result;
        }

        /// <summary>
        /// Handles the Clicked event of the Stop Button to stop translation.
        /// </summary>
        /// <param name="sender">The form</param>
        /// <param name="e">Empty event args</param>
        void form_StopButtonClicked(object sender, EventArgs e)
        {
            this.Dispose();
        }

        /// <summary>
        /// Handles the RunWorkerCompleted event of the background worker to close the progress form, and accept the translation result.
        /// </summary>
        /// <param name="sender">The background worker</param>
        /// <param name="e">The EventArgs containing the result.</param>
        void bw_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (!e.Cancelled)
            {
                result = (string)e.Result;
                if (!form.IsDisposed)
                {
                    form.Close();
                }
            }
        }

        /// <summary>
        /// Handles the ProgressChanged event of the background worker to display new progress information in the progress form.
        /// </summary>
        /// <param name="sender">The background worker</param>
        /// <param name="e">The EventArgs containing the current progress (as a percentage, and a string)</param>
        void bw_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            if (!form.IsDisposed)
            {
                form.Progress = e.ProgressPercentage;
                form.LabelText = (string)e.UserState;
            }
        }

        /// <summary>
        /// Handles the DoWork event of the background worker to start translation.
        /// </summary>
        /// <param name="sender">The background worker</param>
        /// <param name="e">The eventArgs containing the text to be translated.</param>
        void bw_DoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                var text = (string)e.Argument;
                var lines = text.Split(new[] { "\r\n" }, StringSplitOptions.None);
                var translatedLines = Translator.TranslateLines(lines, ReportProgressFunction);
                e.Result = translatedLines.Join("\r\n");
            }
            catch (ApplicationException ex)
            {
                if (ex.Message == UserCancelledMessage)
                {
                    e.Result = null;
                    e.Cancel = true;
                }
                else
                {
                    throw;
                }
            }
        }

        /// <summary>
        /// A function called when progress has changed, raises the ReportProgress event in the background worker.
        /// Called from the secondary thread.
        /// </summary>
        /// <param name="lineNumber">Which line number we're at</param>
        /// <param name="maxLineNumber">Total number of lines</param>
        void ReportProgressFunction(int lineNumber, int maxLineNumber)
        {
            if (bw.CancellationPending)
            {
                throw new ApplicationException(UserCancelledMessage);
            }
            else
            {
                int percent = 100 * lineNumber / maxLineNumber;
                string message = "Translating " + lineNumber.ToString(CultureInfo.InvariantCulture) + " of " + maxLineNumber.ToString(CultureInfo.InvariantCulture);
                bw.ReportProgress(percent, message);
            }
        }
    }
}
