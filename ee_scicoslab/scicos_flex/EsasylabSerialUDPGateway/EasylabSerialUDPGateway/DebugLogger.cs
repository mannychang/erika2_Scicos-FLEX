using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Threading;
namespace EasylabSerialUDPGateway
{
    public class DebugLogger
    {
        
        private DebugLogger()
        {
            receivedFloatsQueue  = new BlockingQueue<List<float>>();
            asyncWriter = new Thread(writeAsync);
            asyncWriter.IsBackground = true;
            asyncWriter.Start();
        }
        
        public static void SetFilePath(string filePath_) {
            if(instance != null)
                instance.filePath = filePath_;
        }

        public static void Close()
        {
            if (instance != null) {
                try
                {
                    instance.stop = true;
                    instance.asyncWriter.Interrupt();
                }
                catch (Exception) 
                { 
                }
            }
        }

        public static void LogPackets(List<float> receivedFloats)
        {
            if (instance == null)
                instance = new DebugLogger();
            instance.receivedFloatsQueue.Add(receivedFloats);

        }

        private void writeAsync()
        {
            while (!stop)
            {
                try
                {
                    List<float> receivedFloats = receivedFloatsQueue.Take();
                    using (FileStream writeFile = new FileStream(filePath, FileMode.Append))
                    {
                        byte[] rows = prepareRows(receivedFloats);
                        writeFile.Write(rows, 0, rows.Length);
                    }
                }
                catch (Exception) 
                { 
                }
            }
        }
        
        private byte[] prepareRows(List<float> receivedFloats)
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
        private static volatile DebugLogger instance;
        private readonly BlockingQueue<List<float>> receivedFloatsQueue;
        private readonly Thread asyncWriter;
        private volatile bool stop;
        private volatile string filePath = @".\PacketsLog.txt";
        private int staticCounter;
    }
}
