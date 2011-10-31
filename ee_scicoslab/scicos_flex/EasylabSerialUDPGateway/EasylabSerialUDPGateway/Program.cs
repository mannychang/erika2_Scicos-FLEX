using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace EasylabSerialUDPGateway
{
    static class Program
    {
        /*
         * Imported win32 API to Bring prior process window on top
         */
        [DllImport("user32.dll")]
        public static extern uint SetForegroundWindow(IntPtr handle);


        public static Process PriorProcess()
        // Returns a System.Diagnostics.Process pointing to
        // a pre-existing process with the same name as the
        // current one, if any; or null if the current process
        // is unique.
        {
            Process curr = Process.GetCurrentProcess();
            Process[] procs = Process.GetProcessesByName(curr.ProcessName);
            foreach (Process p in procs)
            {
                if (p.Id != curr.Id)
                    return p;
            }
            return null;
        }

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            //NICE TIP to get icon from an exe!

            //string filePath = @"C:\Users\UserEG\Documents\Visual Studio 2010\Projects\SerialUDPGateway\SerialUDPGateway\bin\Release\SerialUDPGateway.exe";
            //System.Drawing.Icon icon = System.Drawing.Icon.ExtractAssociatedIcon(filePath);
            //System.IO.FileStream fileStream = new System.IO.FileStream(@"C:\Users\UserEG\Documents\Visual Studio 2010\Projects\SerialUDPGateway\SerialUDPGateway\resources\default.ico", System.IO.FileMode.CreateNew);
            //icon.Save(fileStream);         
            string commPort="COM3";
            decimal receivingPort= 50002;
            decimal sendingPort  = 50001;
            if(args.Length > 0)
            {
                foreach (string s in args)
                {
                    string[] option = s.Split('=');
                    if (option.Length == 2)
                    {
                        switch (option[0])
                        {
                            case "--COM":
                                commPort = "COM" + option[1];
                            break;
                            case "--receiving-port":
                                receivingPort = decimal.Parse(option[1]);
                            break;
                            case "--sending-port":
                            sendingPort = decimal.Parse(option[1]);
                            break;
                            
                            default:
                            return;
                        }
                        continue;
                    }
                    return;
                }
            }

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            
            Process prior = PriorProcess();            
            EasylabSerialUDPGatewayForm easylabSerialUDPGatewayForm;

            if(args.Length > 0)
            {
                if (prior != null)        
                    prior.CloseMainWindow();
                easylabSerialUDPGatewayForm = new EasylabSerialUDPGatewayForm(commPort, receivingPort, sendingPort);
                Application.Run(easylabSerialUDPGatewayForm);
            }
            else if (prior == null)
            {
                easylabSerialUDPGatewayForm = new EasylabSerialUDPGatewayForm();
                Application.Run(easylabSerialUDPGatewayForm);
            }
            else 
            {
                SetForegroundWindow(prior.MainWindowHandle);
            }
        }
    }
}
