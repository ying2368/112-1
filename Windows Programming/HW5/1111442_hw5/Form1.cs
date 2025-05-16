using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        List<Image> images = new List<Image>
            (new Image[] {new Bitmap(Properties.Resources._8_0), new Bitmap(Properties.Resources._8_1),
                        new Bitmap(Properties.Resources._8_2), new Bitmap(Properties.Resources._8_3),
                        new Bitmap(Properties.Resources._8_4), new Bitmap(Properties.Resources._8_5),
                        new Bitmap(Properties.Resources._8_6), new Bitmap(Properties.Resources._8_7),
                        new Bitmap(Properties.Resources._8_8) });

        List<Image> ans = new List<Image>();
        bool[] correct = {false, false, false, false, false, false, false, false,
                            false, false, false, false, false, false, false, false };
        int[] choice = { -1, -1 };
        int t = 0, c = 0;
        bool move = true;

        public Form1()
        {
            InitializeComponent();
        }

        private bool playing()
        {
            for (int i = 0; i < 16; i++)
                if (correct[i] == false)
                    return true;

            return false;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (playing())
            {
                t++;
                toolStripStatusLabel1.Text = t.ToString();
            }
            else
            {
                timer1.Stop();
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            timer1.Interval = 1000;
            timer1.Start();

            int[] count = new int[9];
            Random rd = new Random();
            int r = 0;

            for (int i = 0; i < 16; i++)
            {
                do
                {
                    r = rd.Next(1, 9);
                    count[r]++;
                } while (count[r] > 2);
                ans.Add(images[r]);
            }

            Invalidate();
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            Pen p1 = new Pen(Color.FromArgb(255, 0, 0, 0), 2);
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                {
                    if (correct[i * 4 + j])
                        e.Graphics.DrawImage(ans[i * 4 + j], 60 * i + 1, 60 * j + 25 + 1, 58, 58);
                    else
                        e.Graphics.DrawImage(images[0], 60 * i, 60 * j + 25, 60, 60);
                    e.Graphics.DrawRectangle(p1, 60 * i, 60 * j + 25, 60, 60);
                }
        }

        private void restartToolStripMenuItem_Click(object sender, EventArgs e)
        {
            t = 0;
            timer1.Start();
            for (int i = 0; i < 16; i++)
                correct[i] = false;
            Invalidate();
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            Invalidate();
            timer2.Stop();
            move = true;
        }

        private void Form1_MouseClick(object sender, MouseEventArgs e)
        {
            int j = (e.Y - 25) / 60;
            int i = e.X / 60;
            Graphics g1 = this.CreateGraphics();

            if (playing() && move && i < 4 && j < 4 && i * 4 + j != choice[1])
            {
                choice[c] = i * 4 + j;
                g1.DrawImage(ans[i * 4 + j], 60 * i+1, 60 * j + 25+1, 58, 58);
                c++;

                if (c == 2)
                {
                    if (ans[choice[0]] != ans[choice[1]])
                    {
                        timer2.Interval = 1000;
                        timer2.Start();
                        move = false;
                    }
                    else
                    {
                        correct[choice[0]] = true;
                        correct[choice[1]] = true;
                    }
                    c = 0;
                    choice[1] = -1;
                }
            }
        }
    }
}
