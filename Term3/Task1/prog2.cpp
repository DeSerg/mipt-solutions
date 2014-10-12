#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

void prefixFunc(const string &str, vector<int> &prefix, int firstLength) {
    
    int length = str.length();
    prefix.resize(length);
    prefix[0] = 0;

    for (int i = 1; i < length; i++) {
        
        int prefix_val = prefix[i-1];
        while (prefix_val > 0 && str[i] != str[prefix_val]) {
            prefix_val = prefix[prefix_val - 1];
        }
        if (str[i] == str[prefix_val]) {
            prefix[i] = prefix_val + 1;
        }
        
        if (prefix[i] == firstLength) {
            prefix[i] = prefix[prefix_val];
        }
    }
    
    

}

void Task() {
    
    string s1;
    string s2;
    cin >> s2;
    s1 = s2;
    reverse(s1.begin(), s1.end());
    
    string concat = s1 + "#" + s2;
    vector<int> prefix;
    int length = s1.length();
    prefixFunc(concat, prefix, length);
            
    
    size_t intersect = prefix[prefix.size() - 1];
    
    s2 += s1.assign(s1, intersect, s1.length() - intersect);

    cout << s2 << endl;
    
}


int main()
{
    Task();
    return 0;
}
