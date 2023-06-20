#include "Exception.hpp"

const char* InvalidFileException::what() const throw()
{
	return ("[InvalidFileException] Input file could not be opened.");
}

const char* InvalidVertexException::what() const throw()
{
	return ("[InvalidVertexException] Line starting with 'v ' is invalid.");
}
