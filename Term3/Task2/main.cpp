#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <cstdio>

using namespace std;

const int K = 300;

class AhoCorasik {
    
public:
    
    struct Node {
        int child[K];
        bool last;
        int parent;
        char parch;
        int link;
        int go[K];
        int depth;
        Node() {
            memset (child, 255, sizeof(child));
            memset (go, 255, sizeof(go));
            last = false;
        }
    };
    
    vector<Node> nodeV;
    
    AhoCorasik() {
        Node node;
        node.depth = 0;
        node.link = 0;
        node.parch = 0;
        node.parent = 0;
        nodeV.push_back(node);
    }
    
    void addStr(const string &str) {
        
        int vert = 0;
        for (size_t i = 0; i < str.length(); ++i) {
            int ch = str[i];
            if (nodeV[vert].child[ch] == -1) {
                Node node;
                node.link = -1;
                node.parent = vert;
                node.parch = ch;
                node.depth = nodeV[vert].depth + 1;
                nodeV.push_back(node);
                nodeV[vert].child[ch] =  nodeV.size() - 1;
            }
            vert = nodeV[vert].child[ch];
        }
        
        nodeV[vert].last = true;
    }
    
    int getLink(int vert) {
        if (nodeV[vert].link == -1) {
            if (vert == 0 || nodeV[vert].parent == 0) {
                nodeV[vert].link = 0;
            } else {
                nodeV[vert].link = go(getLink(nodeV[vert].parent), nodeV[vert].parch);
            }
        }
        return nodeV[vert].link;
    }
    
    int go(int vert, int ch) {
        if (nodeV[vert].go[ch] == -1) {
            if (nodeV[vert].child[ch] != -1) {
                nodeV[vert].go[ch] = nodeV[vert].child[ch];
            } else {
                nodeV[vert].go[ch] = vert == 0 ? 0 : go(getLink(vert), ch);
            }
        }
        return nodeV[vert].go[ch];
    }
  /*  
    void setRef(int vert = 0) {
        getLink(vert);
        for (int ch = 0; ch < K; ++ch) {
            if (nodeV[vert].child[ch] != -1) {
                setRef(nodeV[vert].child[ch]);
            }
        }
    }
    */
    int eatText(const string &text) {
        
        int vert = 0;
        for (size_t i = 0; i < text.length(); ++i) {
            int ch = text[i];
            vert = go(vert, ch);
            if (nodeV[vert].last) {
                return i - nodeV[vert].depth + 2;
            }
        }
        return 0;
    }
    
};

int main()
{
 /*   
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
*/
    int n;
    AhoCorasik corasik;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        string str;
        cin >> str;
        corasik.addStr(str);
    }
    
//    corasik.setRef();
    
    int m;
    string text;
    cin >> m; 
    getline(cin, text);
    //ifstream in(stdin);    
    for (int i = 0; i < m; ++i) {
        getline(cin, text);
        int position = corasik.eatText(text);
        if (position) {
            cout << i + 1 << " " << position;
            return 0;
        }
    }
    
    cout << "Passed" << endl;
    return 0;
}

