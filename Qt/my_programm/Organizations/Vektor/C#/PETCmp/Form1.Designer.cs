namespace PETCmp
{
    partial class Form1
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Title title1 = new System.Windows.Forms.DataVisualization.Charting.Title();
            this.label1 = new System.Windows.Forms.Label();
            this.tbFileName = new System.Windows.Forms.TextBox();
            this.btnOpen = new System.Windows.Forms.Button();
            this.chart = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.nudChACorr = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.nudChANumber = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.nudChBCorr = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.nudChBNumber = new System.Windows.Forms.NumericUpDown();
            this.label5 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.tbTts = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.nudTInt = new System.Windows.Forms.NumericUpDown();
            this.label6 = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.nudN = new System.Windows.Forms.NumericUpDown();
            this.label10 = new System.Windows.Forms.Label();
            this.nudWindow = new System.Windows.Forms.NumericUpDown();
            this.label9 = new System.Windows.Forms.Label();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.label14 = new System.Windows.Forms.Label();
            this.tbCmpNum = new System.Windows.Forms.TextBox();
            this.label13 = new System.Windows.Forms.Label();
            this.tbTotalEvents = new System.Windows.Forms.TextBox();
            this.cbShowWave = new System.Windows.Forms.CheckBox();
            this.label12 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.tbSKO = new System.Windows.Forms.TextBox();
            this.tbM = new System.Windows.Forms.TextBox();
            this.btnCalculate = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.chart)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudChACorr)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudChANumber)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudChBCorr)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudChBNumber)).BeginInit();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudTInt)).BeginInit();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudN)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudWindow)).BeginInit();
            this.groupBox5.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(79, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Файл данных:";
            // 
            // tbFileName
            // 
            this.tbFileName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbFileName.Location = new System.Drawing.Point(102, 6);
            this.tbFileName.Name = "tbFileName";
            this.tbFileName.ReadOnly = true;
            this.tbFileName.Size = new System.Drawing.Size(812, 20);
            this.tbFileName.TabIndex = 1;
            // 
            // btnOpen
            // 
            this.btnOpen.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnOpen.Location = new System.Drawing.Point(920, 4);
            this.btnOpen.Name = "btnOpen";
            this.btnOpen.Size = new System.Drawing.Size(32, 23);
            this.btnOpen.TabIndex = 2;
            this.btnOpen.Text = "...";
            this.btnOpen.UseVisualStyleBackColor = true;
            this.btnOpen.Click += new System.EventHandler(this.btnOpen_Click);
            // 
            // chart
            // 
            this.chart.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            chartArea1.AxisX.MajorGrid.LineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Dot;
            chartArea1.AxisX.MajorTickMark.Size = 2F;
            chartArea1.AxisX.MinorTickMark.Enabled = true;
            chartArea1.AxisY.MajorGrid.LineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Dot;
            chartArea1.Name = "Default";
            this.chart.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            this.chart.Legends.Add(legend1);
            this.chart.Location = new System.Drawing.Point(238, 32);
            this.chart.Name = "chart";
            this.chart.Palette = System.Windows.Forms.DataVisualization.Charting.ChartColorPalette.Bright;
            this.chart.Size = new System.Drawing.Size(714, 561);
            this.chart.TabIndex = 3;
            this.chart.Text = "Гистограма совпадений";
            title1.Docking = System.Windows.Forms.DataVisualization.Charting.Docking.Bottom;
            title1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F);
            title1.Name = "Title";
            this.chart.Titles.Add(title1);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.nudChACorr);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.nudChANumber);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Location = new System.Drawing.Point(12, 32);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(220, 76);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Канал А";
            // 
            // nudChACorr
            // 
            this.nudChACorr.Location = new System.Drawing.Point(90, 45);
            this.nudChACorr.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.nudChACorr.Minimum = new decimal(new int[] {
            100000,
            0,
            0,
            -2147483648});
            this.nudChACorr.Name = "nudChACorr";
            this.nudChACorr.Size = new System.Drawing.Size(120, 20);
            this.nudChACorr.TabIndex = 3;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 47);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(75, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "Поправка, пс";
            // 
            // nudChANumber
            // 
            this.nudChANumber.Location = new System.Drawing.Point(90, 19);
            this.nudChANumber.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.nudChANumber.Name = "nudChANumber";
            this.nudChANumber.Size = new System.Drawing.Size(120, 20);
            this.nudChANumber.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 21);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(57, 13);
            this.label2.TabIndex = 0;
            this.label2.Text = "№ канала";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.nudChBCorr);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.nudChBNumber);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Location = new System.Drawing.Point(12, 114);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(220, 76);
            this.groupBox2.TabIndex = 5;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Канал Б";
            // 
            // nudChBCorr
            // 
            this.nudChBCorr.Location = new System.Drawing.Point(90, 45);
            this.nudChBCorr.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.nudChBCorr.Minimum = new decimal(new int[] {
            100000,
            0,
            0,
            -2147483648});
            this.nudChBCorr.Name = "nudChBCorr";
            this.nudChBCorr.Size = new System.Drawing.Size(120, 20);
            this.nudChBCorr.TabIndex = 3;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 47);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(75, 13);
            this.label4.TabIndex = 2;
            this.label4.Text = "Поправка, пс";
            // 
            // nudChBNumber
            // 
            this.nudChBNumber.Location = new System.Drawing.Point(90, 19);
            this.nudChBNumber.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.nudChBNumber.Name = "nudChBNumber";
            this.nudChBNumber.Size = new System.Drawing.Size(120, 20);
            this.nudChBNumber.TabIndex = 1;
            this.nudChBNumber.Value = new decimal(new int[] {
            2,
            0,
            0,
            0});
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 21);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(57, 13);
            this.label5.TabIndex = 0;
            this.label5.Text = "№ канала";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.tbTts);
            this.groupBox3.Controls.Add(this.label8);
            this.groupBox3.Controls.Add(this.nudTInt);
            this.groupBox3.Controls.Add(this.label6);
            this.groupBox3.Location = new System.Drawing.Point(12, 196);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(220, 74);
            this.groupBox3.TabIndex = 6;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Метка времени";
            // 
            // tbTts
            // 
            this.tbTts.Location = new System.Drawing.Point(89, 45);
            this.tbTts.Name = "tbTts";
            this.tbTts.ReadOnly = true;
            this.tbTts.Size = new System.Drawing.Size(120, 20);
            this.tbTts.TabIndex = 12;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 48);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(40, 13);
            this.label8.TabIndex = 11;
            this.label8.Text = "Tts, пс";
            // 
            // nudTInt
            // 
            this.nudTInt.DecimalPlaces = 1;
            this.nudTInt.Location = new System.Drawing.Point(90, 19);
            this.nudTInt.Name = "nudTInt";
            this.nudTInt.Size = new System.Drawing.Size(120, 20);
            this.nudTInt.TabIndex = 7;
            this.nudTInt.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudTInt.ValueChanged += new System.EventHandler(this.nudTInt_ValueChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 21);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(43, 13);
            this.label6.TabIndex = 0;
            this.label6.Text = "Tint, нс";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.nudN);
            this.groupBox4.Controls.Add(this.label10);
            this.groupBox4.Controls.Add(this.nudWindow);
            this.groupBox4.Controls.Add(this.label9);
            this.groupBox4.Location = new System.Drawing.Point(12, 276);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(220, 77);
            this.groupBox4.TabIndex = 7;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Окно совпадений";
            // 
            // nudN
            // 
            this.nudN.DecimalPlaces = 1;
            this.nudN.Location = new System.Drawing.Point(94, 45);
            this.nudN.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.nudN.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.nudN.Name = "nudN";
            this.nudN.Size = new System.Drawing.Size(120, 20);
            this.nudN.TabIndex = 9;
            this.nudN.Value = new decimal(new int[] {
            1024,
            0,
            0,
            0});
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(6, 47);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(15, 13);
            this.label10.TabIndex = 8;
            this.label10.Text = "N";
            // 
            // nudWindow
            // 
            this.nudWindow.DecimalPlaces = 1;
            this.nudWindow.Location = new System.Drawing.Point(94, 19);
            this.nudWindow.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.nudWindow.Name = "nudWindow";
            this.nudWindow.Size = new System.Drawing.Size(120, 20);
            this.nudWindow.TabIndex = 9;
            this.nudWindow.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(6, 21);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(40, 13);
            this.label9.TabIndex = 8;
            this.label9.Text = "Tw, нс";
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.label14);
            this.groupBox5.Controls.Add(this.tbCmpNum);
            this.groupBox5.Controls.Add(this.label13);
            this.groupBox5.Controls.Add(this.tbTotalEvents);
            this.groupBox5.Controls.Add(this.cbShowWave);
            this.groupBox5.Controls.Add(this.label12);
            this.groupBox5.Controls.Add(this.label11);
            this.groupBox5.Controls.Add(this.tbSKO);
            this.groupBox5.Controls.Add(this.tbM);
            this.groupBox5.Location = new System.Drawing.Point(12, 359);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(220, 149);
            this.groupBox5.TabIndex = 8;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Анализ";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(7, 48);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(68, 13);
            this.label14.TabIndex = 8;
            this.label14.Text = "Совпадений";
            // 
            // tbCmpNum
            // 
            this.tbCmpNum.Location = new System.Drawing.Point(89, 45);
            this.tbCmpNum.Name = "tbCmpNum";
            this.tbCmpNum.ReadOnly = true;
            this.tbCmpNum.Size = new System.Drawing.Size(120, 20);
            this.tbCmpNum.TabIndex = 7;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(7, 22);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(51, 13);
            this.label13.TabIndex = 6;
            this.label13.Text = "Событий";
            // 
            // tbTotalEvents
            // 
            this.tbTotalEvents.Location = new System.Drawing.Point(90, 19);
            this.tbTotalEvents.Name = "tbTotalEvents";
            this.tbTotalEvents.ReadOnly = true;
            this.tbTotalEvents.Size = new System.Drawing.Size(120, 20);
            this.tbTotalEvents.TabIndex = 5;
            // 
            // cbShowWave
            // 
            this.cbShowWave.AutoSize = true;
            this.cbShowWave.Location = new System.Drawing.Point(9, 123);
            this.cbShowWave.Name = "cbShowWave";
            this.cbShowWave.Size = new System.Drawing.Size(137, 17);
            this.cbShowWave.TabIndex = 4;
            this.cbShowWave.Text = "Показать огибающую";
            this.cbShowWave.UseVisualStyleBackColor = true;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(7, 100);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(29, 13);
            this.label12.TabIndex = 3;
            this.label12.Text = "СКО";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(7, 74);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(16, 13);
            this.label11.TabIndex = 2;
            this.label11.Text = "М";
            // 
            // tbSKO
            // 
            this.tbSKO.Location = new System.Drawing.Point(90, 97);
            this.tbSKO.Name = "tbSKO";
            this.tbSKO.ReadOnly = true;
            this.tbSKO.Size = new System.Drawing.Size(120, 20);
            this.tbSKO.TabIndex = 1;
            // 
            // tbM
            // 
            this.tbM.Location = new System.Drawing.Point(89, 71);
            this.tbM.Name = "tbM";
            this.tbM.ReadOnly = true;
            this.tbM.Size = new System.Drawing.Size(120, 20);
            this.tbM.TabIndex = 0;
            // 
            // btnCalculate
            // 
            this.btnCalculate.Location = new System.Drawing.Point(12, 544);
            this.btnCalculate.Name = "btnCalculate";
            this.btnCalculate.Size = new System.Drawing.Size(220, 23);
            this.btnCalculate.TabIndex = 9;
            this.btnCalculate.Text = "Расчитать";
            this.btnCalculate.UseVisualStyleBackColor = true;
            this.btnCalculate.Click += new System.EventHandler(this.btnCalculate_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(12, 573);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(220, 23);
            this.button1.TabIndex = 10;
            this.button1.Text = "Сохранить";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(964, 606);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.btnCalculate);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.chart);
            this.Controls.Add(this.btnOpen);
            this.Controls.Add(this.tbFileName);
            this.Controls.Add(this.label1);
            this.MinimumSize = new System.Drawing.Size(980, 644);
            this.Name = "Form1";
            this.Text = "Анализ совпадений";
            ((System.ComponentModel.ISupportInitialize)(this.chart)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudChACorr)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudChANumber)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudChBCorr)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudChBNumber)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudTInt)).EndInit();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudN)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudWindow)).EndInit();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tbFileName;
        private System.Windows.Forms.Button btnOpen;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.NumericUpDown nudChACorr;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown nudChANumber;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.NumericUpDown nudChBCorr;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown nudChBNumber;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.NumericUpDown nudTInt;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.NumericUpDown nudN;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.NumericUpDown nudWindow;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.CheckBox cbShowWave;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox tbSKO;
        private System.Windows.Forms.TextBox tbM;
        private System.Windows.Forms.Button btnCalculate;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.TextBox tbCmpNum;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.TextBox tbTotalEvents;
        private System.Windows.Forms.TextBox tbTts;
        private System.Windows.Forms.Button button1;
    }
}

