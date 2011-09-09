using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace EasylabSerialUDPGateway
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            //NICE TIP to get icon from an exe!

            //string filePath = @"C:\Users\UserEG\Documents\Visual Studio 2010\Projects\SerialUDPGateway\SerialUDPGateway\bin\Release\SerialUDPGateway.exe";
            //System.Drawing.Icon icon = System.Drawing.Icon.ExtractAssociatedIcon(filePath);
            //System.IO.FileStream fileStream = new System.IO.FileStream(@"C:\Users\UserEG\Documents\Visual Studio 2010\Projects\SerialUDPGateway\SerialUDPGateway\resources\default.ico", System.IO.FileMode.CreateNew);
            //icon.Save(fileStream);         

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            
            Application.Run(new EasylabSerialUDPGatewayForm());
        }
    }
}
