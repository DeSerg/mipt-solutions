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


class DSU
{
	vector<int> rang;
	vector<int> parent;

public:

	DSU(size_t length) : rang(length, 1), parent(length)
	{
		for (size_t i = 0; i < length; i++)
		{
			parent[i] = i;
		}
	}


	int FindSet(int node_ind)
	{
		if (parent[node_ind] == node_ind)
			return node_ind;
		else
			return parent[node_ind] = FindSet(parent[node_ind]);
	}

	void Unite(int node1, int node2)
	{
		node1 = FindSet(node1);
		node2 = FindSet(node2);

		if (node1 != node2)
		{
			if (rang[node1] < rang[node2]) swap(node1, node2);

			parent[node2] = node1;
			if (rang[node1] == rang[node2]) rang[node1]++;
		}
	}
};

class Graph
{
    //fields
    
    struct Node
    {
        size_t v1, v2;
        double distance;
        Node(size_t inp_v1, size_t inp_v2, double inp_distance):
            v1(inp_v1), v2(inp_v2), distance(inp_distance) {}
        
        bool operator < (const Node &right) const
        {
            return distance < right.distance;
        }
    };
    
    vector<Node> Edges;
    vector<vector<Node> > List;
    vector<bool> used;
    int vert_num;
    int INF = INT_MAX;
    
    
public:

    //constructors
    Graph(): vert_num(0) {}

    Graph(const Graph &inp_graph) :
        Edges(inp_graph.Edges.begin(), inp_graph.Edges.end()),
        List(inp_graph.List.begin(), inp_graph.List.end()) {}
      

    
    Graph(const vector<vector<double> > &inpMatrix):
        vert_num(inpMatrix.size())
    {        
        //Edges initalizing
        Edges.clear();
        for (int i = 0; i < vert_num; i++)
            for (int j = 0; j < vert_num; j++)
            {
                if (inpMatrix[i][j] > 0)
                {
                    Node node(j, i, -log(inpMatrix[i][j]));
                    Edges.push_back(node);
                    List[i].push_back(node);
                }
            }
    }

    void AddEdge(size_t v1, size_t v2, double distance)
    {
        Node node(v1, v2, distance);
        Edges.push_back(node);
        if (v1 >= List.size()) List.resize(v1 + 1);
        if (v2 >= List.size()) List.resize(v2 + 1);
        List[v1].push_back(node);
        List[v2].push_back(node);
    }

    void AddEdge(Node node)
    {
        size_t v1 = node.v1, v2 = node.v2;

        if (v1 >= List.size()) List.resize(v1 + 1);
        if (v2 >= List.size()) List.resize(v2 + 1);
        
        Edges.push_back(node);
        List[v1].push_back(node);
        List[v2].push_back(node);
    }
    
    
    void DFS(size_t vert, vector<int> &way)
    {
        used[vert] = true;
        
        for (size_t i = 0; i < List[vert].size(); i++)
        {
            if (!used[List[vert][i].v2])
            {
                way.push_back(vert);
            }
        }
        
    }
    
    void FindShortestWay(vector<int> &way)
    {
        sort(Edges.begin(), Edges.end());
        
        DSU dsu(List.size());
        Graph graph;
        
        for (size_t i = 0; i < Edges.size(); i++)
        {
            size_t v1 = Edges[i].v1, v2 = Edges[i].v2;
            
            if (dsu.FindSet(v1) != dsu.FindSet(v2))
            {
                graph.AddEdge(Edges[i]);
                dsu.Unite(v1, v2);
            }
        }
        
        graph.used.resize(graph.List.size());
        for (size_t i = 0; i < graph.List.size(); i++)
        {
            if (!graph.used[i])
                graph.DFS(i, way);     
        }
        
        
    }
    
    

};


double GetNormalPoint(double sigma)
{
    double mu = 0.5;
    double x = 10 * (double(rand() % 1000) / 1000 - 1),
            y = 10 * (double(rand() % 1000) / 1000 - 1);
    double s = x * x + y * y;
    while (s > 1 || s == 0)
    {
        x = double(rand() % 1000) / 1000 - 1;
        y = double(rand() % 1000) / 1000 - 1;
        s = x * x + y * y;
    }
    
    double z = x * sqrt(-2 * log(s) / s);
    z = mu + sigma * z;
    return z;
}

void GeneratePoints(int n, int m, double s1, double s2, vector<pair<double, double> > &points)
{
    //cout << "Generating points..." << endl;
    for (int i = 0; i < n; i++)
    {
        double x = GetNormalPoint(s1), y = GetNormalPoint(s1);
        points.push_back(make_pair(x, y));
        //cout << endl << "Around point [" << setprecision(2) << x << "," << setprecision(2) << y << "]:" << endl;
        
        for (int j = 0; j < m; j++)
        {
            double x = GetNormalPoint(s2), y = GetNormalPoint(s2);
            points.push_back(make_pair(x, y));
            //cout << "[" << setprecision(2) << x << "," << setprecision(2) << y << "]:" << endl;
        }
    }
    
}

template<class T> T alabs(T arg)
{
    if (arg < 0) 
        return -arg;
    else
        return arg;
}

double Distance(pair<double, double> p1, pair<double, double> p2)
{
    return sqrt(alabs(p1.first - p2.first) + alabs(p1.second - p2.second));
}

void Test()
{
    vector<pair<double, double> > points;
    int n, m;
    double s1, s2;
    
    ifstream in("input.txt");
    in >> n >> m >> s1 >> s2;
    GeneratePoints(n, m, s1, s2, points);
    
    Graph graph;

    double long_way_length = 0;    
    for (size_t i = 0; i < points.size(); i++)
    {
        if (i < points.size() - 1)
        {
            pair<double, double> p1 = points[i], p2 = points[i+1];
            long_way_length += Distance(p1, p2);
        }
        else
        {
            pair<double, double> p1 = points[i], p2 = points[0];
            long_way_length += Distance(p1, p2);
        }
        for (size_t j = i + 1; j < points.size(); j++)
        {
            pair<double, double> p1 = points[i], p2 = points[j];
            double dist = Distance(p1, p2);
            graph.AddEdge(i, j, dist);
        }
    }
    vector<int> way;
    graph.FindShortestWay(way);
    
    double short_way_length = 0;
    
    for (size_t i = 0; i < way.size(); i++)
    {
        if (i < way.size() - 1)
        {
            pair<double, double> p1 = points[way[i]], p2 = points[way[i+1]];
            short_way_length += Distance(p1, p2);
        }
        else
        {
            pair<double, double> p1 = points[way[i]], p2 = points[way[0]];
            short_way_length += Distance(p1, p2);
        }
    }
    
    cout << endl << "Long way distance: " << setprecision(3) << long_way_length << endl;
    cout << "Short way distance: " << setprecision(3) << short_way_length << endl;
    
}

int main()
{
    for (int i = 0; i < 50; i++)
    {
        Test();
    }
    return 0;
}
