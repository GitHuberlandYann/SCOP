#include "scop.h"

Scop::Scop( std::string root ) : _root(root), _face_mode(UNSET), _current_used_material(NULL)
{
	std::cout << "Constructor of Scop called" << std::endl;
}

Scop::~Scop( void )
{
	std::cout << "Destructor of Scop called" << std::endl;

	_vertices.clear();
	_vertices_textures.clear();
	_vertices_normals.clear();

	std::vector<Face *>::iterator it = _faces.begin();
	std::vector<Face *>::iterator ite = _faces.end();
	
	for (; it != ite; it++) {
		delete *it;
	}
	_faces.clear();

	std::vector<Material *>::iterator mit = _materials.begin();
	std::vector<Material *>::iterator mite = _materials.end();
	
	for (; mit != mite; mit++) {
		delete *mit;
	}
	_materials.clear();
}

// ************************************************************************** //
//                                  Public                                    //
// ************************************************************************** //

void Scop::display_content( void )
{
	std::cout << std::endl << " ---- SCOP content ----" << std::endl << std::endl;
	std::cout << "\t-Number of vertices: " << _vertices.size() << std::endl;
	std::cout << "\t-Number of vertices_textures: " << _vertices_textures.size() << std::endl;
	std::cout << "\t-Number of vertices_normals: " << _vertices_normals.size() << std::endl;
	std::cout << "\t-Number of faces: " << _faces.size() << std::endl;
	std::cout << "\t-Number of materials: " << _materials.size() << std::endl;
	std::cout << std::endl << " ----------------------" << std::endl << std::endl;
}

void Scop::map_img( Mlx *mlx )
{
	std::vector<Face *>::iterator it = _faces.begin();
	std::vector<Face *>::iterator ite = _faces.end();

	for (; it != ite; it++) {
		(*it)->draw_face(mlx);
	}
}
