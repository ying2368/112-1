#include<opencv2/opencv.hpp>
#include<iostream>
#include<fstream>
#include<vector>
#include<stack>
using namespace std;
using namespace cv;

struct Item
{
	int x, y, dir;

	Item(int i, int j, int dir)
		:x(i), y(j), dir(dir)
	{
	}
};

void path(vector<vector<int>> maze, stack<Item>& s)
{
	const int MAX_M = 200;
	const int MAX_P = 200;
	bool mark[MAX_M][MAX_P] = { 0 };
	s.push(Item(1, 1, 2));
	mark[0][1] = 1;
	mark[1][1] = 1;

	int col = maze.size();
	int row = maze[0].size();
	while (!s.empty())
	{
		Item temp = s.top();
		bool found = false;
		int x = temp.x, y = temp.y, dir = 0;
		while (dir < 4)
		{
			int g = x, h = y;
			if (dir == 0) h--;
			if (dir == 1) g++;
			if (dir == 2) h++;
			if (dir == 3) g--;
			if (!maze[h][g] && !mark[h][g])
			{
				mark[h][g] = 1;
				s.push(Item(g, h, dir));
				if ((g == row - 1 || h == col - 1 || g == 0 || h == 0) && !(g == 0, h == 1))
					return;
				found = true;
				break;
			}
			dir++;
		}

		if (!found)
			s.pop();
	}
}

int main(int argc, char* argv[])
{
	vector<vector<int>> maze;
	stack<Item> s;
	Mat image = imread(argv[1], CV_8UC1);
	for (int i = 5; i < image.rows; i += 10)
	{
		vector<int> rowData;
		for (int j = 5; j < image.cols; j += 10)
		{
			int pixelValue = image.at<uint8_t>(i, j);
			if (pixelValue == 0)
				rowData.push_back(1);
			else
				rowData.push_back(0);
		}
		maze.push_back(rowData);
	}

	path(maze, s);
	s.push(Item(1, 0, 2));
	Mat output(image.rows, image.cols, CV_8UC3);

	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++)
		{
			int c = image.at<uint8_t>(i, j);
			output.at<Vec3b>(i, j) = Vec3b(c, c, c);
		}

	while (!s.empty())
	{
		Item t = s.top();
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
			{
				int x = t.x * 10 + j;
				int y = t.y * 10 + i;
				output.at<Vec3b>(y, x) = Vec3b(0, 255, 0);
			}

		s.pop();
	}

	imwrite(argv[2], output);
}