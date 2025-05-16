using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _1111442_hw2
{
    public partial class Form1 : Form
    {
        private char[,] board = new char[3, 3];
        private bool gameOver = false;

        public Form1()
        {
            InitializeComponent();
            InitializeBoard();
        }

        private void InitializeBoard()
        {
            Graphics g1 = this.CreateGraphics();
            Pen p1 = new Pen(Color.FromArgb(255, 0, 0, 0), 3);
            
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    board[i, j] = ' ';
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            Graphics g1 = this.CreateGraphics();
            Pen p1 = new Pen(Color.FromArgb(255, 0, 0, 0), 3);
            Pen p2 = new Pen(Color.FromArgb(255, 0, 0, 255), 3);
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    e.Graphics.DrawRectangle(p1, i * 60, j * 60 + 25, 60, 60);

            for (int row = 0; row < 3; row++)
            {
                for (int col = 0; col < 3; col++)
                {
                    char symbol = board[row, col];
                    int rx = col * 60, ry = row * 60 + 25;

                    if (symbol == 'X')
                    {
                        g1.DrawLine(p2, rx + 10, ry + 10, rx + 50, ry + 50);
                        g1.DrawLine(p2, rx + 50, ry + 10, rx + 10, ry + 50);
                    }
                    else if (symbol == 'O')
                        g1.DrawEllipse(p2, rx + 10, ry + 10, 40, 40);
                }
            }


            if(gameOver)
            {
                Pen p3 = new Pen(Color.FromArgb(255, 255, 0, 0), 5);

                for (int row = 0; row < 3; row++)
                    if (board[row, 0] == board[row, 1] && board[row, 1] == board[row, 2] && board[row, 0] != ' ')
                        g1.DrawLine(p3, 15, row * 60 + 55, 165, row * 60 + 55);

                for (int col = 0; col < 3; col++)
                    if (board[0, col] == board[1, col] && board[1, col] == board[2, col] && board[0, col] != ' ')
                        g1.DrawLine(p3, col * 60 + 30, 15 + 25, col * 60 + 30, 165 + 25);

                if (board[0, 0] == board[1, 1] && board[1, 1] == board[2, 2] && board[1, 1] != ' ')
                    g1.DrawLine(p3, 15, 15 + 25, 165, 165 + 25);

                if (board[0, 2] ==  board[1, 1] && board[1, 1] == board[2, 0] && board[1, 1] != ' ')
                    g1.DrawLine(p3, 165, 15 + 25, 15, 165 + 25);
            }
            
        }

        private bool IsBoardFull()
        {
            for (int row = 0; row < 3; row++)
                for (int col = 0; col < 3; col++)
                    if (board[row, col] == ' ')
                        return false;

            return true;
        }

        private bool CheckWin(char c)
        {
            for (int row = 0; row < 3; row++)
                if (board[row, 0] == c && board[row, 1] == c && board[row, 2] == c)
                    return true;
                    
            for (int col = 0; col < 3; col++)
                if (board[0, col] == c && board[1, col] == c && board[2, col] == c)
                    return true;

            if (board[0, 0] == c && board[1, 1] == c && board[2, 2] == c)
                return true;

            if (board[0, 2] == c && board[1, 1] == c && board[2, 0] == c)
                return true;

            return false;
        }

        private void Form1_MouseClick(object sender, MouseEventArgs e)
        {
            if (gameOver)
            {
                label1.Text = "遊戲結束，請點擊菜單重新開始！";
                return;
            }

            int row = (e.Y - 25) / 60;
            int col = e.X / 60;

            if (row < 0 || row >= 3 || col < 0 || col >= 3 || board[row, col] != ' ')
                return;

            board[row, col] = 'O';
            Invalidate();

            if (CheckWin('O'))
            {
                label1.Text = "You win！";
                gameOver = true;
            }
            else if (IsBoardFull())
            {
                label1.Text = "Draw！";
                gameOver = true;
            }

            Invalidate();
            MakeComputerMove();
        }

        private void MakeComputerMove()
        {
            if (gameOver)
                return;

            Random rd = new Random();
            int row, col;

            do
            {
                row = rd.Next(3);
                col = rd.Next(3);
            } while (board[row, col] != ' ');

            board[row, col] = 'X';
            Invalidate();

            if (CheckWin('X'))
            {
                label1.Text = "You lose！";
                gameOver = true;
            }
            else if (IsBoardFull())
            {
                label1.Text = "Draw！";
                gameOver = true;
            }

            Invalidate();
        }

        private void restareToolStripMenuItem_Click(object sender, EventArgs e)
        {
            InitializeBoard();
            gameOver = false;
            Invalidate();
            label1.Text = "";
        }
    }
}
