#include "test_segment_tree.h"
#include "segment_tree.h"
#include "sequence.h"
#include <cctype>
#include <functional>
#include <stdexcept>
#include <string>
#include <iostream>


namespace {

class Matrix
{
public:
	int ar[4];
	Matrix() { ar[0] = ar[1] = ar[2] = ar[3] = 0; }
	Matrix(int a, int b, int c, int d)
	{
		ar[0] = a;
		ar[1] = b;
		ar[2] = c;
		ar[3] = d;
	}
	void Set(int a, int b, int c, int d)
	{
		ar[0] = a;
		ar[1] = b;
		ar[2] = c;
		ar[3] = d;
	}
	void LoadIdentity()
	{
		ar[0] = ar[3] = 1;
		ar[1] = ar[2] = 0;
	}
	void LoadRandom(long max)
	{
		ar[0] = rand() % max;
		ar[1] = rand() % max;
		ar[2] = rand() % max;
		ar[3] = rand() % max;
	}
	bool operator ==(const Matrix &m)
	{
		return (ar[0] == m.ar[0] && ar[1] == m.ar[1] && ar[2] == m.ar[2] && ar[3] == m.ar[3]);
	}
	bool operator !=(const Matrix &m)
	{
		return (ar[0] != m.ar[0] || ar[1] != m.ar[1] || ar[2] != m.ar[2] || ar[3] != m.ar[3]);
	}
};


class SumMatrix
{
public:
	Matrix operator() (const Matrix &a, const Matrix &b) const
	{
		Matrix mat;
		mat.ar[0] = a.ar[0] + b.ar[0];
		mat.ar[1] = a.ar[1] + b.ar[1];
		mat.ar[2] = a.ar[2] + b.ar[2];
		mat.ar[3] = a.ar[3] + b.ar[3];
		return mat;
	}
};

class MultMatrix
{
public:
	Matrix operator() (const Matrix &a, const Matrix &b) const
	{
		Matrix mat;
		mat.ar[0] = a.ar[0] * b.ar[0] + a.ar[1] * b.ar[2];
		mat.ar[1] = a.ar[0] * b.ar[1] + a.ar[1] * b.ar[3];
		mat.ar[2] = a.ar[2] * b.ar[0] + a.ar[3] * b.ar[2];
		mat.ar[3] = a.ar[2] * b.ar[1] + a.ar[3] * b.ar[3];
		return mat;
	}
};

void TestWithMatrix()
{
	const size_t size = 2;
	const size_t maxElement = 10;
	const size_t test_quantity = 1000;

	Matrix m;
	SegmentTree<Matrix, SumMatrix, MultMatrix> segment_tree(size, m);
	Sequence<Matrix, SumMatrix, MultMatrix> sequence(size, m);

	for (size_t test_index = 0; test_index < test_quantity; ++test_index)
	{
		int task = rand() % 3;
		size_t start = rand() % size;
		size_t finish = start + (rand() % (size - start)) + 1;
		switch (task)
		{
		case 0: {
//					std::cerr << "\nc[" << start << ',' << finish << ") " << test_index << "\n";
					if (segment_tree.Combine(start, finish) != sequence.Combine(start, finish))
						throw std::logic_error("TestMatrixCombine: FAIL");
					break;
		}
		case 1: {
					Matrix rand_matrix;
					rand_matrix.LoadRandom(maxElement);
//					std::cerr << "\ns[" << start << ',' << finish << ") " << test_index << "\n";;
//					std::cerr << rand_matrix.ar[0] << " " << rand_matrix.ar[1] << "\n" << rand_matrix.ar[1] << " " << rand_matrix.ar[1] << "\n";
					segment_tree.Set(start, finish, rand_matrix);
					sequence.Set(start, finish, rand_matrix);
					break;
		}
		case 2: {
					Matrix rand_matrix;
//					std::cerr << "\nm[" << start << ',' << finish << ") " << test_index << "\n";;
//					std::cerr << rand_matrix.ar[0] << " " << rand_matrix.ar[1] << "\n" << rand_matrix.ar[1] << " " << rand_matrix.ar[1] << "\n";
					rand_matrix.LoadRandom(maxElement);
					segment_tree.Modify(start, finish, rand_matrix);
					sequence.Modify(start, finish, rand_matrix);
		}
		}
	}

}


struct ToggleCaseModifier {
	std::string operator() (const std::string& /*arg*/, std::string string) const
	{
		for (size_t index = 0; index < string.size(); ++index) {
			if (::isupper(static_cast<unsigned char>(string[index]))) {
				string[index] = ::tolower(static_cast<unsigned char>(string[index]));
			} else {
				string[index] = ::toupper(static_cast<unsigned char>(string[index]));
			}
		}
		return string;
	}
};

struct MaxCombiner{
	template<class T>
	T operator() (T a, T b) const
	{
		if (a > b)
			return a;
		else
			return b;
	}
};

void TestWithString1()
{
	SegmentTree<std::string, std::plus<std::string>, ToggleCaseModifier> segment_tree(127, "");
	for (char c = 'A'; c <= 'Z'; ++c) {
		segment_tree.Set(c, c + 1, std::string(1, c));
	}
	for (char c = 'a'; c <= 'z'; ++c) {
		segment_tree.Set(c, c + 1, std::string(1, c));
	}

	for (char c = 'A'; c <= 'Z'; ++c) {
		if (segment_tree.Combine(c, c + 1) != std::string(1, c)) {
			throw std::logic_error("TestStringConcatenation: FAIL");
		}
	}
	for (char c = 'a'; c <= 'z'; ++c) {
		if (segment_tree.Combine(c, c + 1) != std::string(1, c)) {
			throw std::logic_error("TestStringConcatenation: FAIL");
		}
	}

	if (segment_tree.Combine('A', 'Z' + 1) != "ABCDEFGHIJKLMNOPQRSTUVWXYZ") {
		throw std::logic_error("TestStringConcatenation: FAIL");
	}
	if (segment_tree.Combine('a', 'z' + 1) != "abcdefghijklmnopqrstuvwxyz") {
		throw std::logic_error("TestStringConcatenation: FAIL");
	}

	segment_tree.Modify('a', 'h' + 1, "");
	if (segment_tree.Combine('a', 'z' + 1) != "ABCDEFGHijklmnopqrstuvwxyz") {
		throw std::logic_error("TestStringConcatenation: FAIL");
	}

	segment_tree.Modify('e', 't' + 1, "");
	if (segment_tree.Combine('a', 'z' + 1) != "ABCDefghIJKLMNOPQRSTuvwxyz") {
		throw std::logic_error("TestStringConcatenation: FAIL");
	}
}

void TestWithString2()
{
	SegmentTree<std::string, std::plus<std::string>, ToggleCaseModifier> segment_tree(100, "a");
	Sequence<std::string, std::plus<std::string>, ToggleCaseModifier> sequence(100, "a");

	if (segment_tree.Combine(0, 100) != sequence.Combine(0, 100)) 
		throw std::logic_error("TestString2Combine: FAIL");

	segment_tree.Set(10, 50, std::string(1, 'A'));
	sequence.Set(10, 50, std::string(1, 'A'));

	if (segment_tree.Combine(0, 100) != sequence.Combine(0, 100)) 
		throw std::logic_error("TestString2Set1: FAIL");

	segment_tree.Set(30, 80, std::string(1, 'b'));
	sequence.Set(30, 80, std::string(1, 'b'));

	if (segment_tree.Combine(0, 100) != sequence.Combine(0, 100)) 
		throw std::logic_error("TestString2Set2: FAIL");

	
	sequence.Modify(20, 40, "");
	segment_tree.Modify(20, 40, "");

	if (segment_tree.Combine(20, 40) != sequence.Combine(20, 40)) {
		throw std::logic_error("TestString2Modify1: FAIL");
	}

	segment_tree.Modify(30, 100, "");
	sequence.Modify(30, 100, "");

	if (segment_tree.Combine(0, 100) != sequence.Combine(0, 100)) {
		throw std::logic_error("TestString2Modify2: FAIL");
	}
}

struct UpperCaseModifier {
std::string operator() (const std::string& /*arg*/, std::string string) const
{
	for (size_t index = 0; index < string.size(); ++index){
		string[index] = ::toupper(static_cast<unsigned char>(string[index]));
	}
	return string;
}
};

std::string GetRandomString(size_t randomStringLength)
{
	const std::string possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

	std::string randomString;
	for(size_t i = 0; i < randomStringLength; ++i)
	{
		size_t index = rand() % possibleCharacters.length();
		char nextChar = possibleCharacters.at(index);
		randomString.push_back(nextChar);
	}
	return randomString;
}

void TestWithString3()
{
	const size_t size = 1000;
	const size_t randomStringLength = 50;
	const size_t test_quantity = 1000;

	SegmentTree<std::string, std::plus<std::string>, UpperCaseModifier> segment_tree(size, "");
	Sequence<std::string, std::plus<std::string>, UpperCaseModifier> sequence(size, "");

	for (size_t test_index = 0; test_index < test_quantity; ++test_index)
	{
		int task = rand() % 3;
		size_t start  = rand() % size;
		size_t finish = start + (rand() % (size-start)) + 1;
		switch (task)
		{
		case 0: {
//				std::cerr << "c[" << start << ',' << finish << ") " << test_index << "\n";
				if (segment_tree.Combine(start, finish) != sequence.Combine(start, finish))
				throw std::logic_error("TestString3Combine: FAIL");
				break;
			}
		case 1: {
				std::string random_string = GetRandomString(randomStringLength);
//				std::cerr << "s[" << start << ',' << finish << ") " << random_string << " " << test_index << "\n";
				segment_tree.Set(start, finish, random_string);
				sequence.Set(start, finish, random_string);
				break;
			}
		case 2: {
//				std::cerr << "m[" << start << ',' << finish << ")" << test_index << "\n";;
				segment_tree.Modify(start, finish, "");
				sequence.Modify(start, finish, "");
			}
		}
	}

}

void TestWithInt()
{
	SegmentTree<int, std::plus<int>, std::multiplies<int>> segment_tree(16, 1);
	Sequence<int, std::plus<int>, std::multiplies<int>> sequence(16, 1);

	if (segment_tree.Combine(0, 16) != sequence.Combine(0, 16)) 
		throw std::logic_error("TestIntCombine: FAIL");

	for (int i = 0; i <= 6; i++) {
		segment_tree.Set(i, i + 1, 2);
		sequence.Set(i, i + 1, 2);
	}

	if (segment_tree.Combine(0, 5) != sequence.Combine(0, 5)) 
		throw std::logic_error("Test1IntSet: FAIL");

	for (int i = 3; i <= 10; i++) {
		segment_tree.Set(i, i + 1, 3);
		sequence.Set(i, i + 1, 3);
	}

	if (segment_tree.Combine(2, 9) != sequence.Combine(2, 9)) 
		throw std::logic_error("Test2IntSet: FAIL");

	segment_tree.Modify(0, 10, 2);
	sequence.Modify(0, 10, 2);
	if (segment_tree.Combine(0, 16) != sequence.Combine(0, 16)) {
		throw std::logic_error("Test1IntModify: FAIL");
	}

	segment_tree.Modify(5, 16, 7);
	sequence.Modify(5, 16, 7);
	if (segment_tree.Combine(0, 16) != sequence.Combine(0, 16)) {
		throw std::logic_error("Test2IntModify: FAIL");
	}
}

void TestWithDoubles()
{
	SegmentTree<double, MaxCombiner, std::multiplies<double>> segment_tree(50, 1);
	Sequence<double, MaxCombiner, std::multiplies<double>> sequence(50, 1);

	if (segment_tree.Combine(20, 40) != sequence.Combine(20, 40)) 
		throw std::logic_error("TestDoubleCombine: FAIL");

	segment_tree.Set(10, 44, 2);
	sequence.Set(10, 44, 2);

	if (segment_tree.Combine(0, 50) != sequence.Combine(0, 50)) 
		throw std::logic_error("Test1DoubleSet: FAIL");

	segment_tree.Set(30, 50, 2);
	sequence.Set(30, 50, 2);

	if (segment_tree.Combine(10, 50) != sequence.Combine(10, 50)) 
		throw std::logic_error("Test2DoubleSet: FAIL");

	segment_tree.Modify(5, 10, 3);
	sequence.Modify(5, 10, 3);
	if (segment_tree.Combine(0, 16) != sequence.Combine(0, 16)) {
		throw std::logic_error("Test1DoubleModify: FAIL");
	}

	segment_tree.Modify(0, 50, 7);
	sequence.Modify(0, 50, 7);
	if (segment_tree.Combine(0, 50) != sequence.Combine(0, 50)) {
		throw std::logic_error("Test2DoubleModify: FAIL");
	}
}


void TestCatString()
{
	size_t n = 1000;
	SegmentTree<std::string, std::plus<std::string>, UpperCaseModifier> segment_tree(n, "");
	Sequence<std::string, std::plus<std::string>, UpperCaseModifier> sequence(n, "");

	/*	std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<size_t> int_distribution(0, n);
	std::uniform_int_distribution<char> char_distribution('a', 'z');
	std::uniform_int_distribution<int> bool_distribution(0, 1);
	*/
	for (size_t i = 0; i < 10000; ++i) {
		size_t l = rand() % n;
		size_t r = rand() % n;
		char c = rand() % 128;
		if (l == r) continue;
		else if (r < l) std::swap(l, r);
		if (rand() % 2) {
			segment_tree.Set(l, r, std::string(1, c));
			sequence.Set(l, r, std::string(1, c));
		}
		else {
			segment_tree.Modify(l, r, "");
			sequence.Modify(l, r, "");
		}
	}

	if (segment_tree.Combine(0, n) != sequence.Combine(0, n)) {
		throw std::logic_error(": FAIL on test 1");
	}

	for (size_t i = 0; i < 10000; ++i) {
		size_t l = rand() % n;
		size_t r = rand() % n;
		if (l == r) continue;
		else if (r < l) std::swap(l, r);
		if (segment_tree.Combine(l, r) != sequence.Combine(l, r)) {
			throw std::logic_error(": FAIL on test 2");
		}
	}
}

} // namespace


void TestSegmentTree()
{
	try {
		TestWithString1();
		TestWithString2();
		TestWithString3();
		TestWithInt();
		TestWithDoubles();
		TestWithMatrix();
		TestCatString();

	} catch (const std::exception& ex) {
		throw std::logic_error(std::string("TestSegmentTree: ") + ex.what());
	}
}
