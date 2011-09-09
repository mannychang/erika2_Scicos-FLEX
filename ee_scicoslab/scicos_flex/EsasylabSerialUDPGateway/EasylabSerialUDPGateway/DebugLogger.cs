using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace EasylabSerialUDPGateway
{
    public class DebugLogger
    {
        public static void setFilePath(string filePath_) {
            filePath = filePath_;
        }

        public static void LogPackets(List<float> receivedFloats)
        {
            using(FileStream writeFile = new FileStream(filePath, FileMode.Append))
            {
                byte[] rows = prepareRows(receivedFloats);
                writeFile.Write(rows, 0, rows.Length);
            }
        }

        private static byte[] prepareRows(List<float> receivedFloats)
        {
            StringBuilder rowsBuilder = new StringBuilder(receivedFloats.Count * 50);
            for (int i = 0; ; i += 2)
            {
                if (i < receivedFloats.Count - 1)
                {
                    rowsBuilder.AppendLine("Packet Nr: " + ++staticCounter + " " + receivedFloats[i] + " " + receivedFloats[i + 1] + " Time:" + DateTime.Now);
                }
                else if (i == receivedFloats.Count - 1)
                {
                    rowsBuilder.AppendLine("Packet N°:" + ++staticCounter + " " + receivedFloats[i] + "Time:" + DateTime.Now);
                    break;
                }
                else
                {
                    break;
                }
            }
            
            return ASCIIEncoding.ASCII.GetBytes(rowsBuilder.ToString());
        }
        
        private static int staticCounter;
        private static string filePath = @".\PacketsLog.txt";
    }
}
