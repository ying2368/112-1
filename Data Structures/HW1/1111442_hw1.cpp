#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <sstream>
#include <cmath>
#include <iomanip>

using namespace std;

double getNumber(int& n, string input)
{
	vector<int> v;
	long long num = 0, r = 1;
	while (n < input.size() && (input[n] >= '0' && input[n] <= '9'))
	{
		v.push_back(input[n] - '0');
		++n;
	}
	n--;

	for (int i = v.size() - 1; i >= 0; i--)
	{
		num += v[i] * r;
		r *= 10;
	}

	return num;
}

int isp(char x)
{
	switch (x)
	{
	case '(':	return 8;
	case '+':	return 3;
	case '-':	return 3;
	case '*':	return 2;
	case '/':	return 2;
	case '^':	return 1;
		//case '#':	return 8;
	default:	return 9;
	}
}

int icp(char x)
{
	switch (x)
	{
	case '(':	return 0;
	case '+':	return 3;
	case '-':	return 3;
	case '*':	return 2;
	case '/':	return 2;
	case '^':	return 1;
	default:	return 0;
	}
}

void calculate(vector<double>& number, char x)
{
	if (x == '-')
	{
		if (number.size() == 1)
		{
			number[0] = 0 - number[0];
			return;
		}
		else
			number[number.size() - 2] -= number[number.size() - 1];
	}
	else if (x == '+')
		number[number.size() - 2] += number[number.size() - 1];
	else if (x == '*')
		number[number.size() - 2] *= number[number.size() - 1];
	else if (x == '/')
		number[number.size() - 2] = (double)number[number.size() - 2] / number[number.size() - 1];
	else if (x == '^')
		number[number.size() - 2] = pow(number[number.size() - 2], number[number.size() - 1]);
	else
		cout << "error: " << x << endl;

	//cout << fixed << setprecision(5) << x << number[number.size() - 1] << " = " << number[number.size() - 2] << endl;
	number.pop_back();
}

int block(int n, int m, vector<vector<double>>& v)
{
	return n % v[v.size() - m].size();
}

void v_calculate(vector<vector<double>>& v, char x)
{
	if (v.size() == 1 && x == '-')
	{
		for (int i = 0; i < v[0].size(); i++)
			v[0][i] = -v[0][i];
		return;
	}

	int maxSize, d;
	if (v[v.size() - 1].size() >= v[v.size() - 2].size())
	{
		maxSize = v[v.size() - 1].size();
		d = maxSize - v[v.size() - 2].size();
		for (int i = 0; i < d; i++)
			v[v.size() - 2].push_back(v[v.size() - 2][block(i, 2, v)]);

	}
	if (v[v.size() - 1].size() < v[v.size() - 2].size())
	{
		maxSize = v[v.size() - 2].size();
		d = maxSize - v[v.size() - 1].size();
		for (int i = 0; i < d; i++)
			v[v.size() - 1].push_back(v[v.size() - 1][block(i, 1, v)]);
	}

	for (int i = 0; i < maxSize; i++)
	{
		if (x == '-')
			v[v.size() - 2][i] -= v[v.size() - 1][i];
		else if (x == '+')
			v[v.size() - 2][i] += v[v.size() - 1][i];
		else if (x == '*')
			v[v.size() - 2][i] *= v[v.size() - 1][i];
		else if (x == '/')
			v[v.size() - 2][block(i, 2, v)] = (double)v[v.size() - 2][block(i, 2, v)] / v[v.size() - 1][block(i, 1, v)];
		else
			cout << "error: " << x << endl;
	}

	v.pop_back();
}

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

string r(double n)
{
	stringstream ss;
	ss << fixed << setprecision(2) << rounding(n);
	string s = ss.str();
	while (s[s.size() - 1] != '.')
	{
		if (s[s.size() - 1] == '0')
			s.pop_back();
		else
			break;
	}
	if (s[s.size() - 1] == '.')
		s.pop_back();

	return s;
}

int main(int argc, char* argv[])
{
	string fileName = argv[1];
	//string fileName = "example_1.txt";
	ifstream infile(fileName);
	string input;
	char x;

	while (getline(infile, input))
	{
		//cout << input << endl;
		vector<vector<double>> v;
		stack<char> v_op;
		vector<double> number, v1;
		stack<char> op;
		bool is_vector = false;

		for (int i = 0; i < input.size(); i++)
		{
			x = input[i];
			//cout << x << " ";
			if (x == ' ')
				continue;
			if (x == '[')
				is_vector = true;
			else if (x >= '0' && x <= '9') {
				number.push_back(getNumber(i, input));
			}
			else if (x == ',')
			{
				for (; !op.empty(); op.pop())
					calculate(number, op.top());
				v1.push_back(number[0]);
				number.clear();
			}
			else if (x == ']')
			{
				is_vector = false;
				for (; !op.empty(); op.pop())
					calculate(number, op.top());
				v1.push_back(number[0]);
				number.clear();
				v.push_back(v1);
				v1.clear();
			}
			else if (x == ')')
			{
				if (is_vector)
				{
					for (; op.top() != '('; op.pop())
						calculate(number, op.top());
					op.pop();
				}
				else
				{
					for (; v_op.top() != '('; v_op.pop())
						v_calculate(v, v_op.top());
					v_op.pop();
				}
			}
			else
			{
				if (is_vector)
				{
					if (x == '-' && input[i - 1] == '(')
						number.push_back(0);

					for (; !op.empty(); op.pop())
					{
						if (isp(op.top()) <= icp(x))
							calculate(number, op.top());
						else
							break;
					}
					op.push(x);
				}
				else
				{
					for (; !v_op.empty(); v_op.pop())
					{
						if (isp(v_op.top()) <= icp(x))
							v_calculate(v, v_op.top());
						else
							break;
					}
					v_op.push(x);
				}
			}
		}

		for (; !v_op.empty(); v_op.pop())
			v_calculate(v, v_op.top());

		if (!v.empty())
		{
			cout << "[";
			for (int i = 0; i < v[0].size() - 1; i++)
				cout << r(v[0][i]) << ",";
			cout << r(v[0][v[0].size() - 1]) << "]" << endl;
		}

	}

	infile.close();
}
