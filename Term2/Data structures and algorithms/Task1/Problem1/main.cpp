#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cmath>
#include<time.h>
#include<exception>
#include<sstream>
#include<fstream>
#include<stdlib.h>
#include<sstream>
#include<assert.h>
using namespace std;


//main structures
struct Action
{
	Action() {}
	Action(int act, size_t inp_pos, char inp_simb = 0) :
	Do(act), position(inp_pos), simbol(inp_simb)
	{ }

	enum action { I, D, R };
	int Do;
	size_t position;
	char simbol;
};

int Hirshberg(const std::string &inp_str1, const std::string &inp_str2, std::vector<Action> &vect, long start);

template<class T> 
T min(T a, T b)
{
	if (a < b) return a; else return b;
}

void Reverse(string &str)
{
	string temp_str = str;
	long length = str.length();

	for (size_t i = 0; i < str.length(); i++)
	{
		str[i] = temp_str[length - i - 1];
	}
}

void print(const std::vector<long> &vect)
{
	std::cout << std::endl;
	for (size_t i = 0; i < vect.size(); ++i)
	{
		std::cout << vect[i] << " ";
	}
}

int Vagner_Fisher(const std::string &inp_str1, const std::string &inp_str2, std::vector<long> &vect)
{
	std::string string1 = " " + inp_str1;
	std::string string2 = " " + inp_str2;
	long n = string1.length(), m = string2.length();

	std::vector<long> dist(n), new_dist(n);
	for (long i = 0; i < n; i++) new_dist[i] = i;

	for (long i = 1; i < m; i++)
	{
		swap(dist, new_dist);
		new_dist[0] = i;
		for (long j = 1; j < n; j++)
		{
			if (string1[j] == string2[i]) new_dist[j] = dist[j - 1];
			else new_dist[j] = min(min(new_dist[j - 1], dist[j]), dist[j - 1]) + 1;
		}
	}

	vect.clear();
	for (long i = 0; i < n; i++) vect.push_back(new_dist[i]);

	return new_dist[n-1];
}

int Vagner_Fisher(const std::string &inp_str1, const std::string &inp_str2)
{
	vector<long> vect;
	return Vagner_Fisher(inp_str1, inp_str2, vect);	
}

void CutStrings(const std::string &inp_str1, const std::string &inp_str2, std::vector<Action> &vect, long start = 0)
{
	std::string string1 = inp_str1;
	std::string string2;
	string2.assign(inp_str2, 0, inp_str2.length() / 2);
	long n = string1.length();

	std::vector<long> dist1(n + 1), dist2(n + 1);
	Vagner_Fisher(string1, string2, dist1);

	string2.assign(inp_str2, inp_str2.length() / 2, (inp_str2.length() + 1) / 2);
	Reverse(string1);
	Reverse(string2);
	Vagner_Fisher(string1, string2, dist2);

	long min_ = dist1[0] + dist2[n], num = 0;
	for (long i = 1; i < n + 1; i++)
	if (dist1[i] + dist2[n - i] < min_)
	{
		min_ = dist1[i] + dist2[n - i];
		num = i;
	}

	string1.clear();
	string2.clear();
	string2.assign(inp_str2, 0, inp_str2.length() / 2);
	string1.assign(inp_str1, 0, num);
	Hirshberg(string1, string2, vect, start);

	string1.clear();
	string2.clear();
	string1.assign(inp_str1, num, inp_str1.length() - num);
	string2.assign(inp_str2, inp_str2.length() / 2, (inp_str2.length() + 1) / 2);
	Hirshberg(string1, string2, vect, start + num);

}

int Hirshberg(const std::string &inp_str1, const std::string &inp_str2, std::vector<Action> &vect, long start = 0)
{
	if (inp_str1.length() > 1 && inp_str2.length() > 1)
	{
		CutStrings(inp_str1, inp_str2, vect, start);
	}
	else
	{
		//s1 = "", s2 = "*"
		if (inp_str1.empty())
			for (size_t i = 0; i < inp_str2.length(); ++i)
			{
				Action act(0, start, inp_str2[i]);
				vect.push_back(act);
			}
		
		//s1 = "x", s2 = "*"
		else
		if (inp_str1.length() == 1)
		{
			bool flag = 0;
			for (size_t i = 0; i < inp_str2.length() - 1; ++i)
			{
				if (inp_str1[0] == inp_str2[i] && !flag)
					flag = 1;
				else
				{
					Action act(0, start + flag, inp_str2[i]);
					vect.push_back(act);
				}
			}
			
			if (flag)
			{
				Action act(0, start + flag, inp_str2[inp_str2.length() - 1]);
				vect.push_back(act);
			}
			else
			if (inp_str1[0] != inp_str2[inp_str2.length() - 1])
			{
				Action act(2, start, inp_str2[inp_str2.length() - 1]);
				vect.push_back(act);
			}
		}

		//s1 = "*", s2 = "x"
		else
		if (inp_str2.length() == 1)
		{
			bool flag = 0;
			for (size_t i = 0; i < inp_str1.length() - 1; i++)
			{
				if (inp_str1[i] == inp_str2[0] && !flag)
					flag = 1;
				else
				{
					Action act(1, start + i);
					vect.push_back(act);
				}
			}

			if (flag)
			{
				Action act(1, start + inp_str1.length() - 1);
				vect.push_back(act);
			}
			else
			if (inp_str2[0] != inp_str1[inp_str1.length() - 1])
			{
				Action act(2, start + inp_str1.length() - 1, inp_str2[0]);
				vect.push_back(act);
			}
		}
	}
	return vect.size();
}

int Hirshberg(const std::string &inp_str1, const std::string &inp_str2)
{
	vector<Action> vect;
	return Hirshberg(inp_str1, inp_str2, vect, 0);
}


//structures for tests
void ReadFromFile(string &filename, string &input_string)
{
	input_string.clear();
	ifstream book(filename);

	while (!book.eof())
	{
		char ch;
		book >> ch;
		input_string += ch;
	}
}

void RandInsert(string &str)
{
	long pos = rand() % str.length();
	char ch = rand() % 26 + 65;
	string ins_str;
	ins_str += ch;
	str.insert(pos, ins_str);
}

void RandDelete(string &str)
{
	if (str.empty()) return;

	long pos = rand() % str.length();
	str.erase(pos, 1);
}

void RandReplace(string &str)
{
	if (str.empty()) return;

	long pos = rand() % str.length();
	char ch = rand() % 26 + 65;
	string ins_str;
	ins_str += ch;

	str.replace(pos, 1, ins_str);
}

void ChangeString(string &str, int min_num, int max_num)
{
	assert(min_num <= max_num);
	long changes = rand() % (max_num - min_num) + min_num;

	for (long i = 0; i < changes; i++)
	{
		long type = rand() % 3;
		switch (type)
		{
		case 0:
			RandInsert(str);
			break;
		case 1:
			RandDelete(str);
			break;
		case 2:
			RandReplace(str);
			break;
		}
	}
}

void RepairString(string &str, vector<Action> &actions)
{
	ofstream out("sequence.txt");
	out << "Editing sequence:\n";
	for (size_t i = 0; i < actions.size(); ++i)
	{
		Action act = actions[i];
		out << "Action " << i << ":";
		string temp_str;
		switch (act.Do)
		{
		case 0:
			out << "Insert symbol  " << act.simbol << "  at position  " << act.position << endl;
			break;
		case 1:
			out << "Delete symbol  " << act.simbol << "  from position  " << act.position << endl;
			break;
		case 2:
			temp_str.clear();
			temp_str += act.simbol;
			str.replace(act.position, 1, temp_str);
			out << "Replace symbol at position  " << act.position << "  for symbol  " << act.simbol << endl;
		}
	}

	for (int i = actions.size() - 1; i >= 0; i--)
	{
		Action act = actions[i];
		if (act.Do == act.I)
		{
			string temp_str;
			temp_str += act.simbol;
			str.insert(act.position, temp_str);
		}
		else
		if (act.Do == act.D)
		{
			str.erase(act.position, 1);
		}
	}

	out.close();
}

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

std::string intToString(int i)
{
	std::stringstream ss;
	std::string s;
	ss << i;
	s = ss.str();

	return s;
}

//tests
void Test1()
{
	for (long i = 0; i < 1000; i++)
	{
		string str1;
		long length = rand() % 50 + 50;
		for (long j = 0; j < length; j++)
		{
			char ch = rand() % 26 + 65;
			str1 += ch;
		}

		string str2;
		length = rand() % 5 + 5;
		for (long j = 0; j < length; j++)
		{
			char ch = rand() % 26 + 65;
			str2 += ch;
		}

		if (Vagner_Fisher(str1, str2) != Hirshberg(str1, str2))
			throw logic_error("Different function effect");
	}
}

void Test2()
{
	string filename = "book.txt";
	string str1;
	ReadFromFile(filename, str1);
	string str2 = str1;
	vector<Action> actions;

	ChangeString(str2, 20, 40);
	Hirshberg(str1, str2, actions);
	RepairString(str1, actions);

	if (str1 != str2)
		throw logic_error("Test2: Different final strings");

}

void myTest()
{
	for (long i = 0; i < 1000; i++)
	{

		string str1;
		RandString(str1, 100, 200);
		string str2 = str1;
		vector<Action> actions;

		ChangeString(str2, 20, 40);
		Hirshberg(str1, str2, actions);
		RepairString(str1, actions);

		if (str1 != str2)
		{
			string error = "Different strings at test " + intToString(i+1);
			throw logic_error(error);
		}
/*		else
			cerr << "Test3: subtest " << i + 1 << " OK!\n";*/
	}
}


//launching
int main()
{
	srand(0);
 	bool f = 0;
	try
	{
		cerr << "\nTest1: started!\n";
		Test1();
		cerr << "Test1 OK!\n";

		cerr << "\nTest2: started!\n";
		Test2();
		cerr << "Test2 OK!\n";

		cerr << "\nTest3: started!\n";
		myTest();
		cerr << "myTest OK!\n";
	}
	catch (exception except)
	{
		f = 1;
		cout << "Error occured: " << except.what() << endl;
	}
	if (!f) cerr << "\nTests OK!\n";
	return 0;
}