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
            ReceivingThread.IsBackground = true;
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
                this.SendingEndPoint = SendingEndPoint;
                udpSendingClient   = new UdpClient();
                this.ReceivingEndPoint = ReceivingEndPoint;
                udpReceivingClient = new UdpClient(ReceivingEndPoint);
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
            {
                try
                {
                    UdpClient udpClient = udpSendingClient;
                    udpSendingClient = null;
                    udpClient.Close();
                }
                catch (Exception)
                {
                }
            }
            
            if (udpReceivingClient != null)
            {
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

            try
            {
                ReceivingThread.Interrupt();
            }
            catch(Exception)
            {
            }
        }

        public void Close()
        {
            ReceivingActive = false;
            Disconnect();
            if (ReceivingThread.IsAlive)
            {
                try
                {
                    ReceivingThread.Join();
                }
                catch (Exception)
                {
                }
            }
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
                        UdpClient tempUdpReceivingClient = this.udpReceivingClient;
                        //udpReceivingClient = new UdpClient(tempReceivingEndPoint);
                        if (tempUdpReceivingClient != null && tempReceivingEndPoint != null)
                            try
                            {
                                byte[] udpDatagram = udpReceivingClient.Receive(ref tempReceivingEndPoint);
                                if (udpReceivingHandler != null)
                                    udpReceivingHandler(this, udpDatagram);
                            }
                            catch (Exception)
                            {
                            }
                            /*finally
                            {
                                udpReceivingClient.Close();
                            }*/
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
            UdpClient localUdpSendingClient = udpSendingClient;
            IPEndPoint localSendingEndPoint = SendingEndPoint;
            if (udpSendingClient != null && localSendingEndPoint != null)
                udpSendingClient.Send(SendPacket, SendPacket.Length, SendingEndPoint);
        }

        private readonly Thread ReceivingThread;
        private readonly AutoResetEvent ResumeReceiving;
        private readonly UdpReceivingHandler udpReceivingHandler;
        private volatile UdpClient udpSendingClient;
        private volatile UdpClient udpReceivingClient;
        private volatile IPEndPoint ReceivingEndPoint;
        private volatile IPEndPoint SendingEndPoint;
        private volatile bool ReceivingActive;
        private volatile bool ReceivingClosed;
    }
}
