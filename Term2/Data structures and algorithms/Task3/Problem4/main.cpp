#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h>

using namespace std;
class Graph 
{
    size_t left, right;
    vector<vector<size_t> > List;
    vector<int> MatchRight;
    vector<bool> DfsUse, MatchLeft;
    
    bool TryKuhn(int vert)
    {
        if (DfsUse[vert]) return false;
        DfsUse[vert] = true;
        for (size_t i = 0; i < List[vert].size(); i++)
        {
            int to = List[vert][i];
            if (MatchRight[to] == -1 || TryKuhn(MatchRight[to]))
            {
                MatchRight[to] = vert;
                return true;
            }
        }
        return false;
    }
    
public:
    
    Graph(int inp_left): left(inp_left), right(0), List(inp_left) {}
    
    void AddEdge(size_t v1, size_t v2)
    {
        if (v2 >= right) right = v2 + 1;
        bool exist = false;
        for (size_t i = 0; i < List[v1].size(); i++)
            if (List[v1][i] == v2)
            {
                exist = true;
                break;
            }
        
        if (!exist) 
            List[v1].push_back(v2);
    }
    
    bool Matching(vector<int> &ans)
    {
        MatchLeft.assign(left, false);
        MatchRight.assign(right, -1);
        bool possible = true;
        
        
        for (size_t i = 0; i < left; i++)
            for (size_t j = 0; j < List[i].size(); j++)
                if (MatchRight[List[i][j]] == -1)
                {
                    MatchRight[List[i][j]] = i;
                    MatchLeft[i] = true;
                    break;
                }
        
        
        for (size_t i = 0; i < left; i++)
        {
            if (!MatchLeft[i])
            {
                DfsUse.assign(left, false);
                if (!TryKuhn(i)) 
                {
                    possible = false;
                    break;
                }
            }
        }
        if (possible) ans.assign(MatchRight.begin(), MatchRight.end());
        
        return possible;
    }
    
};

void Test()
{
    ifstream in("input.txt");
    int cubes_num;
    in >> cubes_num;
    string word;
    
    in >> word;
    
    Graph graph(word.length());
    
    for (int i = 0; i < cubes_num; i++)
        for (int j = 0; j < 6; j++)
        {
            char symb;
            in >> symb;
            for (size_t p = 0; p < word.length(); p++)
                if (word[p] == symb)
                {
                    graph.AddEdge(p, i);
                }
        }
    
    vector<int> cubes;
    
    ofstream out("output.txt");
    if (graph.Matching(cubes))
    {
        bool correct = true;
        vector<int> c_number(cubes_num);
        for (size_t i = 0; i < cubes.size(); i++)
        {
            c_number[cubes[i]]++;
            if (c_number[cubes[i]] > 1) 
            {
                correct = false;
                break;
            }
        }
        
        if (!correct)
        {
            cerr << "Wrong matching...";
            exit(-1);
        }
        
        out << "Cubes are:" << endl;
        for (size_t i = 0; i < cubes.size(); i++)
        {
            out << cubes[i] + 1 << " ";
        }
    }
    else
    {
        out << "No possible variant";
    }
    
}


int main()
{

    Test();
    return 0;
}

