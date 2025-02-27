#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

struct Node
{
	string data;
	Node *left;
	Node *right;

	Node(string d = "") 
		:data(d),left(nullptr),right(nullptr){}
};

vector<string> get_data(string s)
{
	vector<string> v;
	while (s.size() > 1)
	{
		int w = s.find(')');
		v.push_back(s.substr(1, w - 1));
		
		s = s.substr(w + 1);
	}

	return v;
}

int where(vector<string> v, string s)
{
	for (int i = 0; i < v.size(); i++)
		if (v[i] == s)
			return i;

	return 0;
}

Node* VLR(vector<string> inorder, vector<string> preorder)   //inorder preorder
{
	Node *start = new Node(preorder[0]);

	for (int i = 1; i < preorder.size(); i++)
	{
		Node* ptr = start;
		Node* newNode = new Node(preorder[i]);
		int w = where(inorder, preorder[i]);
		while (ptr)
		{
			int v = where(inorder, ptr->data);
			if (w < v)
			{
				if (!ptr->left)
				{
					ptr->left = newNode;
					break;
				}
				ptr = ptr->left;
			}
			if (w > v)
			{
				if (!ptr->right)
				{
					ptr->right = newNode;
					break;
				}
				ptr = ptr->right;
			}

			v = where(inorder, ptr->data);
		} 

	}

	return start;
}

Node* LRV(vector<string> inorder, vector<string> postorder)  //inorder postorder
{
	Node* start = new Node(postorder[postorder.size() - 1]);

	for (int i = postorder.size() - 2; i >= 0; i--)
	{
		Node* ptr = start;
		Node* newNode = new Node(postorder[i]);
		int w = where(inorder, postorder[i]);
		while (ptr)
		{
			int v = where(inorder, ptr->data);
			if (w < v)
			{
				if (!ptr->left)
				{
					ptr->left = newNode;
					break;
				}
				ptr = ptr->left;
			}
			if (w > v)
			{
				if (!ptr->right)
				{
					ptr->right = newNode;
					break;
				}
				ptr = ptr->right;
			}

			v = where(inorder, ptr->data);
		}

	}

	return start;
}

void output(vector<Node*> nq, vector<int> count)
{
	if (nq.size() == 0)
		return;

	Node* ptr = nq[0];
	if (ptr)
	{
		cout << ptr->data << ":" << count[0] << endl;
		if (ptr->left)
		{
			nq.push_back(ptr->left);
			count.push_back(count[0] * 2);
		}
		if (ptr->right)
		{
			nq.push_back(ptr->right);
			count.push_back(count[0] * 2 + 1);
		}
	}

	nq.erase(nq.begin());
	count.erase(count.begin());
	output(nq, count);
}

int main(int argc, char* argv[])
{
	ifstream infile(argv[1]);
	string s1,s2;

	while (getline(infile, s1))
	{
		getline(infile, s2);
		Node *levelorder = new Node;

		if (s1[1] == 'r')
		{
			vector<string> inorder = get_data(s2.substr(8, s2.size() - 8));
			vector<string> preorder = get_data(s1.substr(9, s1.size() - 9));
			levelorder = VLR(inorder, preorder);
		}
		if (s2[1] == 'r')
		{
			vector<string> inorder = get_data(s1.substr(8, s1.size() - 8));
			vector<string> preorder = get_data(s2.substr(9, s2.size() - 9));
			levelorder = VLR(inorder, preorder);
		}
		if (s1[1] == 'o')
		{
			vector<string> inorder = get_data(s2.substr(8, s2.size() - 8));
			vector<string> postorder = get_data(s1.substr(10, s1.size() - 10));
			levelorder = LRV(inorder, postorder);
		}
		if (s2[1] == 'o')
		{
			vector<string> inorder = get_data(s1.substr(8, s1.size() - 8));
			vector<string> postorder = get_data(s2.substr(10, s2.size() - 10));
			levelorder = LRV(inorder, postorder);
		}

		ofstream outfile(argv[2]);
		vector<Node*> nq;
		vector<int> count;
		nq.push_back(levelorder);
		count.push_back(1);
		output(nq, count);
	}
}
