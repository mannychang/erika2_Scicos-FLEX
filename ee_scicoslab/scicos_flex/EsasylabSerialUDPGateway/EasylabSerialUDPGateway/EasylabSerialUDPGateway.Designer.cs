﻿namespace EasylabSerialUDPGateway
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
            System.Windows.Forms.Label ReceivedFloatsLabel;
            System.Windows.Forms.Label UDPReceivingPortLabel;
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EasylabSerialUDPGatewayForm));
            this.SerialPortCombo = new System.Windows.Forms.ComboBox();
            this.ConnectButton = new System.Windows.Forms.Button();
            this.UDPSendingPortUpDown = new System.Windows.Forms.NumericUpDown();
            this.SerialUDPNotifyIcon = new System.Windows.Forms.NotifyIcon(this.components);
            this.DisconnectButton = new System.Windows.Forms.Button();
            this.UDPReceivingPortUpDown = new System.Windows.Forms.NumericUpDown();
            this.ShowReceivedValues = new System.Windows.Forms.CheckBox();
            this.ConsoleLikeTextBox = new System.Windows.Forms.TextBox();
            this.LogValuesOnFile = new System.Windows.Forms.CheckBox();
            SerialPortLabel = new System.Windows.Forms.Label();
            UDPSendingPortLabel = new System.Windows.Forms.Label();
            ReceivedFloatsLabel = new System.Windows.Forms.Label();
            UDPReceivingPortLabel = new System.Windows.Forms.Label();
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
            // ReceivedFloatsLabel
            // 
            ReceivedFloatsLabel.AutoSize = true;
            ReceivedFloatsLabel.Location = new System.Drawing.Point(11, 172);
            ReceivedFloatsLabel.Name = "ReceivedFloatsLabel";
            ReceivedFloatsLabel.Size = new System.Drawing.Size(76, 13);
            ReceivedFloatsLabel.TabIndex = 4;
            ReceivedFloatsLabel.Text = "received floats";
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
            this.ConnectButton.Location = new System.Drawing.Point(14, 277);
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
            50000,
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
            this.DisconnectButton.Location = new System.Drawing.Point(136, 277);
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
            // ShowReceivedValues
            // 
            this.ShowReceivedValues.AutoSize = true;
            this.ShowReceivedValues.Location = new System.Drawing.Point(14, 128);
            this.ShowReceivedValues.Name = "ShowReceivedValues";
            this.ShowReceivedValues.Size = new System.Drawing.Size(137, 17);
            this.ShowReceivedValues.TabIndex = 10;
            this.ShowReceivedValues.Text = "Show Received Values";
            this.ShowReceivedValues.UseVisualStyleBackColor = true;
            // 
            // ConsoleLikeTextBox
            // 
            this.ConsoleLikeTextBox.Location = new System.Drawing.Point(14, 188);
            this.ConsoleLikeTextBox.Multiline = true;
            this.ConsoleLikeTextBox.Name = "ConsoleLikeTextBox";
            this.ConsoleLikeTextBox.ReadOnly = true;
            this.ConsoleLikeTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.ConsoleLikeTextBox.Size = new System.Drawing.Size(198, 83);
            this.ConsoleLikeTextBox.TabIndex = 12;
            // 
            // LogValuesOnFile
            // 
            this.LogValuesOnFile.AutoSize = true;
            this.LogValuesOnFile.Location = new System.Drawing.Point(15, 152);
            this.LogValuesOnFile.Name = "LogValuesOnFile";
            this.LogValuesOnFile.Size = new System.Drawing.Size(115, 17);
            this.LogValuesOnFile.TabIndex = 13;
            this.LogValuesOnFile.Text = "Log Values On File";
            this.LogValuesOnFile.UseVisualStyleBackColor = true;
            // 
            // EasylabSerialUDPGatewayForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(235, 308);
            this.Controls.Add(this.LogValuesOnFile);
            this.Controls.Add(this.ConsoleLikeTextBox);
            this.Controls.Add(this.ShowReceivedValues);
            this.Controls.Add(this.UDPReceivingPortUpDown);
            this.Controls.Add(UDPReceivingPortLabel);
            this.Controls.Add(this.DisconnectButton);
            this.Controls.Add(this.UDPSendingPortUpDown);
            this.Controls.Add(UDPSendingPortLabel);
            this.Controls.Add(ReceivedFloatsLabel);
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
        private System.Windows.Forms.CheckBox ShowReceivedValues;
        private System.Windows.Forms.TextBox ConsoleLikeTextBox;
        private System.Windows.Forms.CheckBox LogValuesOnFile;
    }
}
