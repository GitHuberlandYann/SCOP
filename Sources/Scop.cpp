#include "scop.h"

Scop::Scop( void )
{
	std::cout << "Constructor of Scop called" << std::endl;
}

Scop::~Scop( void )
{
	std::cout << "Destructor of Scop called" << std::endl;

	_vertices.clear();
}

void Scop::display_content( void )
{
	std::cout << std::endl << " ---- SCOP content ----" << std::endl << std::endl;
	std::cout << "\t-Number of vertices: " << _vertices.size() << std::endl;
	std::cout << std::endl << " ----------------------" << std::endl << std::endl;
}
