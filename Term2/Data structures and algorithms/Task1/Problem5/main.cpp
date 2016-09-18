#include<iostream>
#include"HashTable.h"
using namespace std;


void RandString(string &str, int min_size, int max_size)
{
	str.clear();
	int length = rand() % (max_size - min_size) + min_size;
	for (long j = 0; j < length; j++)
	{
		char ch = rand() % 26 + 65;
		str += ch;
	}
}

void Test()
{

	HashTable hash_table;

	cerr << endl << "Testing Insert()" << endl;
	for (int i = 0; i < 100; i++)
	{
		string key, value;
		RandString(key, 10, 15);
		RandString(value, 10, 15);
		hash_table.Insert(key, value);
		cout << "Successfully inserted " << i + 1 << " pair:  " << key << " " << value << endl;
	}

	HashTable::Iterator iter = hash_table.Begin();

	cerr << endl << "Testing operator ++" << endl << endl;
	do
	{
		cout << *iter << endl;
		iter++;
	} while (iter != hash_table.End());
	cout << *iter << endl;

	cerr << endl << "Testing operator --" << endl;
	do
	{
		cout << *iter << endl;
		iter--;
	} while (iter != hash_table.Begin());
	cout << *iter << endl;
}

int main()
{
	bool OK = true;

	try
	{
		Test();
	}
	catch (exception e)
	{
		cout << e.what();
		OK = false;
	}

	if (OK)
		cout << endl << "Tests complete!" << endl;
	return 0;
}