using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Threading;
namespace EasylabSerialUDPGateway
{
    public class DebugLogger
    {

        private class Container {
            public Container(List<float> receivedFloats, byte[] commBuffer, byte[] prevRemainig, byte[] actualRemaning, List<byte[]> packetsSent)
            {
                // TODO: Complete member initialization
                this.receivedFloats = receivedFloats;
                this.commBuffer = commBuffer;
                this.prevRemainig = prevRemainig;
                this.actualRemaning = actualRemaning;
                this.packetsSent = packetsSent;
                this.now = DateTime.Now;
            }

            internal readonly List<float> receivedFloats;
            internal readonly byte[] commBuffer;
            internal readonly List<byte[]> packetsSent;
            internal readonly DateTime now;
            internal readonly byte[] prevRemainig;
            internal readonly byte[] actualRemaning;
        }
        
        public static void SetFilePath(string filePath_) 
        {
            filePath = filePath_;
        }

        public static void LogPackets(List<float> receivedFloats, byte[] commBuffer, byte[] prevRemainig, byte[] actualRemaning, List<byte[]> packetsSent)
        {
            try
            {
                if(writeFile == null)   
                    writeFile = new FileStream(filePath, FileMode.Append, FileAccess.Write, FileShare.Read);
                byte[] rows = prepareRows(receivedFloats, commBuffer, prevRemainig, actualRemaning, packetsSent);
                writeFile.Write(rows, 0, rows.Length);
            }
            catch (Exception)
            {
                if (writeFile != null) 
                {
                    try {
                        writeFile.Close();
                    } 
                    catch(Exception)
                    {
                    } finally 
                    {
                        writeFile = null;
                    }
                }
            }
        }

        public static void Close()
        {
            if (writeFile != null)
            {
                try
                {
                    writeFile.Close();
                }
                catch (Exception)
                {
                }
                finally
                {
                    writeFile = null;
                }
            }
        }

        private static byte[] prepareRows(List<float> receivedFloats, byte[] commBuffer, byte[] prevRemainig, byte[] actualRemaning, List<byte[]> packetsSent)
        {
            StringBuilder rowsBuilder = new StringBuilder(receivedFloats.Count * 50 + commBuffer.Length + packetsSent.Count * 60 + 20);
            rowsBuilder.AppendLine("Comm packet Nr: " + ++commCounter + " buffer: " + BitConverter.ToString(commBuffer) + " bytes: " + commBuffer.Length);
            for (int i = 0; ; i += 2)
            {
                if (i < receivedFloats.Count - 1)
                {
                    rowsBuilder.AppendLine("\tEasylab Packet Nr: " + ++easylabCounter + " " + receivedFloats[i] + " " + receivedFloats[i + 1] + " Time:" + DateTime.Now + " ");
                }
                else if (i == receivedFloats.Count - 1)
                {
                    rowsBuilder.AppendLine("\tEasylab Wrong Packet Nr:" + ++easylabCounter + " " + receivedFloats[i] + " Time: " + DateTime.Now + " ");
                    break;
                }
                else
                {
                    break;
                }
            }
            rowsBuilder.AppendLine("\t\tPrev. Remaining Buffer: " + BitConverter.ToString(prevRemainig) + " bytes: " + prevRemainig.Length);
            rowsBuilder.AppendLine("\t\tActual Remaining Buffer: " + BitConverter.ToString(actualRemaning) + " bytes: " + actualRemaning.Length);
            
            for (int i = 0; i < packetsSent.Count; ++i) 
            {
                rowsBuilder.AppendLine("\t\t\tUDP packet Nr:" + (i + 1) + " " + BitConverter.ToString(packetsSent[i]) + " bytes: " + packetsSent[i].Length);
            }
            rowsBuilder.AppendLine();
            return ASCIIEncoding.ASCII.GetBytes(rowsBuilder.ToString());
        }

        private static volatile string filePath = @".\PacketsLog.txt";
        private static volatile FileStream writeFile;
        private static volatile int easylabCounter;
        private static volatile int commCounter;
    }
}
