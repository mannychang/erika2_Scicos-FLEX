using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Text;
using System.Net;
using System.Windows.Forms;



namespace EasylabSerialUDPGateway
{
    public partial class EasylabSerialUDPGatewayForm : Form
    {
        public EasylabSerialUDPGatewayForm()
        {
            SendPacket = new byte[StreamParserEncoder.UDP_PACKET_SIZE];
            CommParserEncoder = new StreamParserEncoder();
            CommPort = new SerialPort();
            udpCommunicator = new UdpCommunicator(udpReceivingHandler); 
            InitializeComponent();
            InitializeCoponentUser();
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

        private void ConnectButton_Click(object sender, EventArgs e)
        {
            if ((CommPort == null || !CommPort.IsOpen) && !String.IsNullOrEmpty(SerialPortCombo.Text))
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
                    MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void DisconnectButton_Click(object sender, EventArgs e)
        {
            try
            {
                if(CommPort != null && CommPort.IsOpen)
                    CommPort.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error Serial Port", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            finally 
            {
                CommPort = null;
            }

            udpCommunicator.Disconnect();

            ConnectButton.Enabled = true;
            DisconnectButton.Enabled = false;
        }

        private void udpReceivingHandler(object sender, byte[] udpDatagram)
        {
            //Echo received bytes on Comm port
            List<byte[]> packets = CommParserEncoder.EncodeBytes(udpDatagram);
            SerialPort LocalPort = CommPort;
            if(LocalPort != null)
                foreach (byte[] p in packets)
                    try
                    {
                        if (LocalPort.IsOpen)
                            CommPort.Write(p, 0, p.Length);
                        else
                            break;
                    }
                    catch (Exception ex) 
                    {
                        MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
       }

        private void commPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {

            SerialPort LocalPort = (SerialPort)sender;
            try
            {   
                //read data waiting in the buffer
                int bytes = LocalPort.BytesToRead;
                if (bytes > 0)
                {
                    byte[] commBuffer = new byte[bytes];
                    CommPort.Read(commBuffer, 0, bytes);

                    //Convert byte received to floats & populate paket to send trought UDP
                    List<float> receivedFloats = CommParserEncoder.ParseBytes(commBuffer);

                    //Send packets trought the socket;
                    SendPackets(receivedFloats);

                    if(this.ShowReceivedValues.Checked)
                        //print floats on rech text box, in main thread with Action Delegate (best way to implement 'almost anonymous delegate' in .NET 2.0)
                        this.BeginInvoke(new Action<List<float>>(PrintFloats), new object[] { receivedFloats });

                    if (this.LogValuesOnFile.Checked)
                        DebugLogger.LogPackets(receivedFloats);
                }
            }
            catch (Exception ex) 
            {
                MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
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
                if (ConsoleLikeTextBox.Text.Length + floatString.Length > ConsoleLikeTextBox.MaxLength)
                {
                    ConsoleLikeTextBox.Text = ConsoleLikeTextBox.Text.Remove(0, ConsoleLikeTextBox.MaxLength - floatString.Length);
                }

                ConsoleLikeTextBox.AppendText(floatString);
                //Piazza la scroll bar alla fine del testo per visualizzare i nuovi valori.
                ConsoleLikeTextBox.ScrollToCaret();
            }
        }

        private void SendPackets(List<float> receivedFloats)
        { 
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
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }
        
        private volatile SerialPort             CommPort;
        private readonly StreamParserEncoder    CommParserEncoder;
        private readonly UdpCommunicator        udpCommunicator;
        private readonly byte[]                 SendPacket;
    }
}
