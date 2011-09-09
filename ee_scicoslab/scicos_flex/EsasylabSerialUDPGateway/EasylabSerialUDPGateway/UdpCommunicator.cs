using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace EasylabSerialUDPGateway
{
    public class UdpCommunicator
    {

        //Utility class that hold async receving info
        class UdpState
        {
            internal UdpState(UdpClient udpClient, IPEndPoint endPoint)
            {
                this.udpClient = udpClient;
                this.endPoint = endPoint;
            }
            internal readonly UdpClient udpClient;
            internal readonly IPEndPoint endPoint;
        }

        public delegate void UdpReceivingHandler(object sender, byte[] udpDatagram);

        public UdpCommunicator(UdpReceivingHandler udpReceivingHandler)
        {
            ReceivingThread = new Thread(ReceiveMessages);
            ResumeReceiving = new AutoResetEvent(false);
            this.udpReceivingHandler = udpReceivingHandler;
        }

        public bool Connect(IPEndPoint SendingEndPoint, IPEndPoint ReceivingEndPoint)
        {
            if (ReceivingClosed)
                return false;

            if (udpSendingClient != null)
                Disconnect();
            
            try
            {
                udpSendingClient   = new UdpClient();
                this.ReceivingEndPoint = ReceivingEndPoint;
                udpSendingClient.Connect(SendingEndPoint);
            } catch(Exception) {
                return false;
            }

            if (ReceivingActive)
            {
                ResumeReceiving.Set();
            }
            else
            {
                ReceivingActive = true;
                ReceivingThread.Start();
            }
            return true;
        }

        public void Disconnect()
        {
                if (udpSendingClient != null)
                try 
                {
                    UdpClient udpClient = udpSendingClient;
                    udpSendingClient = null;
                    udpClient.Close();
                } 
                catch (Exception) 
                {
                }
            if (udpReceivingClient != null)
                try
                {
                    UdpClient udpClient = udpReceivingClient;
                    udpReceivingClient = null;
                    udpClient.Close();
                }
                catch (Exception)
                {
                }
        }

        public void Close()
        {
            ReceivingActive = false;
            Disconnect();
            ReceivingClosed = true;
        }

        private void ReceiveMessages() {
            while (ReceivingActive) 
            {
                try
                {
                    /* udpSendingClient is a flag of communication enabled */
                    if (udpSendingClient != null)
                    {
                        IPEndPoint tempReceivingEndPoint = this.ReceivingEndPoint;
                        udpReceivingClient = new UdpClient(tempReceivingEndPoint);
                        try
                        {
                            byte[] udpDatagram = udpReceivingClient.Receive(ref tempReceivingEndPoint);
                            if (udpReceivingHandler != null)
                                udpReceivingHandler(this, udpDatagram);
                        }
                        catch (Exception)
                        {
                        }
                        finally
                        {
                            udpReceivingClient.Close();
                        }
                    }   
                    else 
                    {
                        ResumeReceiving.WaitOne();
                    }
                } 
                catch(Exception)
                {
                }
            }
        }

        public void Send(byte[] SendPacket)
        {
            if (udpSendingClient != null)
                udpSendingClient.Send(SendPacket, SendPacket.Length);
        }

        private readonly Thread ReceivingThread;
        private readonly AutoResetEvent ResumeReceiving;
        private readonly UdpReceivingHandler udpReceivingHandler;
        private volatile UdpClient udpSendingClient;
        private volatile UdpClient udpReceivingClient;
        private volatile IPEndPoint ReceivingEndPoint;
        private volatile bool ReceivingActive;
        private volatile bool ReceivingClosed;
    }
}
