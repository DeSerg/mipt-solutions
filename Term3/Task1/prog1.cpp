#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

void prefixFunc(const string &str, vector<int> &prefix) {
    
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
    }

}

void Task() {
    
    string s1;
    string s2;
    cin >> s1;
    cin >> s2;
    
    string concat = s1 + "#" + s2;
    vector<int> prefix;
    prefixFunc(concat, prefix);
            
    
    bool success = false;
    
    for (size_t i = s1.length() + 1; i < prefix.size(); ++i) {
        if (!prefix[i]) {
            success = true;
            break;
        }
    }
    
    if (success) {
        cout << "Yes" << endl;
    } else {
        cout << "No" << endl;
        
        reverse(prefix.begin(), prefix.end());
        reverse(s2.begin(), s2.end());
        
        string out;
        size_t pos = 0;
        while (pos < s2.length()) {
            int step = prefix[pos];
            out.push_back(' ');
            for (size_t i = pos; i < pos + step; ++i) {
                out.push_back(s2[i]);
            }
            pos += step;
        }
        reverse(out.begin(), out.end());
        out.resize(out.size() - 1);
        cout << out << endl;
        
    }

}


int main()
{
    Task();
    return 0;
}
