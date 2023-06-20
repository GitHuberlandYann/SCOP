#include "Exception.hpp"

const char* InvalidFileException::what() const throw()
{
	return ("[InvalidFileException] Input file could not be opened.");
}

const char* InvalidVertexException::what() const throw()
{
	return ("[InvalidVertexException] Vertex line (v, vt, vn) is invalid.");
}
