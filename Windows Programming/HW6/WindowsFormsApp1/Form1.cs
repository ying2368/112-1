using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        Image[] BackImage = {new Bitmap(Properties.Resources.Hydrangeas), new Bitmap(Properties.Resources.Penguins),
                        new Bitmap(Properties.Resources.Tulips) };
        Image[] FruitImage = {new Bitmap(Properties.Resources.Banana), new Bitmap(Properties.Resources.StawBerry),
                        new Bitmap(Properties.Resources.Tomato) };
        int time = 120, count = 0, backgroundIndex = 0;
        int three = 0;
        bool playing = true;
        int bpx;
        Random random = new Random();
        private PictureBox bowl;

        public Form1()
        {
            InitializeComponent();
            DoubleBuffered = true; //開啟雙暫存區以避免螢幕閃爍
            Width = 600;
            Height = 480;
            bowl = new PictureBox
            {
                Size = new Size(60, 40),
                Image = Properties.Resources.Bowl,
                SizeMode = PictureBoxSizeMode.StretchImage
            };

            bowl.Location = new Point((Width - bowl.Width) / 2, Height - bowl.Height - 100);

            Controls.Add(bowl);
        }

        private void DropFruit()
        {
            PictureBox fruit = new PictureBox
            {
                Size = new Size(50, 50),
                Location = new Point(random.Next(Width - 50), 0),
                Image = GetRandomFruit(),
                SizeMode = PictureBoxSizeMode.StretchImage,
                Tag = "fruit"
            };

            Controls.Add(fruit);
            three++;

            // 移動水果
            Timer fruitTimer = new Timer();
            fruitTimer.Interval = 50; // 每0.05秒移動一次
            fruitTimer.Tick += (sender, e) =>
            {
                fruit.Top += 5;

                if (fruit.Bounds.IntersectsWith(bowl.Bounds))
                {
                    // 接到水果
                    Controls.Remove(fruit);
                    fruitTimer.Stop();
                    count++;
                    three--;
                    label5.Text = count.ToString(); // 在這裡更新接到的水果數
                }
                else if (fruit.Top >= Height)
                {
                    // 水果掉到底部未被接到
                    Controls.Remove(fruit);
                    fruitTimer.Stop();
                }
            };
            fruitTimer.Start();
        }

        private Image GetRandomFruit()
        {
            int randomFruitIndex = random.Next(1, 4);
            switch (randomFruitIndex)
            {
                case 1:
                    return Properties.Resources.StawBerry;
                case 2:
                    return Properties.Resources.Banana;
                default:
                    return Properties.Resources.Tomato;
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (playing)
            {
                if(time==0)
                    timer1.Stop();
                else
                    time--;

                label3.Text = time.ToString();
                if (three < 3)
                    DropFruit();
            }
        }

        private void Form1_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.X <= 600)
                bpx = e.X;
            bowl.Left = e.X - (bowl.Width / 2);
            Invalidate();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            timer1.Interval = 1000;
            timer1.Start();
            timer2.Interval = 10000;
            timer2.Start();
            DropFruit();
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.DrawImage(BackImage[backgroundIndex%3], 0, 25, 600, 460);
            //e.Graphics.DrawImage(new Bitmap(Properties.Resources.Bowl), bpx, 350, 60, 30);
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            backgroundIndex++;
        }

        private void restartToolStripMenuItem_Click(object sender, EventArgs e)
        {
            timer1.Start();
            DropFruit();
            playing = true;
            timer1.Start();
            timer2.Start();
            count = 0;
        }
    }
}
