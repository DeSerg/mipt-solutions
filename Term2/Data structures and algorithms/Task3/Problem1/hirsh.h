#pragma once

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



std::string intToString(int i)
{
	std::stringstream ss;
	std::string s;
	ss << i;
	s = ss.str();

	return s;
}
