using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _1111441_hw1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            int r, g, b;
            Random rd = new Random();
            Brush b1;
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    r = rd.Next(256); //產生0~255的亂數
                    g = rd.Next(256);
                    b = rd.Next(256);
                    Rectangle rect = new Rectangle(i * 50, j * 50, 50, 50);
                    b1 = new SolidBrush(Color.FromArgb(r, g, b));
                    e.Graphics.FillRectangle(b1, rect);
                    e.Graphics.DrawRectangle(Pens.Black, rect);
                }
            }
        }

        private void Form1_MouseClick(object sender, MouseEventArgs e)
        {
            int r, g, b;
            int x = e.X, y = e.Y; // 
            Random rd = new Random();  //使用亂數類別
            Brush b1;
            Graphics g1 = this.CreateGraphics();
            r = rd.Next(256); //產生0~255的亂數
            g = rd.Next(256);
            b = rd.Next(256);
            if(x < 150 && y < 150)
            {
                Rectangle rect = new Rectangle(x / 50 * 50, y / 50 * 50, 50, 50);
                b1 = new SolidBrush(Color.FromArgb(r, g, b));
                g1.FillRectangle(b1, rect);
                g1.DrawRectangle(Pens.Black, rect);
            }
        }
    }
}
