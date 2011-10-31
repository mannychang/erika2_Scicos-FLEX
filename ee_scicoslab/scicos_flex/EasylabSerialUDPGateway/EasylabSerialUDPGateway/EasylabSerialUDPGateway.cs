using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Text;
using System.Net;
using System.Windows.Forms;
//For Dll Import
using System.Runtime.InteropServices;



namespace EasylabSerialUDPGateway
{
    
    public partial class EasylabSerialUDPGatewayForm : Form
    {
        public EasylabSerialUDPGatewayForm()
        {
            CommPortMonitor = new object();
            SendPacket = new byte[StreamParserEncoder.UDP_PACKET_SIZE];
            CommParserEncoder = new StreamParserEncoder();
            CommPort = new SerialPort();
            udpCommunicator = new UdpCommunicator(udpReceivingHandler); 
            InitializeComponent();
            InitializeCoponentUser();
        }

        public EasylabSerialUDPGatewayForm(string commPort, decimal receivingPort, decimal sendingPort): this()
        {
            SerialPortCombo.Text         = commPort;
            UDPReceivingPortUpDown.Value = receivingPort;
            UDPSendingPortUpDown.Value   = sendingPort;
            Connect();
        }

        /*
         * Imported win32 API to register a message
         */
        [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        public static extern uint RegisterWindowMessage(string lpString);
        
        /*
         * Overriding WndProc to handle reset message.
         */
        protected override void WndProc(ref Message m)
        {
            if (m.Msg == resetMsgId)
            {
                /* Reset connection */
                this.Disconnect();
                System.Threading.Thread.Sleep(50);
                this.Connect();
            }
            base.WndProc(ref m);
        }

        private void InitializeCoponentUser() 
        {
            //Popolo la Combo box con i valori delle porte comm rilevate.
            string[] theSerialPortNames = SerialPort.GetPortNames();
            SerialPortCombo.Items.AddRange(theSerialPortNames);
            // Visualizzo il primo trovato
            if (theSerialPortNames.Length > 0)
                SerialPortCombo.SelectedIndex = 0; 
        }

        private void Connect()
        {
            try
            {
                CommPort = new SerialPort(SerialPortCombo.Text, 115200, Parity.None, 8, StopBits.One);
                //RTS-CTS policy
                CommPort.Handshake = Handshake.None;
                //Data Receiver handler
                CommPort.DataReceived += new SerialDataReceivedEventHandler(commPort_DataReceived);

                //Initialize end-points
                IPEndPoint  SendingEndPoint = new IPEndPoint(IPAddress.Loopback, (int)UDPSendingPortUpDown.Value);
                IPEndPoint  ReceivingEndPoint = new IPEndPoint(IPAddress.Loopback, (int)UDPReceivingPortUpDown.Value);

                udpCommunicator.Connect(SendingEndPoint, ReceivingEndPoint);

                //Open communications
                CommPort.Open();

                //Disable Connect/Enable Disconnect Buttons
                ConnectButton.Enabled = false;
                DisconnectButton.Enabled = true;
            }
            catch (Exception ex) 
            {
                MessageBox.Show(ex.Message, "Connect Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void Disconnect()
        {
            if (DisconnectButton.Enabled)
            {
                try
                {
                    if (CommPort != null && CommPort.IsOpen)
                        CommPort.Close();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Disconnect Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                finally
                {
                    CommPort = null;
                }
                udpCommunicator.Disconnect();
            }
        }

        private void ConnectButton_Click(object sender, EventArgs e)
        {
            if ((CommPort == null || !CommPort.IsOpen) && !String.IsNullOrEmpty(SerialPortCombo.Text))
            {
                Connect();
            }
        }

        private void DisconnectButton_Click(object sender, EventArgs e)
        {
            Disconnect();

            ConnectButton.Enabled = true;
            DisconnectButton.Enabled = false;
        }

        private void udpReceivingHandler(object sender, byte[] udpDatagram)
        {
            //Create Easylab packets from UDP dataghram
            List<byte[]> packets = CommParserEncoder.EncodeBytes(udpDatagram);

            if (this.ShowReceivedUDPPackets.Checked)
                //print UDP packet in UDP text box, in main thread with Action Delegate (best way to implement 'almost anonymous delegate' in .NET 2.0)
                this.BeginInvoke(new Action<List<byte[]>>(PrintUDP), new object[] { packets });

            if (this.LogUDPFloatsOnFile.Checked)
                DebugLogger.LogUDPPackets(packets);

            //Echo received bytes on Comm port
            SerialPort LocalPort = CommPort;
            if (LocalPort != null)
            {
                foreach (byte[] p in packets)
                {
                    try
                    {
                        if (LocalPort.IsOpen)
                            CommPort.Write(p, 0, p.Length);
                        else
                            break;
                    }
                    catch (Exception /*ex*/)
                    {
                        //MessageBox.Show(ex.Message, "UDP Receiving Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
       }

        private void commPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {

            lock (CommPortMonitor)
            {
                SerialPort LocalPort = (SerialPort)sender;
                if (LocalPort != null)
                {
                    try
                    {
                        //read data waiting in the buffer
                        int bytes = LocalPort.BytesToRead;
                        if (bytes > 0)
                        {
                            byte[] commBuffer = new byte[bytes];
                            CommPort.Read(commBuffer, 0, bytes);

                            //Convert byte received to floats & populate paket to send trought UDP
                            byte[] prevRemainig;
                            byte[] actualRemaning;
                            List<float> receivedFloats = CommParserEncoder.ParseBytes(commBuffer, out prevRemainig, out actualRemaning);

                            //Send packets trought the socket;
                            List<byte[]> packetsSent = SendPackets(receivedFloats);

                            if (this.ShowReceivedSerialValues.Checked)
                                //print floats on serial text box, in main thread with Action Delegate (best way to implement 'almost anonymous delegate' in .NET 2.0)
                                this.BeginInvoke(new Action<List<float>>(PrintFloats), new object[] { receivedFloats });

                            if (this.LogSerialValuesOnFile.Checked)
                                DebugLogger.LogPackets(receivedFloats, commBuffer, prevRemainig, actualRemaning, packetsSent);
                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message, "COM Port Receiving Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
        }

        private void PrintFloats(List<float> receivedFloats)
        {

            StringBuilder floatStringBuilder = new StringBuilder(receivedFloats.Count * 20);
            for (int i = 0; ; i += 2)
            {
                if (i < receivedFloats.Count - 1)
                {
                    floatStringBuilder.AppendLine(receivedFloats[i] + " " + receivedFloats[i + 1]);
                }
                else if (i == receivedFloats.Count - 1)
                {
                    floatStringBuilder.AppendLine(receivedFloats[i].ToString());
                    break;
                }
                else
                {
                    break;
                }
            }
            string floatString = floatStringBuilder.ToString();
            if (!String.IsNullOrEmpty(floatString))
            {
                //Check se non supero MaxLegth. Se lo faccio rimuovo un numero di caratteri sufficient i dalla testa della stringa
                if (SerialTextBox.Text.Length + floatString.Length > SerialTextBox.MaxLength)
                {
                    SerialTextBox.Text = SerialTextBox.Text.Remove(0, SerialTextBox.MaxLength - floatString.Length);
                }

                SerialTextBox.AppendText(floatString);
                //Piazza la scroll bar alla fine del testo per visualizzare i nuovi valori.
                SerialTextBox.ScrollToCaret();
            }
        }

        private void PrintUDP(List<byte[]> udpPackets) {
            StringBuilder stringBuilder = new StringBuilder(udpPackets.Count * 60);
            foreach (byte[] p in udpPackets)
            {
                stringBuilder.AppendLine(BitConverter.ToString(p));
            }
            UdpTextBox.AppendText(stringBuilder.ToString());
            UdpTextBox.ScrollToCaret();
        }

        private List<byte[]> SendPackets(List<float> receivedFloats)
        {
            List<byte[]> packetsSentList = new List<byte[]>();
            for (int i = 0; ; i += 2)
            {
                if (i < receivedFloats.Count - 1)
                {
                    Array.Copy(BitConverter.GetBytes(receivedFloats[i]), SendPacket, sizeof(float));
                    Array.Copy(BitConverter.GetBytes(receivedFloats[i + 1]), 0, SendPacket, sizeof(float), sizeof(float));
                }                    
                else if (i == receivedFloats.Count - 1)
                {
                    Array.Copy(BitConverter.GetBytes(receivedFloats[i]), SendPacket, sizeof(float));
                    for (int j = sizeof(float); j < sizeof(float); ++j)
                    {
                        SendPacket[j] = 0;
                    }

                }
                else
                {
                    break;
                }
                try
                {
                    udpCommunicator.Send(SendPacket);
                    byte[] copy = new byte[SendPacket.Length];
                    SendPacket.CopyTo(copy, 0);
                    packetsSentList.Add(copy);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "UDP Send Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            return packetsSentList;
        }

        public static readonly uint resetMsgId = RegisterWindowMessage("Reset Easylab Gateway");

        private volatile SerialPort             CommPort;
        private readonly object                 CommPortMonitor;
        private readonly StreamParserEncoder    CommParserEncoder;
        private readonly UdpCommunicator        udpCommunicator;
        private readonly byte[]                 SendPacket;
    }
}
