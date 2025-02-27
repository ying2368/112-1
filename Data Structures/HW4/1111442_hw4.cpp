#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

double getNumber(int &n,string input)
{
    int w = input.find(",", n);
    string s;
    if (w == -1)
    {
        s = input.substr(n);
        n = input.size();
    }
    else
    {
        s = input.substr(n, w - n);
        n = w;
    }
    int num = stoi(s);

    return num;
}

bool check(vector<int> request, vector<int> resources) //less
{
    for (size_t i = 0; i < request.size(); ++i)
        if (request[i] > resources[i])
            return false;

    return true;
}

void add(vector<int> &op,vector<int> op2)
{
    for (size_t i = 0; i < op.size(); ++i) 
        op[i] += op2[i];
}

void subtract(vector<int>& op, vector<int> op2)
{
    for (size_t i = 0; i < op.size(); ++i)
        op[i] -= op2[i];
}

struct Process {
    string id;
    vector<int> max;
    vector<int> Allocated;
    vector<int> Need;

    Process(string pid, const vector<int>& max_res) 
        : id(pid), max(max_res), Allocated(max_res.size(), 0), Need(max_res)  {}

    void output()
    {
        cout << id << endl << "Allocated: ";
        int n = Allocated.size();
        for (int i = 0; i < n; i++)
            cout << " " << Allocated[i];

        cout << endl << "Max: ";
        for (int i = 0; i < n; i++)
            cout << " " << max[i];

        cout << endl << "Need: ";
        for (int i = 0; i < n; i++)
            cout << " " << Need[i];

        cout << endl;
    }
};

int find(vector<Process> processes, string id)
{
    for (int i = 0; i < processes.size(); ++i)
        if (id == processes[i].id)
            return i;

    return -1;
}

bool Safety(vector<Process> processes, vector<int> Available)
{
    int size = processes.size();

    if (size == 0)
        return true;
    for (int i = 0; i < size; i++)
    {
        if (check(processes[i].Need, Available))
        {
            add(Available, processes[i].Allocated);
            processes.erase(processes.begin() + i);
            return Safety(processes, Available);
        }
    }

    return false;
}

int main(int argc, char* argv[]) 
{
    ifstream infile(argv[1]);
    string s;
    getline(infile, s);
    vector<int> Max;
    vector<int> Available;
    for (int i = 0; i < s.size(); i++)
        Available.push_back(getNumber(i, s));
    int num_resource = Available.size();
    for (int i = 0; i < num_resource; i++)
        Max.push_back(Available[i]);

    vector<Process> processes;
    string command;
    while (!infile.eof())
    {
        getline(infile, s);
        //cout << s << endl;

        int w1 = s.find(" ");
        int w2 = s.find(" ", w1 + 1);
        string pid = s.substr(0, w1);
        string command = s.substr(w1 + 1, w2 - w1 - 1);
        s = s.substr(w2 + 1);

        if (command == "in")
        {
            vector<int> max_resources;
            for (int i = 0; i < s.size(); i++)
                max_resources.push_back(getNumber(i, s));

            if (check(max_resources, Max)&&find(processes,pid)==-1)
            {
                int i = 0;
                for (; i < processes.size(); i++)
                    if (pid < processes[i].id)
                        break;

                processes.insert(processes.begin() + i, Process(pid, max_resources));
            }
            //else cout << "REJECT resource request. in" << endl;
        }
        else if (command == "request")
        {
            vector<int> request;
            for (int i = 0; i < s.size(); ++i)
                request.push_back(getNumber(i, s));

            int i = find(processes, pid);
            if (i != -1) {
                Process& process = processes[i];
                if (check(request,process.Need))
                {
                    subtract(Available, request);
                    add(process.Allocated, request);
                    subtract(process.Need, request);
                    if (!Safety(processes, Available))
                    {
                        add(Available, request);
                        add(process.Need, request);
                        subtract(process.Allocated, request);
                        //cout << "REJECT, system will be unsafe" << endl;
                    }
                }
                //else cout << "REJECT" << endl;
            }
        }
        else if (command == "want")
        {
            int i = find(processes, pid);
            if (i != -1) {
                Process& process = processes[i];
                if (check(process.Need,Available) && Safety(processes, Available))
                {
                    //cout << "Process " << pid << " has released all resources." << endl;
                    add(Available, process.Allocated);
                    processes.erase(processes.begin() + i);
                }
                //else cout << "REJECT, system will be unsafe" << endl;
            }
        }
    }

    for (const auto& process : processes) {
        cout << process.id << " " << process.Allocated[0];
        for (int i = 1;i < process.Allocated.size(); i++) 
            cout << "," << process.Allocated[i];
        cout << endl;
    }

    infile.close();
}