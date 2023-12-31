#include "Exception.hpp"

const char* InvalidFileException::what() const throw()
{
	return ("[InvalidFileException] Input file could not be opened.");
}

const char* InvalidMltFileException::what() const throw()
{
	return ("[InvalidMltFileException] Mlt file could not be opened.");
}

const char* InvalidVertexException::what() const throw()
{
	return ("[InvalidVertexException] Vertex line (v, vt, vn, kd) is invalid.");
}

const char* InvalidFaceException::what() const throw()
{
	return ("[InvalidFaceException] Face line is invalid.");
}

const char* DoubleMltlibException::what() const throw()
{
	return ("[DoubleMltlibException] Two or more lines begin with mltlib.");
}

const char* NoMltlibFileException::what() const throw()
{
	return ("[NoMltlibFileException] mltlib line is missing a file.");
}

const char* MltExtensionException::what() const throw()
{
	return ("[MltExtensionException] mltlib must be .mlt file.");
}

const char* InvalidMltException::what() const throw()
{
	return ("[InvalidMltException] Error in .mlt file.");
}

const char* NoMatchingMaterialException::what() const throw()
{
	return ("[NoMatchingMaterialException] No matching material in .mlt file.");
}

const char* InvalidRGBException::what() const throw()
{
	return ("[InvalidRGBException] RGB in .mlt must be in range [0:1].");
}

const char* EmptyObjectException::what() const throw()
{
	return ("[EmptyObjectException] .obj has no faces.");
}

const char* MlxCallException::what() const throw()
{
	return ("[MlxCallException] Call to mlx function failed.");
}

const char* QuitProgramException::what() const throw()
{
	return ("[QuitProgramException] You successfully quit the program.");
}
