#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <iterator>
using namespace std;

const int inf = numeric_limits<int>::max();

class Comparator
{
	const vector<int> *array;

public:
	Comparator(const vector<int> *inp_array) : array(inp_array) {}

	bool operator ()(int a, int b)
	{
		return (*array)[a] <= (*array)[b];
	}
};

int LIS(vector<int> &Array, vector<int> &result)
{
	vector<int> lis_ar;
	unique(Array.begin(), Array.end());
	Array.push_back(inf);
	size_t length = Array.size();
	vector<int> Ref(length);

	lis_ar.push_back(length - 1);

	Comparator comp(&Array);
	
	for (size_t i = 0; i < length - 1; ++i)
	{
		vector<int>::iterator index_it = upper_bound(lis_ar.begin(), lis_ar.end(), i, comp);
		int index = distance(lis_ar.begin(), index_it);

		if (*index_it == length - 1)
		{
			*index_it = i;
			lis_ar.push_back(length - 1);
		}
		else
			*index_it = i;

		if (index > 0) Ref[i] = lis_ar[index - 1];
	}

	result.clear();
	int index = lis_ar[lis_ar.size() - 2];
	result.push_back(Array[index]);

	for (int i = 0; i < lis_ar.size() - 2; i++)
	{
		index = Ref[index];
		result.push_back(Array[index]);
	}
	
	reverse(result.begin(), result.end());
	return lis_ar.size() - 1;
}


void Test1()
{
	int len, temp, k, b, m;
	cin >> len >> temp >> k >> b >> m;
	vector<int> ar(len);
	ar[0] = temp;
	for (int i = 1; i < len; i++)
	{
		ar[i] = (ar[i - 1] * k + b) % m;
	}

	vector<int> result;
	LIS(ar, result);
	for (long i = 0; i < result.size(); i++)
	{
		cout << result[i] << " ";
	}

}

int main()
{
	Test1();
	return 0;
}
