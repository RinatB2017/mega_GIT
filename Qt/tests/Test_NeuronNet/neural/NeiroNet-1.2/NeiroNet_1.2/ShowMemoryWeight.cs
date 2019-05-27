using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace NeiroNet_12
{
    public partial class ShowMemoryWeight : Form
    {
        private Neiron neiron;
        public ShowMemoryWeight(Neiron neiron)
        {
            InitializeComponent();
            this.neiron = neiron;
        }

        private void ShowMemoryWeight_Load(object sender, EventArgs e)
        {
            dataGridView1.ColumnCount = neiron.weight.GetLength(0);
            dataGridView1.RowCount    = neiron.weight.GetLength(1);
            dataGridView1.DefaultCellStyle.ForeColor = Color.Green;      
            
            for (int n = 0; n < neiron.weight.GetLength(0); n++)
            {
                DataGridViewColumn column = dataGridView1.Columns[n];
                column.Width = 32;

                for (int m = 0; m < neiron.weight.GetLength(1); m++)
                {
                    int color = (int)((1 - neiron.weight[n, m]) * 255);
                    dataGridView1.Rows[m].Cells[n].Style.BackColor = Color.FromArgb(color, color, color);
                    dataGridView1.Rows[m].Cells[n].Value = neiron.weight[n, m];
                }
            }
        }
    }
}
