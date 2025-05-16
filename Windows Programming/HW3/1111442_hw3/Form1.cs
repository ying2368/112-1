using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _1111442_hw3
{
    public partial class Form1 : Form
    {
        List<Point> startPt = new List<Point>();
        List<Point> endPt = new List<Point>();
        List<Color> colorPt = new List<Color>();
        List<int> widthPt = new List<int>();
        List<bool> stylePt = new List<bool>();
        Point p1;
        Color lineColor = Color.Red;
        bool lineSolid = true;
        int lineWidth = 3;


        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_MouseDown(object sender, MouseEventArgs e)
        {
            p1 = e.Location;
        }

        private void Form1_MouseUp(object sender, MouseEventArgs e)
        {
            startPt.Add(p1);
            endPt.Add(e.Location);
            colorPt.Add(lineColor);
            widthPt.Add(lineWidth);
            stylePt.Add(lineSolid);
            Invalidate();
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            for (int i = 0; i < endPt.Count(); i++)
            {
                Pen pen1 = new Pen(colorPt[i], widthPt[i]);
                if (stylePt[i])
                    pen1.DashStyle = DashStyle.Solid;
                else
                    pen1.DashStyle = DashStyle.Dash;
                e.Graphics.DrawLine(pen1, startPt[i], endPt[i]);
            }

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            openFileDialog1.Filter = "線段檔(*.pnt)|*.pnt";
            saveFileDialog1.Filter = "線段檔(*.pnt)|*.pnt";
            redToolStripMenuItem.Checked = true;
            solidToolStripMenuItem.Checked = true;
            toolStripMenuItem4.Checked = true;
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                String s = saveFileDialog1.FileName;
                BinaryWriter outFile = new BinaryWriter(File.Open(s, FileMode.Create));
                outFile.Write(endPt.Count());
                for (int i = 0; i < endPt.Count(); i++)
                {
                    outFile.Write(startPt[i].X);
                    outFile.Write(startPt[i].Y);
                    outFile.Write(endPt[i].X);
                    outFile.Write(endPt[i].Y);
                }
                outFile.Close();
            }

        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                String s = openFileDialog1.FileName;
                if (!File.Exists(s)) 
                    return;
                BinaryReader inFile = new BinaryReader(File.Open(s, FileMode.Open));
                startPt.Clear();
                endPt.Clear();
                int n = inFile.ReadInt32();
                for (int i = 0; i < n; i++)
                {
                    startPt.Add(new Point(inFile.ReadInt32(), inFile.ReadInt32()));
                    endPt.Add(new Point(inFile.ReadInt32(), inFile.ReadInt32()));
                }
                Invalidate();
                inFile.Close();
            }

        }

        private void clearToolStripMenuItem_Click(object sender, EventArgs e)
        {
            startPt.Clear();
            endPt.Clear();
            Invalidate();

        }

        private void redToolStripMenuItem_Click(object sender, EventArgs e)
        {
            lineColor = Color.Red;
            redToolStripMenuItem.Checked = true;
            greenToolStripMenuItem.Checked = false;
            blueToolStripMenuItem.Checked = false;
        }

        private void greenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            lineColor = Color.Green;
            redToolStripMenuItem.Checked = false;
            greenToolStripMenuItem.Checked = true;
            blueToolStripMenuItem.Checked = false;
        }

        private void blueToolStripMenuItem_Click(object sender, EventArgs e)
        {
            lineColor = Color.Blue;
            redToolStripMenuItem.Checked = false;
            greenToolStripMenuItem.Checked = false;
            blueToolStripMenuItem.Checked = true;
        }

        private void solidToolStripMenuItem_Click(object sender, EventArgs e)
        {
            lineSolid = true;
            solidToolStripMenuItem.Checked = true;
            dashToolStripMenuItem.Checked = false;
        }

        private void dashToolStripMenuItem_Click(object sender, EventArgs e)
        {
            lineSolid = false;
            solidToolStripMenuItem.Checked = false;
            dashToolStripMenuItem.Checked = true;
        }

        private void toolStripMenuItem2_Click(object sender, EventArgs e)
        {
            lineWidth = 1;
            toolStripMenuItem2.Checked = true;
            toolStripMenuItem3.Checked = false;
            toolStripMenuItem4.Checked = false;
            toolStripMenuItem5.Checked = false;
            toolStripMenuItem6.Checked = false;
        }

        private void toolStripMenuItem3_Click(object sender, EventArgs e)
        {
            lineWidth = 2;
            toolStripMenuItem2.Checked = false;
            toolStripMenuItem3.Checked = true;
            toolStripMenuItem4.Checked = false;
            toolStripMenuItem5.Checked = false;
            toolStripMenuItem6.Checked = false;
        }

        private void toolStripMenuItem4_Click(object sender, EventArgs e)
        {
            lineWidth = 3;
            toolStripMenuItem2.Checked = false;
            toolStripMenuItem3.Checked = false;
            toolStripMenuItem4.Checked = true;
            toolStripMenuItem5.Checked = false;
            toolStripMenuItem6.Checked = false;
        }

        private void toolStripMenuItem5_Click(object sender, EventArgs e)
        {
            lineWidth = 4;
            toolStripMenuItem2.Checked = false;
            toolStripMenuItem3.Checked = false;
            toolStripMenuItem4.Checked = false;
            toolStripMenuItem5.Checked = true;
            toolStripMenuItem6.Checked = false;
        }

        private void toolStripMenuItem6_Click(object sender, EventArgs e)
        {
            lineWidth = 5;
            toolStripMenuItem2.Checked = false;
            toolStripMenuItem3.Checked = false;
            toolStripMenuItem4.Checked = false;
            toolStripMenuItem5.Checked = false;
            toolStripMenuItem6.Checked = true;
        }
    }
}
