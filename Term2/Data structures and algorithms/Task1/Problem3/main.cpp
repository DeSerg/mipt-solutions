#include<iostream>
#include<fstream>
#include<string>
#include<vector>

using namespace std;

int LCIS(const vector<int> &a, const vector<int> &b, vector<int> &result)
{
	if (!a.size() || !b.size()) return 0;

	int n = a.size(), m = b.size();
	vector<int> vect(m), prev(m);

	for (int i = 0; i < m; i++)
	if (a[0] == b[i])
		vect[i] = 1;
	else
		vect[i] = 0;


	for (int i = 1; i < n; i++)
	{
		int best = 0, best_ind = 0;
		for (int j = 0; j < m; j++)
		{
			if (a[i] == b[j] && vect[j] < (best + 1))
			{
				vect[j] = best + 1;
				prev[j] = best_ind;
			}
			else
			if (a[i] > b[j] && vect[j] > best)
			{
				best = vect[j];
				best_ind = j;
			}
		}
	}
	int pos = 0;
	for (int i = 0; i < m; i++)
	{
		if (vect[i] > vect[pos]) pos = i;
	}

	int ans = vect[pos];
	result.clear();
	while (result.size() < ans)
	{
		result.push_back(b[pos]);
		pos = prev[pos];
	}

	reverse(result.begin(), result.end());
	return ans;
}


void Test()
{
	ifstream in("input.txt"); 
	int n;
	in >> n;
	vector<int> vect1(n);
	for (int i = 0; i < n; i++) in >> vect1[i];

	in >> n;
	vector<int> vect2(n);
	for (int i = 0; i < n; i++) in >> vect2[i];

	vector<int> result;

	cout << LCIS(vect1, vect2, result) << endl;

	for (int i = 0; i < result.size(); i++)
		cout << result[i] << " ";
}



int main()
{
	Test();
	return 0;
}