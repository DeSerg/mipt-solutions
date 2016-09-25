#include "serialization.h"
#include <stdexcept>
#include <sstream>



//Test serialization of integer numbers.
void testSerializationInt()
{
	static const int INTEGERS[] = {
		0, 1, -8, 64, -512, 4096, -32768, 262144,
		-2097152, 16777216, -134217728, 1073741824,
		-2147483648
	};
	static const size_t INTEGERS_SIZE = sizeof(INTEGERS) / sizeof(INTEGERS[0]);

	std::ostringstream ostream;	
	for (size_t i = 0; i < INTEGERS_SIZE; ++i) {
		serialize(INTEGERS[i], &ostream);
	}

	std::istringstream istream(ostream.str());
	for (size_t i = 0; i < INTEGERS_SIZE; ++i) {
		int value;
		if (!deserialize(&istream, &value) || value != INTEGERS[i]) {
			throw std::logic_error("testSerialization: integer: FAIL");
		}
	}

	int value;
	if (deserialize(&istream, &value)) {
		throw std::logic_error("testSerialization: integer: FAIL");
	}
}

//Test serialization of real numbers.
void testSerializationDouble()
{
	static const double DOUBLES[] = {
		0.0, 1.5, -1.5, 12345.678910,
		999.9999, -9876543210.012345,
		1e10, 1e-10,
		-3.4e38, 3.4e38,
		-3.4e-38, 3.4e-38
	};
	static const size_t DOUBLES_SIZE = sizeof(DOUBLES) / sizeof(DOUBLES[0]);

	std::ostringstream ostream;
	for (size_t i = 0; i < DOUBLES_SIZE; ++i) {
		serialize(DOUBLES[i], &ostream);
	}

	std::istringstream istream(ostream.str());
	for (size_t i = 0; i < DOUBLES_SIZE; ++i) {
		double value;
		if (!deserialize(&istream, &value) || value != DOUBLES[i]) {
			throw std::logic_error("testSerialization: double : FAIL");
		}
	}

	double value;
	if (deserialize(&istream, &value)) {
		throw std::logic_error("testSerialization: double: FAIL");
	}
}

//Test serialization of string numbers.
void testSerializationString()
{
	static const std::string STRINGS[] = {
		"",
		"a", "b", "c", "\0\0", "\n", "\r", "\r\n", "\"",
		"Hello", "World", "Hello \n World\0\0",
		"x yy zzz"
	};
	static const size_t STRINGS_SIZE = sizeof(STRINGS) / sizeof(STRINGS[0]);

	std::ostringstream ostream;
	for (size_t i = 0; i < STRINGS_SIZE; i++)
	{
			serialize(STRINGS[i], &ostream);
	}
        
	std::istringstream istream(ostream.str());
	for (size_t i = 0; i < STRINGS_SIZE; i++)
	{
		std::string value;
		if (!deserialize(&istream, &value) || value != STRINGS[i])
		throw std::runtime_error("testSerialization: string: FAIL");
	}
	
	std::string value;
	if (deserialize(&istream, &value))
			throw std::runtime_error("testSerialization: string: FAIL");
}


void testSerializationVectorInt()
{
	std::vector<int> VECTOR;

	VECTOR.push_back(0);
	VECTOR.push_back(1);
	VECTOR.push_back(-8);
	VECTOR.push_back(64);
	VECTOR.push_back(-512);
	VECTOR.push_back(16777216);
	VECTOR.push_back(-134217728);
	VECTOR.push_back(1073741824);
	VECTOR.push_back(-2147483648);

	std::ostringstream ostream;
	serialize(VECTOR, &ostream);

	std::istringstream istream(ostream.str());
	std::vector<int> value;
	if (!deserialize(&istream, &value) || value!=VECTOR) 
		throw std::runtime_error("testSerialization: vector: int:  FAIL");

}

void testSerializationVectorDouble()
{
	std::vector<double> VECTOR;

	VECTOR.push_back(0.0);
	VECTOR.push_back(1.11111);
	VECTOR.push_back(-8.88888);
	VECTOR.push_back(64.64646464);
	VECTOR.push_back(-512.512512512);
	VECTOR.push_back(16777216.16777216);
	VECTOR.push_back(1e10);
	VECTOR.push_back(1e-10);
	VECTOR.push_back(-1e10);

	std::ostringstream ostream;
	serialize(VECTOR, &ostream);

	std::istringstream istream(ostream.str());
	std::vector<double> value;
	if (!deserialize(&istream, &value) || value!=VECTOR) 
			throw std::runtime_error("testSerialization: vector: double:  FAIL");

}

void testSerializationVectorString()
{
	std::vector<std::string> VECTOR;

	VECTOR.push_back("");
	VECTOR.push_back("Hello");
	VECTOR.push_back("World");
	VECTOR.push_back("Hello \n World\0\0");
	VECTOR.push_back("x yy zzz");

	std::ostringstream ostream;
	serialize(VECTOR, &ostream);

	std::istringstream istream(ostream.str());
	std::vector<std::string> value;
	if (!deserialize(&istream, &value) || value!=VECTOR) 
			throw std::runtime_error("testSerialization: vector: string:  FAIL");

}

//Test serialization of integer vector.
void testSerializationVector()
{
	testSerializationVectorInt();
	testSerializationVectorDouble();
	testSerializationVectorString();
}


void testSerialization()
{
	testSerializationInt();
	testSerializationDouble();
	testSerializationString();
	testSerializationVector();
}
