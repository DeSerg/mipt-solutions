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
    
    std::unordered_map<int, int> vertDepth;
    std::vector<int> occurences;
    
    bool canPass(int vert, int substrStart, int substrEnd) {
        
        if (vertDepth[vert] >= (int)pattern.length()) {
            return true;
        }
        
        int length = std::min<int>(substrEnd - substrStart, pattern.length() - vertDepth[vert]);
        std::string::const_iterator patternStart = pattern.cbegin() + vertDepth[vert];
        std::string::const_iterator patternEnd = patternStart + length;
        std::string::const_iterator sourseStart = sourse.cbegin() + substrStart;
        
        return std::equal(patternStart, patternEnd, sourseStart);
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
    void beforeVertexProc(int vert) {
        if (!rootInit) {
            vertDepth[vert] = 0;
            rootInit = true;
        }
    }
    
    void edgeProc(int from, int to, int substrStart, int substrEnd, bool &go) {
                
        if (substrEnd > (int)sourse.length()) {
            go = false;
            
            if (canPass(from, substrStart, substrEnd)) {
                occurences.push_back(substrStart - vertDepth[from] + 1);
            }
            
        } else if (canPass(from, substrStart, substrEnd)) {
            go = true;
            vertDepth[to] = vertDepth[from] + substrEnd - substrStart;
        } else {
            go = false;
        }
        
    }
    
    void afterVertexProc(int vert) {}
    
};


std::vector<int> findAllOccurences(const SuffixTree &suffixTree, const std::string &pattern) {
    
    FindOccurences visitor(pattern);
    suffixTree.DFS(&visitor);
    
    return visitor.getOccurences();
}
