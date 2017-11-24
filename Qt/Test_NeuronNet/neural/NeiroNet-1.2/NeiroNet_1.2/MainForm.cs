﻿using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace NeiroNet_12
{
    public partial class MainForm : Form
    {
        private NeiroWeb	nw;
        private Point 		startP;
        private int[,] 		arr;

        private bool enableTraining;

        public MainForm()
        {
            InitializeComponent();
            enableTraining = false;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            NeiroGraphUtils.ClearImage(pictureBox1);
            nw = new NeiroWeb();
			/*
			if (nw == null)
			{
				MessageBox.Show("Error create NeiroWeb");
				return;
			}
			*/
            string[] items = nw.GetLiteras();
            if (items.Length > 0)
            {
                comboBox.Items.AddRange(items);
                comboBox.SelectedIndex = 0;
            }            
        }      

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
			if (nw == null)
			{
				MessageBox.Show("Error: nw == null");
				return;
			}
			nw.SaveState();
        }

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                Point endP = new Point(e.X,e.Y);
                Bitmap image = (Bitmap)pictureBox1.Image;
                using (Graphics g = Graphics.FromImage(image))
                {
                    g.DrawLine(new Pen(Color.BlueViolet), startP, endP);
                }
                pictureBox1.Image = image;
                startP = endP;                
            }
        }    

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {    
            startP = new Point(e.X, e.Y);
        }     

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void loadSymvolsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            addZipFile();
        }

        private void addZipFile()
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
            openFileDialog.Filter = "All files (*.txt)|*.txt";
            openFileDialog.FilterIndex = 2;
            openFileDialog.RestoreDirectory = true;
            openFileDialog.Multiselect = true;
            if (openFileDialog.ShowDialog() != DialogResult.OK) return;
            string[] ls = File.ReadAllLines(openFileDialog.FileName, Encoding.GetEncoding(1251));
            if (ls.Length == 0) return;
            var newItems = new List<string>();
            foreach (var i in comboBox.Items) if (!newItems.Contains((string)i)) newItems.Add((string)i);
            foreach (var i in             ls) if (!newItems.Contains((string)i)) newItems.Add((string)i);
            newItems.Sort();
            comboBox.Items.Clear();
            comboBox.Items.AddRange(newItems.ToArray());
            comboBox.SelectedIndex = 0;            
        }

        private void toMemoryToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string litera = comboBox.SelectedIndex >= 0 ? (string)comboBox.Items[comboBox.SelectedIndex] : comboBox.Text;
            if (litera.Length == 0)
            {
                MessageBox.Show("Не выбран ни один символ для занесения в память.");
                return;
            }
            nw.SetTraining(litera, arr);
            NeiroGraphUtils.ClearImage(pictureBox1);
            NeiroGraphUtils.ClearImage(pictureBox2);
            NeiroGraphUtils.ClearImage(pictureBox3);
            MessageBox.Show("Выбранный символ '" + litera + "' успешно добавлен в память сети");
        }

        private void clearToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NeiroGraphUtils.ClearImage(pictureBox1);
            NeiroGraphUtils.ClearImage(pictureBox2);
            NeiroGraphUtils.ClearImage(pictureBox3);
        }

        private void learnToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Learn();
        }

        private void drawFromComboBoxToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NeiroGraphUtils.ClearImage(pictureBox1);
            NeiroGraphUtils.ClearImage(pictureBox2);
            NeiroGraphUtils.ClearImage(pictureBox3);
            pictureBox1.Image = NeiroGraphUtils.DrawLitera(pictureBox1.Image, (string)comboBox.SelectedItem);
        }     

        private void Learn()
        {
            int[,] clipArr = NeiroGraphUtils.CutImageToArray((Bitmap)pictureBox1.Image, new Point(pictureBox1.Width, pictureBox1.Height));
            if (clipArr == null) return;
            arr = NeiroGraphUtils.LeadArray(clipArr, new int[NeiroWeb.neironInArrayWidth, NeiroWeb.neironInArrayHeight]);
            pictureBox2.Image = NeiroGraphUtils.GetBitmapFromArr(clipArr);
            pictureBox3.Image = NeiroGraphUtils.GetBitmapFromArr(arr);
            string s = nw.CheckLitera(arr);
            if (s == null) s = "null";
            DialogResult askResult = MessageBox.Show("Результат распознавания - " + s + " ?", "", MessageBoxButtons.YesNo);
            if ( askResult != DialogResult.Yes || !enableTraining || MessageBox.Show("Добавить этот образ в память нейрона '" + s + "'" , "", MessageBoxButtons.YesNo) != DialogResult.Yes) return;
            nw.SetTraining(s, arr);
            // очищаем рисунки
            NeiroGraphUtils.ClearImage(pictureBox1);
            NeiroGraphUtils.ClearImage(pictureBox2);
            NeiroGraphUtils.ClearImage(pictureBox3);       
        }

        // процедура помещает строку в список значений
        private void AddSymbolToList(string symbol)
        {
            if (symbol == null || symbol.Length == 0)
            {
                MessageBox.Show("Значение не может иметь длину 0 символов.");
                return;
            }
            comboBox.Items.Add(symbol);
            comboBox.SelectedIndex = comboBox.Items.Count - 1;
            MessageBox.Show("Сейчас значение '" + symbol + "' в списке, теперь можно научить нейросеть сеть его распознавать.");
        }     

        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter) AddSymbolToList(((TextBox)sender).Text);
        }   

        private void pictureBox1_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right) Learn();
        }

        private void enableTrainingToolStripMenuItem_Click(object sender, EventArgs e)
        {
            enableTraining = true;
            toMemoryToolStripMenuItem.Enabled = true;
            MessageBox.Show("Режим обучения включен, теперь нейросеть сможет запоминать введённый образ.");
        }

        private void button1_Click(object sender, EventArgs e)
        {
            AddSymbolToList(textBox1.Text);
        }
    }
}
