#include "gtest/gtest.h"
#include "findoccurences.h"
#include <vector>
#include <string>
#include <ctime>

void RandString(string &str, int min_size, int max_size)
{
	str.clear();
    
    int length;
    if (min_size != max_size) 
        length = rand() % (max_size - min_size) + min_size;
    else
        length = min_size;
    
	for (long j = 0; j < length; j++)
	{
		char ch = rand() % 3 + 65;
		str += ch;
	}
}


vector<int> slowSolution(const std::string &sourse, const std::string &pattern) {
    
    std::vector<int> occurences;
    if (pattern.length() > sourse.length())
        return occurences;
    
    for (int i = 0; i < (int)sourse.length() - (int)pattern.length() + 1; ++i) {
        
        
        std::string::const_iterator patternStart = pattern.cbegin();
        std::string::const_iterator sourseStart = sourse.cbegin() + i;
        std::string::const_iterator sourseEnd = sourseStart + pattern.length();
        
        if (std::equal(sourseStart, sourseEnd, patternStart))
            occurences.push_back(i);
    }
            
    return occurences;
}


TEST(EXTREME, 1) {
    
    std::string sourse("aaaaaaaaaaaaa");
    std::string pattern("a");
    vector<int> v1(slowSolution(sourse, pattern));
    
    SuffixTree st(sourse);
    vector<int> v2(findAllOccurences(st, pattern));
    
    std::cout << sourse << std::endl << pattern << std::endl;
    for (int val: v1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    for (int val: v2) {
        std::cout << val << " ";
    }    
    std::cout << std::endl;
    
    EXPECT_EQ(v1, v2);
    
}

TEST(EXTREME, 2) {
    
    std::string sourse("");
    std::string pattern("asccs");
    vector<int> v1(slowSolution(sourse, pattern));
    
    SuffixTree st(sourse);
    vector<int> v2(findAllOccurences(st, pattern));
    
    std::cout << sourse << std::endl << pattern << std::endl;
    for (int val: v1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    for (int val: v2) {
        std::cout << val << " ";
    }    
    std::cout << std::endl;
    
    EXPECT_EQ(v1, v2);
    
}

TEST(EXTREME, 3) {
    
    std::string sourse;
    std::string pattern("a");
    RandString(sourse, 100000, 10000);
    RandString(pattern, 1, 4);
    
    vector<int> v1(slowSolution(sourse, pattern));
    
    SuffixTree st(sourse);
    vector<int> v2(findAllOccurences(st, pattern));
    
    std::cout << sourse << std::endl << pattern << std::endl;
    for (int val: v1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    for (int val: v2) {
        std::cout << val << " ";
    }    
    std::cout << std::endl;
    
    EXPECT_EQ(v1, v2);
    
}



int main(int argc, char **argv) {
    
    srand(time(NULL));
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    
}
