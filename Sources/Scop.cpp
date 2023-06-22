#include "scop.h"

Scop::Scop( std::string root ) : _face_mode(UNSET), _max_box{-10000, -10000, -10000},
		_min_box{10000, 10000, 10000}, _current_used_material(NULL), _root(root)
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

/* use minmax_box to center object on {0, 0, 0} */
void Scop::center_object( void )
{
	t_vertex central_axis = {(_max_box.x + _min_box.x) / 2,
								(_max_box.y + _min_box.y) / 2,
								(_max_box.z + _min_box.z) / 2};
	
	// std::cout << "max: {" << _max_box.x << ", " << _max_box.y << ", " << _max_box.z << "}" << std::endl;
	// std::cout << "min: {" << _min_box.x << ", " << _min_box.y << ", " << _min_box.z << "}" << std::endl;
	// std::cout << "axis: {" << central_axis.x << ", " << central_axis.y << ", " << central_axis.z << "}" << std::endl;

	std::vector<Face *>::iterator it = _faces.begin();
	std::vector<Face *>::iterator ite = _faces.end();

	for (; it != ite; it++) {
		(*it)->center_object(central_axis);
	}
}

double Scop::get_extramum( void )
{
	double res = fabs(_max_box.x);
	if (fabs(_max_box.y) > res) {
		res = fabs(_max_box.y);
	}
	if (fabs(_max_box.z) > res) {
		res = fabs(_max_box.z);
	}
	if (fabs(_min_box.x) > res) {
		res = fabs(_min_box.x);
	}
	if (fabs(_min_box.y) > res) {
		res = fabs(_min_box.y);
	}
	if (fabs(_min_box.z) > res) {
		res = fabs(_min_box.z);
	}
	return (res);
}

void Scop::map_img( Mlx *mlx )
{
	std::vector<Face *>::iterator it = _faces.begin();
	std::vector<Face *>::iterator ite = _faces.end();

	for (; it != ite; it++) {
		(*it)->draw_face(mlx);
	}
}
