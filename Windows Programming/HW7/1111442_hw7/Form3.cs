using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _1111442_12_2
{
    public partial class Form3 : Form
    {

        float r1 = 10, g1 = 10, b1 = 10, a1 = 10;

        public float get_r() { return r1/10; }
        public float get_g() { return g1/10; }
        public float get_b() { return b1/10; }
        public float get_a() { return a1/10; }

        private void trackBar3_Scroll(object sender, EventArgs e)
        {
            b1 = trackBar3.Value;
            label7.Text = (b1/10).ToString();
        }

        private void trackBar4_Scroll(object sender, EventArgs e)
        {
            a1 = trackBar4.Value;
            label8.Text = (a1/10).ToString();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void trackBar2_Scroll(object sender, EventArgs e)
        {
            g1 = (float)trackBar2.Value;
            label6.Text = (g1/10).ToString();
        }

        public Form3()
        {
            InitializeComponent();
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            r1 = (float)trackBar1.Value;
            label5.Text = (r1/10).ToString();
        }
    }
}
