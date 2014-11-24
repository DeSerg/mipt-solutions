#include "suffixtree.h"


using namespace std;

SuffixTree::SuffixTree(const string &source_)
{
    sourse = source_;
    sourse.append("$");
    initTree();
    
    pair<int, int> activePoint = make_pair(root, 0);
    for (int i = 0; i < (int)sourse.length(); ++i) {
        activePoint = update(activePoint.first, activePoint.second, i);
        activePoint = canonize(activePoint.first, activePoint.second, i + 1);
    }  
}



uchar SuffixTree::chr(int ind) {
    if (ind < 0) {
        return -ind - 1;
    } else {
        if (ind < (int)sourse.length()) 
            return sourse[ind];
        else 
            throw logic_error("Incorrect index");
    }
}

int &SuffixTree::suf(int vert) {
    return tree[vert].suffix;
}

int SuffixTree::newVert() {
    tree.push_back(Vertex());
    return (int)tree.size() - 1;
}

void SuffixTree::mkLink(int from, int start, int end, int to) {
    
    tree[from].links[chr(start)] = Link(start, end, to);
    
}

void SuffixTree::print(int v, int start = 0, int end = 0, string prefix = "") {
    // What's written on the edge that leads here
    cout << prefix;
    for(int i = start; i < end && i < (int)sourse.length(); i++)
        cout << chr(i);
    
    if(end == inf) cout << "@";
    
    // This vertex and its suffix link
    cout << "[" << v << "]";
    if(suf(v) != -1)
        cout << "f = " << suf(v) << endl;
    
    // The children
    for(int i = 0; i < 256; i++)
        if(tree[v].links[i].to != -1) {
            print(tree[v].links[i].to, tree[v].links[i].start,
                  tree[v].links[i].end, prefix + "   ");
        }
}

void SuffixTree::initTree() {
    tree.clear();
    dummy = newVert();
    root = newVert();
    
    suf(root) = dummy;
    for (int i = 0; i < 256; ++i) {
        mkLink(dummy, -i - 1, -i, root);
    }
}

pair<int, int> SuffixTree::canonize(int vert, int start, int end) {
    if (end <= start) {
        return make_pair(vert, start);
    } else {
        Link cur = tree[vert].links[chr(start)];
        while (end - start >= cur.end - cur.start) {
            start += cur.end - cur.start;
            vert = cur.to;
            if (end > start) {
                cur = tree[vert].links[chr(start)];
            }
        }
        return make_pair(vert, start);
    }
    
}

pair<bool, int> SuffixTree::testAndSplit(int vert, int start, int end, uchar ch) {
    
    if (end <= start) {
        return make_pair(tree[vert].links[ch].to != -1, vert);
    } else {
        Link cur = tree[vert].links[chr(start)];
        if (ch == chr(cur.start + end - start)) {
            return make_pair(true, vert);
        }
        
        int middle = newVert();
        mkLink(vert, cur.start, cur.start + end - start, middle);
        mkLink(middle, cur.start + end - start, cur.end, cur.to);
        return make_pair(false, middle);
    }
}


pair<int, int> SuffixTree::update(int vert, int start, int end) {
    pair<bool, int> splitRes;
    int oldR = root;
    
    splitRes = testAndSplit(vert, start, end, chr(end));
    while (!splitRes.first) {
        
        mkLink(splitRes.second, end, inf, newVert());
        
        if (oldR != root) 
            suf(oldR) = splitRes.second;
        oldR = splitRes.second;
        
        pair<int, int> newPoint = canonize(suf(vert), start, end);
        vert = newPoint.first;
        start = newPoint.second;
        splitRes = testAndSplit(vert, start, end, chr(end));
        
    }
    if (oldR != root) 
        suf(oldR) = splitRes.second;
    
    return make_pair(vert, start);
}



