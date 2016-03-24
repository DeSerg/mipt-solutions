#include <string>
#include <vector>
#include <stack>
#include <stdexcept>
#include <stdio.h>
#include <iostream>

using namespace std;


class Graph
{
    //fields
    struct StackNode
    {
        StackNode(int inp_vert_ind, int inp_connect_ind):
            vert_ind(inp_vert_ind), connect_ind(inp_connect_ind){}

        StackNode(const StackNode &stack_node):
            vert_ind(stack_node.vert_ind), connect_ind(stack_node.connect_ind){}

        int vert_ind;
        int connect_ind;

        StackNode operator = (StackNode &right)
        {
            vert_ind = right.vert_ind;
            connect_ind = right.connect_ind;
            return *this;
        }
    };

    vector<vector<int> > List_;

public:

    //constructors
    Graph(){}

    Graph(int vert_num) : List_(vert_num){}

    Graph(Graph &inp_graph) :
        List_(inp_graph.List_.begin(), inp_graph.List_.end()) {}

    Graph(vector<vector<int> > inpList) :
        List_(inpList.begin(), inpList.end()) {}



    void AddEdge(int v1, int v2)
    {
        if (v1 >= (int)List_.size()) List_.resize(v1 + 1);
        if (v2 >= (int)List_.size()) List_.resize(v2 + 1);

        bool exist = false;
        for (int i = 0; i < (int)List_[v1].size(); i++)
        {
            if (List_[v1][i] == v2)
            {
                exist = true;
                break;
            }
        }

        if (!exist)
            List_[v1].push_back(v2);
    }

    //DFS without recoursion to make topological sort
    bool DFS(vector<int> &top_sorting)
    {
        vector<int> visited(List_.size(), 0);

        size_t index;
        for (index = 0; index < List_.size(); index++)
        {
            if (visited[index]) continue;

            stack<StackNode> dfs_stack;
            visited[index] = 1;
            StackNode node(index, 0);
            dfs_stack.push(node);

            while (!dfs_stack.empty())
            {
                node = dfs_stack.top();
                int v_ind = node.vert_ind;
                int c_ind = node.connect_ind++;

                dfs_stack.pop();
                dfs_stack.push(node);

                visited[v_ind] = 1;

                if (c_ind == (int)List_[v_ind].size())
                {
                    visited[v_ind] = 2;
                    top_sorting.push_back(v_ind);
                    dfs_stack.pop();
                }
                else
                {
                    int forward_v_ind = List_[v_ind][c_ind];
                    if (visited[forward_v_ind] == 1) return false;
                    if (!visited[forward_v_ind])
                    {
                        StackNode new_node(forward_v_ind, 0);
                        dfs_stack.push(new_node);
                    }
                }
            }
        }

        return true;
    }
};


void Test()
{
    freopen("input.txt", "r", stdin); freopen("output.txt", "w", stdout);

    int n, m;
    cin >> n >> m;

    Graph graph(n);

    for (int i = 0; i < m; i++)
    {
        int v1, v2;
        cin >> v1 >> v2;
        graph.AddEdge(v1 - 1, v2 - 1);
    }

    vector<int> top_sorting;
    if (graph.DFS(top_sorting))
    {
        cout << "Yes" << endl;
        for (int i = n - 1; i >= 0; i--)
        {
            cout << top_sorting[i] + 1 << " ";
        }

    }
    else
        cout << "No";
}

int main()
{
    Test();
    return 0;
}

