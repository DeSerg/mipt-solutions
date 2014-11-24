#define uchar unsigned char

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <limits>

using namespace std;

const int inf = numeric_limits<int>::max();

class SuffixTree {
    
    struct Link {
        int start;
        int end;
        int to;
        
        Link(): to(-1) {}
        
        Link(int start_, int end_, int to_): start(start_), end(end_), to(to_) {}        
    };
    
    struct Vertex {
        vector<Link> links;
        int suffix;
        
        Vertex() {
            links.assign(256, Link());
            suffix = -1;
        }
    };

    vector<Vertex> tree;
    int root;
    int dummy;
    string sourse;
    
    //gets the string's character by index
    uchar chr(int ind);
    
    int &suf(int vert);
    
    int newVert();
    
    void mkLink(int from, int start, int end, int to);
    
    void print(int v, int start, int end, string prefix);
    
    void initTree();
    
    pair<int, int> canonize(int vert, int start, int end);
    
    pair<bool, int> testAndSplit(int vert, int start, int end, uchar ch);
    
    pair<int, int> update(int vert, int start, int end);
    
    
public:
    
    SuffixTree(const string &source_);
    
    template <class Visitor>
    void DFS(Visitor *visitor) const {
        DFS<Visitor>(visitor, root);
    }
    
    
    template <class Visitor>
    void DFS(Visitor *visitor, int curVert) const {
        
        static bool set = false;
        if (!set) {
            visitor->setSourse(sourse);
            set = true;
        }
        
        visitor->beforeVertexProc(curVert);
    
        for (uchar ch = 0; ch < 255; ++ch) {
            if (tree[curVert].links[ch].to != -1) {
                Link link = tree[curVert].links[ch];
                bool go;
                visitor->edgeProc(curVert, link.to, link.start, link.end, go);
                if (go) DFS(visitor, link.to);
            }
        }
        
        visitor->afterVertexProc(curVert);
    }
    
};
