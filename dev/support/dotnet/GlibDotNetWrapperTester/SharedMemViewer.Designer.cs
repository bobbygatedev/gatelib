namespace GlibDotNetWrapperTester
{
    partial class SharedMemViewer
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
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.TheTextSharedName = new System.Windows.Forms.TextBox();
            this.Thelabel1 = new System.Windows.Forms.Label();
            this.TheComboValueTypes = new System.Windows.Forms.ComboBox();
            this.TheLabel2 = new System.Windows.Forms.Label();
            this.TheTextSharedValue = new System.Windows.Forms.TextBox();
            this.TheButtonRead = new System.Windows.Forms.Button();
            this.TheButtonWrite = new System.Windows.Forms.Button();
            this.TheCheckIsHex = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // TheTextSharedName
            // 
            this.TheTextSharedName.Location = new System.Drawing.Point(45, 72);
            this.TheTextSharedName.Name = "TheTextSharedName";
            this.TheTextSharedName.Size = new System.Drawing.Size(379, 20);
            this.TheTextSharedName.TabIndex = 0;
            // 
            // Thelabel1
            // 
            this.Thelabel1.AutoSize = true;
            this.Thelabel1.Location = new System.Drawing.Point(42, 56);
            this.Thelabel1.Name = "Thelabel1";
            this.Thelabel1.Size = new System.Drawing.Size(69, 13);
            this.Thelabel1.TabIndex = 1;
            this.Thelabel1.Text = "SharedName";
            // 
            // TheComboValueTypes
            // 
            this.TheComboValueTypes.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.TheComboValueTypes.FormattingEnabled = true;
            this.TheComboValueTypes.Location = new System.Drawing.Point(45, 145);
            this.TheComboValueTypes.Name = "TheComboValueTypes";
            this.TheComboValueTypes.Size = new System.Drawing.Size(121, 21);
            this.TheComboValueTypes.TabIndex = 2;
            // 
            // TheLabel2
            // 
            this.TheLabel2.AutoSize = true;
            this.TheLabel2.Location = new System.Drawing.Point(196, 130);
            this.TheLabel2.Name = "TheLabel2";
            this.TheLabel2.Size = new System.Drawing.Size(34, 13);
            this.TheLabel2.TabIndex = 4;
            this.TheLabel2.Text = "Value";
            // 
            // TheTextSharedValue
            // 
            this.TheTextSharedValue.Location = new System.Drawing.Point(199, 146);
            this.TheTextSharedValue.Name = "TheTextSharedValue";
            this.TheTextSharedValue.Size = new System.Drawing.Size(225, 20);
            this.TheTextSharedValue.TabIndex = 3;
            // 
            // TheButtonRead
            // 
            this.TheButtonRead.Location = new System.Drawing.Point(457, 130);
            this.TheButtonRead.Name = "TheButtonRead";
            this.TheButtonRead.Size = new System.Drawing.Size(75, 23);
            this.TheButtonRead.TabIndex = 5;
            this.TheButtonRead.Text = "&Read";
            this.TheButtonRead.UseVisualStyleBackColor = true;
            this.TheButtonRead.Click += new System.EventHandler(this.TheButtonRead_Click);
            // 
            // TheButtonWrite
            // 
            this.TheButtonWrite.Location = new System.Drawing.Point(457, 159);
            this.TheButtonWrite.Name = "TheButtonWrite";
            this.TheButtonWrite.Size = new System.Drawing.Size(75, 23);
            this.TheButtonWrite.TabIndex = 6;
            this.TheButtonWrite.Text = "&Write";
            this.TheButtonWrite.UseVisualStyleBackColor = true;
            this.TheButtonWrite.Click += new System.EventHandler(this.TheButtonWrite_Click);
            // 
            // TheCheckIsHex
            // 
            this.TheCheckIsHex.AutoSize = true;
            this.TheCheckIsHex.Location = new System.Drawing.Point(199, 172);
            this.TheCheckIsHex.Name = "TheCheckIsHex";
            this.TheCheckIsHex.Size = new System.Drawing.Size(56, 17);
            this.TheCheckIsHex.TabIndex = 7;
            this.TheCheckIsHex.Text = "Is &Hex";
            this.TheCheckIsHex.UseVisualStyleBackColor = true;
            // 
            // SharedMemViewer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(570, 262);
            this.Controls.Add(this.TheCheckIsHex);
            this.Controls.Add(this.TheButtonWrite);
            this.Controls.Add(this.TheButtonRead);
            this.Controls.Add(this.TheLabel2);
            this.Controls.Add(this.TheTextSharedValue);
            this.Controls.Add(this.TheComboValueTypes);
            this.Controls.Add(this.Thelabel1);
            this.Controls.Add(this.TheTextSharedName);
            this.Name = "SharedMemViewer";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox TheTextSharedName;
        private System.Windows.Forms.Label Thelabel1;
        private System.Windows.Forms.ComboBox TheComboValueTypes;
        private System.Windows.Forms.Label TheLabel2;
        private System.Windows.Forms.TextBox TheTextSharedValue;
        private System.Windows.Forms.Button TheButtonRead;
        private System.Windows.Forms.Button TheButtonWrite;
        private System.Windows.Forms.CheckBox TheCheckIsHex;
    }
}

