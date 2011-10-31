namespace EasylabSerialUDPGateway
{
    partial class EasylabSerialUDPGatewayForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            //To clean up notify icon
            if (disposing && this.SerialUDPNotifyIcon != null)
            {
                this.SerialUDPNotifyIcon.Visible = false;
                this.SerialUDPNotifyIcon.Dispose();
                this.SerialUDPNotifyIcon = null;
            }
            
            if (disposing && (components != null))
                components.Dispose();
            
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.Label SerialPortLabel;
            System.Windows.Forms.Label UDPSendingPortLabel;
            System.Windows.Forms.Label ReceivedSerialFloatsLabel;
            System.Windows.Forms.Label UDPReceivingPortLabel;
            System.Windows.Forms.Label ReceivedUDPFloatsLabel;
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EasylabSerialUDPGatewayForm));
            this.SerialPortCombo = new System.Windows.Forms.ComboBox();
            this.ConnectButton = new System.Windows.Forms.Button();
            this.UDPSendingPortUpDown = new System.Windows.Forms.NumericUpDown();
            this.SerialUDPNotifyIcon = new System.Windows.Forms.NotifyIcon(this.components);
            this.DisconnectButton = new System.Windows.Forms.Button();
            this.UDPReceivingPortUpDown = new System.Windows.Forms.NumericUpDown();
            this.ShowReceivedSerialValues = new System.Windows.Forms.CheckBox();
            this.SerialTextBox = new System.Windows.Forms.TextBox();
            this.LogSerialValuesOnFile = new System.Windows.Forms.CheckBox();
            this.UdpTextBox = new System.Windows.Forms.TextBox();
            this.ShowReceivedUDPPackets = new System.Windows.Forms.CheckBox();
            this.LogUDPFloatsOnFile = new System.Windows.Forms.CheckBox();
            SerialPortLabel = new System.Windows.Forms.Label();
            UDPSendingPortLabel = new System.Windows.Forms.Label();
            ReceivedSerialFloatsLabel = new System.Windows.Forms.Label();
            UDPReceivingPortLabel = new System.Windows.Forms.Label();
            ReceivedUDPFloatsLabel = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.UDPSendingPortUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.UDPReceivingPortUpDown)).BeginInit();
            this.SuspendLayout();
            // 
            // SerialPortLabel
            // 
            SerialPortLabel.AutoSize = true;
            SerialPortLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            SerialPortLabel.Location = new System.Drawing.Point(11, 18);
            SerialPortLabel.Name = "SerialPortLabel";
            SerialPortLabel.Size = new System.Drawing.Size(70, 16);
            SerialPortLabel.TabIndex = 1;
            SerialPortLabel.Text = "Serial Port";
            SerialPortLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // UDPSendingPortLabel
            // 
            UDPSendingPortLabel.AutoSize = true;
            UDPSendingPortLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            UDPSendingPortLabel.Location = new System.Drawing.Point(12, 53);
            UDPSendingPortLabel.Name = "UDPSendingPortLabel";
            UDPSendingPortLabel.Size = new System.Drawing.Size(117, 16);
            UDPSendingPortLabel.TabIndex = 5;
            UDPSendingPortLabel.Text = "UDP Sending Port";
            UDPSendingPortLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // ReceivedSerialFloatsLabel
            // 
            ReceivedSerialFloatsLabel.AutoSize = true;
            ReceivedSerialFloatsLabel.Location = new System.Drawing.Point(12, 172);
            ReceivedSerialFloatsLabel.Name = "ReceivedSerialFloatsLabel";
            ReceivedSerialFloatsLabel.Size = new System.Drawing.Size(155, 13);
            ReceivedSerialFloatsLabel.TabIndex = 4;
            ReceivedSerialFloatsLabel.Text = "Received floats from Serial Port";
            // 
            // UDPReceivingPortLabel
            // 
            UDPReceivingPortLabel.AutoSize = true;
            UDPReceivingPortLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            UDPReceivingPortLabel.Location = new System.Drawing.Point(12, 92);
            UDPReceivingPortLabel.Name = "UDPReceivingPortLabel";
            UDPReceivingPortLabel.Size = new System.Drawing.Size(128, 16);
            UDPReceivingPortLabel.TabIndex = 8;
            UDPReceivingPortLabel.Text = "UDP Receiving Port";
            UDPReceivingPortLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // ReceivedUDPFloatsLabel
            // 
            ReceivedUDPFloatsLabel.AutoSize = true;
            ReceivedUDPFloatsLabel.Location = new System.Drawing.Point(11, 326);
            ReceivedUDPFloatsLabel.Name = "ReceivedUDPFloatsLabel";
            ReceivedUDPFloatsLabel.Size = new System.Drawing.Size(143, 13);
            ReceivedUDPFloatsLabel.TabIndex = 15;
            ReceivedUDPFloatsLabel.Text = "Received packets from UDP";
            // 
            // SerialPortCombo
            // 
            this.SerialPortCombo.FormattingEnabled = true;
            this.SerialPortCombo.Location = new System.Drawing.Point(146, 17);
            this.SerialPortCombo.Name = "SerialPortCombo";
            this.SerialPortCombo.Size = new System.Drawing.Size(66, 21);
            this.SerialPortCombo.TabIndex = 0;
            // 
            // ConnectButton
            // 
            this.ConnectButton.Location = new System.Drawing.Point(14, 431);
            this.ConnectButton.Name = "ConnectButton";
            this.ConnectButton.Size = new System.Drawing.Size(75, 23);
            this.ConnectButton.TabIndex = 3;
            this.ConnectButton.Text = "Connect";
            this.ConnectButton.UseVisualStyleBackColor = true;
            this.ConnectButton.Click += new System.EventHandler(this.ConnectButton_Click);
            // 
            // UDPSendingPortUpDown
            // 
            this.UDPSendingPortUpDown.Location = new System.Drawing.Point(146, 53);
            this.UDPSendingPortUpDown.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.UDPSendingPortUpDown.Name = "UDPSendingPortUpDown";
            this.UDPSendingPortUpDown.Size = new System.Drawing.Size(65, 20);
            this.UDPSendingPortUpDown.TabIndex = 6;
            this.UDPSendingPortUpDown.Value = new decimal(new int[] {
            50002,
            0,
            0,
            0});
            // 
            // SerialUDPNotifyIcon
            // 
            this.SerialUDPNotifyIcon.Icon = ((System.Drawing.Icon)(resources.GetObject("SerialUDPNotifyIcon.Icon")));
            this.SerialUDPNotifyIcon.Text = "Serial UDP Gateway";
            this.SerialUDPNotifyIcon.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.notifyIcon1_MouseDoubleClick);
            // 
            // DisconnectButton
            // 
            this.DisconnectButton.Enabled = false;
            this.DisconnectButton.Location = new System.Drawing.Point(137, 431);
            this.DisconnectButton.Name = "DisconnectButton";
            this.DisconnectButton.Size = new System.Drawing.Size(75, 23);
            this.DisconnectButton.TabIndex = 7;
            this.DisconnectButton.Text = "Disconnect";
            this.DisconnectButton.UseVisualStyleBackColor = true;
            this.DisconnectButton.Click += new System.EventHandler(this.DisconnectButton_Click);
            // 
            // UDPReceivingPortUpDown
            // 
            this.UDPReceivingPortUpDown.Location = new System.Drawing.Point(146, 92);
            this.UDPReceivingPortUpDown.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.UDPReceivingPortUpDown.Name = "UDPReceivingPortUpDown";
            this.UDPReceivingPortUpDown.Size = new System.Drawing.Size(65, 20);
            this.UDPReceivingPortUpDown.TabIndex = 9;
            this.UDPReceivingPortUpDown.Value = new decimal(new int[] {
            50001,
            0,
            0,
            0});
            // 
            // ShowReceivedSerialValues
            // 
            this.ShowReceivedSerialValues.AutoSize = true;
            this.ShowReceivedSerialValues.Location = new System.Drawing.Point(14, 128);
            this.ShowReceivedSerialValues.Name = "ShowReceivedSerialValues";
            this.ShowReceivedSerialValues.Size = new System.Drawing.Size(166, 17);
            this.ShowReceivedSerialValues.TabIndex = 10;
            this.ShowReceivedSerialValues.Text = "Show Received Serial Values";
            this.ShowReceivedSerialValues.UseVisualStyleBackColor = true;
            // 
            // SerialTextBox
            // 
            this.SerialTextBox.Location = new System.Drawing.Point(14, 188);
            this.SerialTextBox.Multiline = true;
            this.SerialTextBox.Name = "SerialTextBox";
            this.SerialTextBox.ReadOnly = true;
            this.SerialTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.SerialTextBox.Size = new System.Drawing.Size(198, 83);
            this.SerialTextBox.TabIndex = 12;
            // 
            // LogSerialValuesOnFile
            // 
            this.LogSerialValuesOnFile.AutoSize = true;
            this.LogSerialValuesOnFile.Location = new System.Drawing.Point(14, 152);
            this.LogSerialValuesOnFile.Name = "LogSerialValuesOnFile";
            this.LogSerialValuesOnFile.Size = new System.Drawing.Size(144, 17);
            this.LogSerialValuesOnFile.TabIndex = 13;
            this.LogSerialValuesOnFile.Text = "Log Serial Values On File";
            this.LogSerialValuesOnFile.UseVisualStyleBackColor = true;
            // 
            // UdpTextBox
            // 
            this.UdpTextBox.Location = new System.Drawing.Point(14, 342);
            this.UdpTextBox.Multiline = true;
            this.UdpTextBox.Name = "UdpTextBox";
            this.UdpTextBox.ReadOnly = true;
            this.UdpTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.UdpTextBox.Size = new System.Drawing.Size(198, 83);
            this.UdpTextBox.TabIndex = 14;
            // 
            // ShowReceivedUDPPackets
            // 
            this.ShowReceivedUDPPackets.AutoSize = true;
            this.ShowReceivedUDPPackets.Location = new System.Drawing.Point(14, 277);
            this.ShowReceivedUDPPackets.Name = "ShowReceivedUDPPackets";
            this.ShowReceivedUDPPackets.Size = new System.Drawing.Size(169, 17);
            this.ShowReceivedUDPPackets.TabIndex = 16;
            this.ShowReceivedUDPPackets.Text = "Show Received UDP packets";
            this.ShowReceivedUDPPackets.UseVisualStyleBackColor = true;
            // 
            // LogUDPFloatsOnFile
            // 
            this.LogUDPFloatsOnFile.AutoSize = true;
            this.LogUDPFloatsOnFile.Location = new System.Drawing.Point(14, 300);
            this.LogUDPFloatsOnFile.Name = "LogUDPFloatsOnFile";
            this.LogUDPFloatsOnFile.Size = new System.Drawing.Size(134, 17);
            this.LogUDPFloatsOnFile.TabIndex = 17;
            this.LogUDPFloatsOnFile.Text = "Log UDP floats On File";
            this.LogUDPFloatsOnFile.UseVisualStyleBackColor = true;
            // 
            // EasylabSerialUDPGatewayForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(225, 478);
            this.Controls.Add(this.LogUDPFloatsOnFile);
            this.Controls.Add(this.ShowReceivedUDPPackets);
            this.Controls.Add(ReceivedUDPFloatsLabel);
            this.Controls.Add(this.UdpTextBox);
            this.Controls.Add(this.LogSerialValuesOnFile);
            this.Controls.Add(this.SerialTextBox);
            this.Controls.Add(this.ShowReceivedSerialValues);
            this.Controls.Add(this.UDPReceivingPortUpDown);
            this.Controls.Add(UDPReceivingPortLabel);
            this.Controls.Add(this.DisconnectButton);
            this.Controls.Add(this.UDPSendingPortUpDown);
            this.Controls.Add(UDPSendingPortLabel);
            this.Controls.Add(ReceivedSerialFloatsLabel);
            this.Controls.Add(this.ConnectButton);
            this.Controls.Add(SerialPortLabel);
            this.Controls.Add(this.SerialPortCombo);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "EasylabSerialUDPGatewayForm";
            this.Text = "Easylab Serial UDP Gateway";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.SerialUDPGatewayForm_FormClosing);
            this.Resize += new System.EventHandler(this.SerialUDPBrokerForm_Resize);
            ((System.ComponentModel.ISupportInitialize)(this.UDPSendingPortUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.UDPReceivingPortUpDown)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private void SerialUDPBrokerForm_Resize(object sender, System.EventArgs e)
        {
            if (System.Windows.Forms.FormWindowState.Minimized == WindowState)
            {
                Hide();
                this.SerialUDPNotifyIcon.Visible = true;
            }
        }

        private void notifyIcon1_MouseDoubleClick(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            Show();
            WindowState = System.Windows.Forms.FormWindowState.Normal;
            this.SerialUDPNotifyIcon.Visible = false;
        }

        private void SerialUDPGatewayForm_FormClosing(object sender, System.Windows.Forms.FormClosingEventArgs e)
        {
            Disconnect();
            udpCommunicator.Close();
            DebugLogger.Close();
        }

        private System.Windows.Forms.ComboBox SerialPortCombo;
        private System.Windows.Forms.Button ConnectButton;
        private System.Windows.Forms.NumericUpDown UDPSendingPortUpDown;
        private System.Windows.Forms.NotifyIcon SerialUDPNotifyIcon;
        private System.Windows.Forms.Button DisconnectButton;
        private System.Windows.Forms.NumericUpDown UDPReceivingPortUpDown;
        private System.Windows.Forms.CheckBox ShowReceivedSerialValues;
        private System.Windows.Forms.TextBox SerialTextBox;
        private System.Windows.Forms.CheckBox LogSerialValuesOnFile;
        private System.Windows.Forms.TextBox UdpTextBox;
        private System.Windows.Forms.CheckBox ShowReceivedUDPPackets;
        private System.Windows.Forms.CheckBox LogUDPFloatsOnFile;
    }
}

