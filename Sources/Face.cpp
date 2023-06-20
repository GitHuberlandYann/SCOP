#include "scop.h"

Face::Face( void ) : _size(0)
{

}

Face::~Face( void )
{

}

void Face::add_vertex( t_vertex *vertex, t_vertex *texture, t_vertex *normal )
{
	if (vertex) {
		_vertices.push_back(*vertex);
	}
	if (texture) {
		_vertices_textures.push_back(*texture);
	}
	if (normal) {
		_vertices_normals.push_back(*normal);
	}
	++_size;
}

size_t Face::get_size( void )
{
	return (_size);
}