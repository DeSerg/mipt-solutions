#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;



void MaxPalindrom(const string &str, string &ans)
{
	enum direct { up, right, diag };
	const int size = str.size();
	vector<vector<int>> ar(size), way(size);
	for (int i = 0; i < size; i++)
	{
		ar[i].resize(size);
		way[i].resize(size);
	}

	for (int i = 0; i < size; i++) ar[i][i] = 1;
	for (int i = 1; i < size; i++) 
	if (str[i - 1] == str[i])
	{
		ar[i][i - 1] = 2;
		way[i][i - 1] = direct::up;
	}
	else
	{
		ar[i][i - 1] = 1;
		way[i][i - 1] = direct::up;
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = size - 1; j >= 0; j--)
		if (i - j >= 2)
		{
			if (str[i] == str[j])
			{
				ar[i][j] = ar[i - 1][j + 1] + 2;
				way[i][j] = direct::diag;
			}
			else
			if (ar[i - 1][j] > ar[i][j + 1])
			{
				ar[i][j] = ar[i - 1][j];
				way[i][j] = direct::up;
			}
			else
			{
				ar[i][j] = ar[i][j + 1];
				way[i][j] = direct::right;
			}
		}
	}



	int _i = size - 1, _j = 0;
	string temp_ans;
	while ((_i - _j) > 1)
	{
		if (way[_i][_j] == direct::diag)
		{
			temp_ans += str[_i];
			_i--; _j++;
		}
		else
		if (way[_i][_j] == direct::up)
			_i--;
		else
			_j++;
	}
	if (ar[_i][_j] == 2)
	{
		temp_ans += str[_i];
		ans = temp_ans;
		reverse(ans.begin(), ans.end());
		ans = temp_ans + ans;
	}
	else
	{
		ans = temp_ans;
		reverse(ans.begin(), ans.end());
		ans = temp_ans + str[_i] + ans;
	}
}


int main()
{
	string inp_s, ans;
	cin >> inp_s;

	MaxPalindrom(inp_s, ans);
	cout << ans.size() << endl << ans << endl;
	return 0;
}