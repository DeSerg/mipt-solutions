#pragma once

#include <istream>
#include <ostream>
#include <string>
#include <vector>


// Serialize int
inline void serialize(int value, std::ostream* ostream)
{
	ostream->write(reinterpret_cast<char *>(&value), sizeof(value));
	if (ostream->fail()) 
		throw std::runtime_error("serialize<int>: Fail");        
}

// Deserialize int
inline bool deserialize(std::istream* istream, int* value)
{
	istream->read(reinterpret_cast<char *>(value), sizeof(int));

	if (istream->eof())
		return false;

	if (istream->fail())
		throw std::runtime_error("deserialize<int>: Fail");        
	return true;
}



// Serialize double
inline void serialize(double value, std::ostream* ostream)
{
ostream->write(reinterpret_cast<char *>(&value), sizeof(value));
	if (ostream->fail())
			throw std::runtime_error("serialize<double>: Fail");
}

// Deserialize double
inline bool deserialize(std::istream* istream, double* value)
{
istream->read(reinterpret_cast<char *>(value),sizeof(double));

	if (istream->eof())
			return false;

	if (istream->fail())
			throw std::runtime_error("deserialize<double>: Fail");

	return true;
}



// Serialize string
inline void serialize(const std::string& value, std::ostream* ostream)
{
	const size_t string_size = value.size();

	ostream->write(reinterpret_cast<const char *>(&string_size), sizeof(string_size));
	if (ostream->fail()) 
			throw std::runtime_error("serialize<string>: Fail");

	ostream->write(value.data(), string_size*sizeof(char));        
	if (ostream->fail()) 
			throw std::runtime_error("serialize<string>: Fail");
}

// Deserialize string
inline bool deserialize(std::istream* istream, std::string* value)
{
	size_t string_size;

	istream->read(reinterpret_cast<char *>(&string_size), sizeof(string_size));
	if (istream->eof())
			return false;
	if (istream->fail())
			throw std::runtime_error("deserialize<string>: Fail");

	if (string_size)
	{
			value->resize(string_size);

			istream->read(&(*value)[0], string_size*sizeof(char));
			if (istream->eof())
			{
				return false;
				throw std::runtime_error("deserialize<string>: Fail");
			}
			if (istream->fail())
				throw std::runtime_error("deserialize<string>: Fail");
	}
	else
	{
			*(value)="";
	}
	return true;        

}


//serialize vector
template <class T> void serialize(const std::vector<T>& value, std::ostream* ostream)
{
	int vector_size = value.size();
	serialize(vector_size, ostream);
	for (int i = 0; i < value.size(); i++)
	{
			serialize(value[i], ostream);
	}
}

//deserialize vector
template <class T> bool deserialize(std::istream* istream, std::vector<T>* value)
{
	size_t vector_size;
	if (!deserialize(istream, reinterpret_cast<int *>(&vector_size)))
			return false;
	value->resize(vector_size);

	for (size_t i = 0; i < value->size(); i++)
	{
			if ( !deserialize(istream, &((*value)[i])) )
			{
				throw std::runtime_error("deserialize<vector>: Fail");
				return false;
			}
	}
	return true;
}
