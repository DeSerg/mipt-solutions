#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "suffixtree.h"

class FindOccurences
{
    std::string sourse;
    std::string pattern;
    bool rootInit;
    
    std::vector<int> vert_depth;
    std::vector<int> occurences;
    
    bool canPass(int vert, int substrStart, int substrEnd) {
        
        if (vert_depth[vert] >= (int)pattern.length()) {
            return true;
        }
        
        
        int length = std::min<int>(substrEnd - substrStart, pattern.length() - vert_depth[vert]);
        
        std::string::const_iterator patternStart = pattern.cbegin() + vert_depth[vert];
        std::string::const_iterator patternEnd = patternStart + length;
        std::string::const_iterator sourseStart = sourse.cbegin() + substrStart;
        
        bool ans = std::equal(patternStart, patternEnd, sourseStart);
        return ans;
    }
    

public:
    

    FindOccurences(const std::string &pattern_): pattern(pattern_), rootInit(false) {}
    
    std::vector<int> getOccurences() {
        std::sort(occurences.begin(), occurences.end());
        return occurences;
    }
    
    
    void setSourse(const std::string &sourse_) {
        sourse = sourse_;
    }
    
    void setDepth(const std::vector<int> &vert_depth) {
        this->vert_depth = vert_depth;
    }
    
    void beforeVertexProc(int vert) {
        if (!rootInit) {
            vert_depth[vert] = 0;
            rootInit = true;
        }
    }
    
    void edgeProc(int from, int to, int substrStart, int substrEnd, bool &go) {
        
        substrEnd = std::min(substrEnd, (int)sourse.length());
        if (substrEnd == (int)sourse.length()) {
            go = false;
            
            if (canPass(from, substrStart, substrEnd)) {
                int value = sourse.size() - (substrEnd - substrStart) - vert_depth[from];
                occurences.push_back(value);
            }
            
        } else {
            go = canPass(from, substrStart, substrEnd);
        }
        
    }
    
    void afterVertexProc(int vert) {}
    
};


std::vector<int> findAllOccurences(const SuffixTree &suffixTree, const std::string &pattern) {
    
    if (pattern.length() == 0) {
        return std::vector<int>(0);
    }
    
    FindOccurences visitor(pattern);
    suffixTree.DFS(&visitor);
    
    return visitor.getOccurences();
}
