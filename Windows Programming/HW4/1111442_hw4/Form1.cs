using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _1111442_hw4
{
    public partial class Form1 : Form
    {
        Color color = Color.Red;
        Point p;
        int speed = 4; // 增加的速度
        int num = 0; //時間
        float a; //角度
        bool playing = true;
        float bx, by;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            timer1.Interval = 1000;
            timer1.Start();
            p.X = 125;
            p.Y = 300;
            bx = 125;
            by = 50;
            Random rd = new Random();  
            a = 30 + rd.Next(120);  
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (playing)
            {
                num++;
                toolStripStatusLabel1.Text = num.ToString();
                toolStripStatusLabel2.Text = "Playing!";
            }
            else
            {
                timer1.Stop();
                toolStripStatusLabel2.Text = "Game Over!";
            }
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            Graphics g1 = this.CreateGraphics();
            Pen p1 = new Pen(Color.FromArgb(255, 0, 0, 0), 1);
            g1.DrawRectangle(p1, 0, 50, 250, 250);

            Brush myBrush = new SolidBrush(Color.Red); // 紅色的單色塗刷
            Brush myBrush2 = new SolidBrush(color); // 紅色的單色塗刷
            e.Graphics.FillRectangle(myBrush, p.X-10, p.Y, 30, 15);
            e.Graphics.FillEllipse(myBrush2, bx, by, 15, 15); // 填滿一個橢圓形
        }

        private void redToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            color = Color.Red;
            redToolStripMenuItem1.Checked = true;
            greenToolStripMenuItem1.Checked = false;
            blueToolStripMenuItem.Checked = false;
        }

        private void greenToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            color = Color.Green;
            redToolStripMenuItem1.Checked = false;
            greenToolStripMenuItem1.Checked = true;
            blueToolStripMenuItem.Checked = false;
        }

        private void blueToolStripMenuItem_Click(object sender, EventArgs e)
        {
            color = Color.Blue;
            redToolStripMenuItem1.Checked = false;
            greenToolStripMenuItem1.Checked = false;
            blueToolStripMenuItem.Checked = true;
        }

        private void restartToolStripMenuItem_Click(object sender, EventArgs e)
        {
            timer1.Start();
            playing = true;
            num = 0;
            bx = 125;
            by = 50;
            Random rd = new Random();
            a = 180 + rd.Next(180);
        }
        
        private void timer2_Tick(object sender, EventArgs e)
        {
            speed = (num / 5) * 2 + 4;
            
            if (by > 285 || by < 50)
                a = 360 - a;

            if (bx < 10 || bx > 235)
            {
                if (a > 180)
                    a = 180 + (360 - a);
                else
                    a = 180 - a;
            }

            if (by > 285 && (bx > p.X+15 || bx < p.X-15))
                playing = false;
            else
            { 
                bx += (float)Math.Cos(a * Math.PI / 180.0) * speed;
                by += (float)Math.Sin(a * Math.PI / 180.0) * speed;

                Invalidate(); // 要求重畫
            }
        }

        private void Form1_MouseMove(object sender, MouseEventArgs e)
        {
            if (playing)
            {
                if (e.X <= 240)
                    p.X = e.X;
            }
        }
    }
}
