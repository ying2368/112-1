#define _USE_MATH_DEFINES
#define EPS 1E-3 
#include <iostream>
#include <fstream>
#include <cmath>
#include<string>
#include <vector>
#include <iomanip>
using namespace std;

double rounding(double n)
{
	long long a = n * 1000;
	int d = a % 10;

	if (a > 0)
	{
		if (d > 4)
			a = (a + 10) / 10;
		else
			a = a / 10;
	}
	else
	{
		if (d < -4)
			a = (a - 10) / 10;
		else
			a = a / 10;
	}

	return (double)a / 100;
}

double det(vector<double> v)
{
	return v[0] * v[4] * v[8] + v[1] * v[5] * v[6] + v[2] * v[3] * v[7] - v[2] * v[4] * v[6] - v[0] * v[5] * v[7] - v[1] * v[3] * v[8];
}

struct Matrix
{
	double m[4][4] = { 1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1 };

	Matrix(int i)
	{
		m[0][0] = m[1][1] = m[2][2] = m[3][3] = i;
	}
	Matrix(double t[4][4])
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				m[i][j] = t[i][j];
	}

	void RT(double x, double y, double z)
	{
		double t1[4][4] = { 1, 0, 0, -x,
						   0, 1, 0, -y,
						   0, 0, 1, -z,
						   0, 0, 0, 1 };

		M(Matrix(t1));
	}

	void S(ifstream& input, int n)
	{
		string s;
		for (int i = 0; i < n; i++)
		{
			double t1[4][4] = { 1, 0, 0, 0,
							   0, 1, 0, 0,
							   0, 0, 1, 0,
							   0, 0, 0, 1 };
			input >> s;
			if (s == "#T")
				input >> t1[0][3] >> t1[1][3] >> t1[2][3];
			else if (s == "#Pyz")
				t1[0][0] = 0;
			else if (s == "#Pxz")
				t1[1][1] = 0;
			else if (s == "#Pxy")
				t1[2][2] = 0;
			else if (s == "#S")
			{
				input >> t1[0][3] >> t1[1][3] >> t1[2][3] >> t1[0][0] >> t1[1][1] >> t1[2][2];
				RT(t1[0][3], t1[1][3], t1[2][3]);
			}
			else if (s == "#Rx")
			{
				double theta;
				input >> t1[0][3] >> t1[1][3] >> t1[2][3] >> theta;
				RT(t1[0][3], t1[1][3], t1[2][3]);
				theta = double((theta * M_PI) / 180);
				t1[1][1] = cos(theta);
				t1[1][2] = -sin(theta);
				t1[2][1] = sin(theta);
				t1[2][2] = cos(theta);
			}
			else if (s == "#Ry")
			{
				double theta;
				input >> t1[0][3] >> t1[1][3] >> t1[2][3] >> theta;
				RT(t1[0][3], t1[1][3], t1[2][3]);
				theta = double((theta * M_PI) / 180);
				t1[0][0] = cos(theta);
				t1[0][2] = sin(theta);
				t1[2][0] = -sin(theta);
				t1[2][2] = cos(theta);
			}
			else if (s == "#Rz")
			{
				double theta;
				input >> t1[0][3] >> t1[1][3] >> t1[2][3] >> theta;
				RT(t1[0][3], t1[1][3], t1[2][3]);
				theta = double((theta * M_PI) / 180);
				t1[0][0] = cos(theta);
				t1[0][1] = -sin(theta);
				t1[1][0] = sin(theta);
				t1[1][1] = cos(theta);
			}
			else if (s == "#Hx")
			{
				input >> t1[0][3] >> t1[1][3] >> t1[2][3] >> t1[0][1] >> t1[0][2];
				RT(t1[0][3], t1[1][3], t1[2][3]);
			}
			else if (s == "#Hy")
			{
				input >> t1[0][3] >> t1[1][3] >> t1[2][3] >> t1[1][0] >> t1[1][2];
				RT(t1[0][3], t1[1][3], t1[2][3]);
			}
			else if (s == "#Hz")
			{
				input >> t1[0][3] >> t1[1][3] >> t1[2][3] >> t1[2][0] >> t1[2][1];
				RT(t1[0][3], t1[1][3], t1[2][3]);
			}
			else if (s == "#M")
			{
				for (int i = 0; i < 4; i++)
					for (int j = 0; j < 4; j++)
						input >> t1[i][j];
			}
			else
				cout << "error: " << s << "!" << endl;

			M(Matrix(t1));
		}
	}

	void M(Matrix v)   //¯x°}­¼ªk
	{
		Matrix result(0);
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				for (int k = 0; k < 4; ++k)
					result.m[i][j] += v.m[i][k] * m[k][j];

		*this = result;
	}

	void output(ofstream& output)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (j == 3)
					break;
				output << fixed << setprecision(2) << rounding(m[i][j]) << " ";
			}
			output << rounding(m[i][3]) << endl;
		}
	}
	void output()
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (j == 3)
					break;
				cout << fixed << setprecision(2) << rounding(m[i][j]) << " ";
			}
			cout << rounding(m[i][3]) << endl;
		}
	}
};

struct Matrix3
{
	vector<double> v11;
	vector<double> v12;
	vector<double> v13;
	vector<double> v14;
	vector<double> a;

	Matrix3(Matrix m)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				double d = m.m[i][j];
				if (i == 0)
				{
					a.push_back(d);
					continue;
				}
				else if (j == 0)
				{
					v12.push_back(d);
					v13.push_back(d);
					v14.push_back(d);
				}
				else if (j == 1)
				{
					v11.push_back(d);
					v13.push_back(d);
					v14.push_back(d);
				}
				else if (j == 2)
				{
					v11.push_back(d);
					v12.push_back(d);
					v14.push_back(d);
				}
				else if (j == 3)
				{
					v11.push_back(d);
					v12.push_back(d);
					v13.push_back(d);
				}
			}
	}
	Matrix3(Matrix t, double a[4])
	{
		double m1[3][3] = { a[0] - t.m[0][3], t.m[0][1], t.m[0][2],
							a[1] - t.m[1][3], t.m[1][1], t.m[1][2],
							a[2] - t.m[2][3], t.m[2][1], t.m[2][2], };
		double m2[3][3] = { t.m[0][0], a[0] - t.m[0][3], t.m[0][2],
							 t.m[1][0], a[1] - t.m[1][3], t.m[1][2],
							 t.m[2][0], a[2] - t.m[2][3], t.m[2][2], };
		double m3[3][3] = { t.m[0][0], t.m[0][1], a[0] - t.m[0][3],
							 t.m[1][0], t.m[1][1], a[1] - t.m[1][3],
							 t.m[2][0], t.m[2][1], a[2] - t.m[2][3] };

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				v11.push_back(m1[i][j]);
				v12.push_back(m2[i][j]);
				v13.push_back(m3[i][j]);
			}
		}
	}

	double get_det()   //4*4
	{
		return a[0] * det(v11) - a[1] * det(v12) + a[2] * det(v13) - a[3] * det(v14);
	}
};

struct Point
{
	double x, y, z, value;

	Point()
		:x(0), y(0), z(0), value(0)
	{
	}
	Point(double x, double y, double z, double v)
		:x(x), y(y), z(z), value(v)
	{
	}
	Point(double p[4])
		:x(p[0]), y(p[1]), z(p[2]), value(p[3])
	{
	}
};

Point inverse(Matrix t, double x, double y, double z)
{
	double u[4] = { x, y, z, 1 };
	Matrix3 A(t);
	Matrix3 m(t, u);
	double dett = A.get_det();

	return Point(det(m.v11) / dett, det(m.v12) / dett, det(m.v13) / dett, 0);
}

int get_value(vector<vector<vector<double>>> slice, int x, int y, int z)
{
	if (x >= 0 && x < slice[0].size() && y >= 0 && y < slice[0][0].size() && z >= 0 && z < slice.size())
		return slice[z][x][y];

	return 0;
}

Point get_pa(Point p1, Point p2, double x)
{
	double r = double((p1.x - x) / (p1.x - p2.x));
	return Point(x, p1.y, p1.z, p1.value * (1 - r) + p2.value * r);
}

Point get_pb(Point p1, Point p2, double z)
{
	double r = double((p1.z - z) / (p1.z - p2.z));
	return Point(p1.x, p1.y, z, p1.value * (1 - r) + p2.value * r);
}

Point get_pc(Point p1, Point p2, double y)
{
	double r = double((p1.y - y) / (p1.y - p2.y));
	return Point(p1.x, y, p1.z, p1.value * (1 - r) + p2.value * r);
}

int Interpolation(vector<vector<vector<double>>> s, Point traget)
{
	int x = traget.x / 1, y = traget.y / 1, z = traget.z / 1;
	if (traget.x < 0)
		x--;
	if (traget.y < 0)
		y--;
	if (traget.z < 0)
		z--;

	Point p0(x, y, z, get_value(s, x, y, z));
	Point p1(x, y, z + 1, get_value(s, x, y, z + 1));
	Point p2(x, y + 1, z, get_value(s, x, y + 1, z));
	Point p3(x, y + 1, z + 1, get_value(s, x, y + 1, z + 1));
	Point p4(x + 1, y, z, get_value(s, x + 1, y, z));
	Point p5(x + 1, y, z + 1, get_value(s, x + 1, y, z + 1));
	Point p6(x + 1, y + 1, z, get_value(s, x + 1, y + 1, z));
	Point p7(x + 1, y + 1, z + 1, get_value(s, x + 1, y + 1, z + 1));

	Point a0 = get_pa(p0, p4, traget.x);
	Point a1 = get_pa(p1, p5, traget.x);
	Point a2 = get_pa(p2, p6, traget.x);
	Point a3 = get_pa(p3, p7, traget.x);
	Point b0 = get_pb(a0, a1, traget.z);
	Point b1 = get_pb(a2, a3, traget.z);
	Point c = get_pc(b0, b1, traget.y);

	return c.value;
}

int main(int argc, char* argv[])
{
	ifstream input1(argv[1]);
	ofstream output1(argv[2]);
	ifstream input2(argv[3]);
	ofstream output2(argv[4]);

	double iv[4][4] = { 0, 0, 0, 1,
					0, 0, 0, 1,
					0, 0, 0, 1,
					0, 0, 0, 1 };
	double ux, uy, uz;
	int n;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++)
			input1 >> iv[i][j];
	input1 >> ux >> uy >> uz;
	input1 >> n;
	Matrix t(1);
	t.S(input1, n);
	t.output(output1);

	//b
	Matrix ou(0);
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
			for (int k = 0; k < 4; ++k)
				ou.m[i][j] += t.m[j][k] * iv[i][k];

		for (int j = 0; j < 3; ++j)
			output1 << rounding(ou.m[i][j]) << " ";
		output1 << rounding(ou.m[i][3]) << endl;
	}

	//c
	Matrix3 a(iv);
	Matrix3 b(ou);
	Matrix3 c(t);
	double r = abs(b.get_det() / a.get_det());
	double dett = c.get_det();
	output1 << rounding(r) << " " << rounding(dett) << endl;

	if (r < EPS && dett < EPS)
		output1 << "zeros" << endl;
	else if (abs(r - dett) < EPS)
		output1 << "r==det(T)" << endl;
	else if (abs(r + dett) < EPS)
		output1 << "r==-det(T)" << endl;
	else
		output1 << "others" << endl;

	//d
	if (abs(dett) < EPS)
		output1 << "NaN" << endl;
	else
	{
		Point ov =  inverse(t, ux, uy, uz);
		output1 << rounding(ov.x) << " " << rounding(ov.y) << " " << rounding(ov.z) << " " << 1.00 << endl;
	}

	input1.close();
	output1.close();

	cout << "==================================" << endl;
	int l, w, h;
	double d;
	input2 >> l >> w >> h;

	vector<vector<vector<double>>> slice;
	for (int i = 0; i < h; i++)
	{
		vector<vector<double>> s1;
		for (int j = 0; j < l; j++)
		{
			vector<double> s2;
			for (int k = 0; k < w; k++)
			{
				input2 >> d;
				s2.push_back(d);
			}
			s1.push_back(s2);
		}
		slice.push_back(s1);
	}

	input2 >> n;
	Matrix t2(1);
	t2.S(input2, n);

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < l; j++)
		{
			for (int k = 0; k < w - 1; k++)
			{
				Point op = inverse(t2, j, k, i);
				output2 << Interpolation(slice, op) << " ";
			}
			Point op = inverse(t2, j, w - 1, i);
			output2 << Interpolation(slice, op) << endl;
		}
	}

	input2.close();
	output2.close();
}
