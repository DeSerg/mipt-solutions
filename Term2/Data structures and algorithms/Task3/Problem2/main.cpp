#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <stack>
#include <stdexcept>
#include <algorithm>
#include <climits>
#include <cmath>

using namespace std;

class Graph
{
    //fields
    
    struct Node
    {
        size_t v1, v2;
        double distance;
        Node(size_t inp_v1, size_t inp_v2, double inp_distance):
            v1(inp_v1), v2(inp_v2), distance(inp_distance) {}
    };
    
    vector<Node> Edges;
    vector<vector<double> > Matrix;
    vector<vector<int> > Parents;
    int vert_num;
    int INF = INT_MAX;
    double EPS = 1e-10;
    
    
    void FindWay(int v1, int v2, vector<int> &ans)
    {
        if (Parents[v1][v2] == -1 || (v1 == Parents[v1][v2] && v2 == Parents[v1][v2])) 
            ans.push_back(v1);
        else
        {
            if (v1 < 0 || v2 < 0) 
                throw logic_error("Error at finding cycle");
            FindWay(v1, Parents[v1][v2], ans);
            FindWay(Parents[v1][v2], v2, ans);
        }
    }

public:

    //constructors
    Graph(){}

    Graph(const Graph &inp_graph) :
        Edges(inp_graph.Edges.begin(), inp_graph.Edges.end()),
        Matrix(inp_graph.Matrix.begin(), inp_graph.Matrix.end()),
        Parents(inp_graph.Parents.begin(), inp_graph.Parents.end()) {}

    
    Graph(const vector<vector<double> > &inpMatrix):
        vert_num(inpMatrix.size())
    {
        //Matrix initalizing
        Matrix.resize(vert_num);
        Parents.resize(vert_num);
        for (int i = 0; i < vert_num; i++) 
        {
            Matrix[i].resize(vert_num);
            Parents[i].resize(vert_num);
        }
        
        for (int i = 0; i < vert_num; i++)
        {

            for (int j = 0; j < vert_num; j++)
            {
                if (inpMatrix[i][j] > 0)
                {
                    Matrix[j][i] = -log(inpMatrix[i][j]);
                    Parents[j][i] = -1;
                }
                else
                    Matrix[j][i] = INF;
            }
        }
        
        
        //Edges initalizing
        Edges.clear();
        for (int i = 0; i < vert_num; i++)
            for (int j = 0; j < vert_num; j++)
            {
                if (inpMatrix[i][j] > 0)
                {
                    Node node(j, i, -log(inpMatrix[i][j]));
                    Edges.push_back(node);
                }
            }
    }

    bool FordBellman(vector<int> &ans)
    {
        ans.clear();
        
        int cycle;
        
        vector<double> dist(vert_num);
        vector<int> parent(vert_num, -1);
        
        for (int i = 0; i < vert_num; i++)
        {
            cycle = -1;
            for (size_t j = 0; j < Edges.size(); j++)
            {
                int v1 = Edges[j].v1, v2 = Edges[j].v2;
                double distance = Edges[j].distance;
                if (dist[v2] > dist[v1] + distance)
                {
                    dist[v2] = dist[v1] + distance;
                    parent[v2] = v1;
                    cycle = v2;
                }
            }
        }
        
        
        if (cycle > -1)
        {
            for (int i = 0; i < vert_num*3; i++)
                cycle = parent[cycle];
            
            for (int cur = cycle; ; cur = parent[cur])
            {
                ans.push_back(cur);
                if (cur == cycle && ans.size() > 1) break;
            }
            reverse(ans.begin(), ans.end());
            return true;
        }
        else
            return false;
    }
    
    
    
    bool FloydWarshall(vector<int> &ans)
    {
        vector<vector<double> > Distance(Matrix.begin(), Matrix.end());
        int cycle = -1;
        
        for (int k = 0; k < vert_num && cycle == -1; k++)
            for (int i = 0; i < vert_num && cycle == -1; i++)
                for (int j = 0; j < vert_num && cycle == -1; j++)
                {
                    if (Distance[i][k] < INF  &&  Distance[k][j] < INF  &&  Distance[i][k] + Distance[k][j] < Distance[i][j])
                    {
                        Distance[i][j] = Distance[i][k] + Distance[k][j];
                        Parents[i][j] = k;
                        if (i == j && Distance[i][j] < 0) cycle = i;
                    }
                }
        
        
        if (cycle != -1)
        {
            FindWay(cycle, cycle, ans);
            ans.push_back(cycle);
            return true;
        }
        else return false;
        
        
    }
};


void MakeGraph(vector<vector<double> > &Matrix)
{
    size_t vert_num = Matrix.size();
    for (size_t i = 0; i < vert_num; i++)
        for (size_t j = 0; j < vert_num; j++)
        {
            bool edge = rand() % 2;
            if (edge)
            {
                bool low = rand() % 2;
                if (low)
                    Matrix[i][j] = 1 / (1 + (double)(rand() % 100) / 20);
                else
                    Matrix[i][j] = 1 + (double)(rand() % 100) / 20;
            }
            else
                Matrix[i][j] = 0;
        }
}


void PrintGraph(vector<vector<double> > &Matrix)
{
    ofstream out("output.txt");
    
    size_t vert_num = Matrix.size();
    for (size_t i = 0; i < vert_num; i++)
    {
        for (size_t j = 0; j < vert_num; j++)
        {
            out << setw(5) << setprecision(2) << Matrix[i][j];
        }
        out << endl;
    }
}


void ReadGraph(vector<vector<double> > &Matrix)
{
    ifstream in("input.txt");
    size_t vert_num;
    in >> vert_num;
    Matrix.resize(vert_num);
    
    for (size_t i = 0; i < vert_num; i++)
    {
        Matrix[i].resize(vert_num);
        for (size_t j = 0; j < vert_num; j++)
        {
            in >> Matrix[i][j];
        }
    }
}

int CheckAnswer(const vector<vector<double> > &Matrix, vector<int> &way)
{
    int result;
    double money = 1;
    for (size_t i = 0; i < way.size() - 1; i++)
    {
        size_t v1 = way[i], v2 = way[i+1];
        money *= Matrix[v2][v1];
    }
    
    if (money <= 1) 
        result = 0;
    else
        result = 1;
    
    return result;
}

int FordBellmanTest()
{
    size_t vert_num = rand() % 5 + 5;
    vector<vector<double> > Matrix(vert_num, vector<double>(vert_num));
    
    MakeGraph(Matrix);
    PrintGraph(Matrix);
    Graph graph(Matrix);
    
    vector<int> way;
    
    if (graph.FordBellman(way))
        return CheckAnswer(Matrix, way);
    else
        return 2;
}


int FloydWarshallTest()
{
    size_t vert_num = rand() % 5 + 5;
    vector<vector<double> > Matrix(vert_num, vector<double>(vert_num));
    
    MakeGraph(Matrix);
    PrintGraph(Matrix);
    Graph graph(Matrix);
    
    vector<int> way;

    if (graph.FloydWarshall(way))
        return CheckAnswer(Matrix, way);
    else    
        return 2;
    
    
}




void StressTest()
{
    bool fail = false;
    int test, i_max = 1000;
    
    for (int i = 0; i < i_max; i++)
    {
        if (i < i_max / 2) 
            test = FordBellmanTest();
        else
            test = FloydWarshallTest();
        switch (test)
        {
        case 0:
            cout << "Test " << i + 1 << " failed!" << endl;
            fail = true;
            break;
        case 1:
            cout << "Test " << i + 1 << " complete!" << endl;
            break;
        case 2:
            cout << "Cycle was not found!" << endl;
            break;
        }
    }
    
    if (!fail) cout << "Tests OK!" << endl;
}



void HandleTest()
{
    
    vector<vector<double> > Matrix;
    
    ReadGraph(Matrix);
    Graph graph(Matrix);
    
    vector<int> way;
    
    if (graph.FloydWarshall(way))
    {
        cout << "Way:" << endl;
        for (size_t i = 0; i < way.size(); i++)
        {
            cout << way[i] + 1 << " ";
        }
    }      
}

int main()
{
    
    //StressTest();
    HandleTest();
    cout << endl;
    
    return 0;
    
}

