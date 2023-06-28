#include "scop.h"

Scop::Scop( std::string root ) : _face_mode(UNSET), _current_used_material(NULL), _root(root)
{
	std::cout << "Constructor of Scop called" << std::endl;
	set_vertex(_max_box, -10000, -10000, -10000);
	set_vertex(_min_box, 10000, 10000, 10000);

	t_vertex box0 = {-50, -50, -50};
	t_vertex box1 = {-50, 50, -50};
	t_vertex box2 = {50, 50, -50};
	t_vertex box3 = {50, -50, -50};
	t_vertex box4 = {-50, -50, 50};
	t_vertex box5 = {-50, 50, 50};
	t_vertex box6 = {50, 50, 50};
	t_vertex box7 = {50, -50, 50};

	Face *new_face = new Face(NULL, 0);
	_box.push_back(new_face);
	new_face->add_vertex(&box0, NULL, NULL);
	new_face->add_vertex(&box1, NULL, NULL);
	new_face->add_vertex(&box2, NULL, NULL);
	new_face->add_vertex(&box3, NULL, NULL);

	new_face = new Face(NULL, 0);
	_box.push_back(new_face);
	new_face->add_vertex(&box4, NULL, NULL);
	new_face->add_vertex(&box5, NULL, NULL);
	new_face->add_vertex(&box6, NULL, NULL);
	new_face->add_vertex(&box7, NULL, NULL);

	new_face = new Face(NULL, 0);
	_box.push_back(new_face);
	new_face->add_vertex(&box0, NULL, NULL);
	new_face->add_vertex(&box1, NULL, NULL);
	new_face->add_vertex(&box5, NULL, NULL);
	new_face->add_vertex(&box4, NULL, NULL);

	new_face = new Face(NULL, 0);
	_box.push_back(new_face);
	new_face->add_vertex(&box1, NULL, NULL);
	new_face->add_vertex(&box2, NULL, NULL);
	new_face->add_vertex(&box6, NULL, NULL);
	new_face->add_vertex(&box5, NULL, NULL);

	new_face = new Face(NULL, 0);
	_box.push_back(new_face);
	new_face->add_vertex(&box2, NULL, NULL);
	new_face->add_vertex(&box3, NULL, NULL);
	new_face->add_vertex(&box7, NULL, NULL);
	new_face->add_vertex(&box6, NULL, NULL);

	new_face = new Face(NULL, 0);
	_box.push_back(new_face);
	new_face->add_vertex(&box0, NULL, NULL);
	new_face->add_vertex(&box3, NULL, NULL);
	new_face->add_vertex(&box7, NULL, NULL);
	new_face->add_vertex(&box4, NULL, NULL);
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

	std::vector<Face *>::iterator itb = _box.begin();
	std::vector<Face *>::iterator iteb = _box.end();
	
	for (; itb != iteb; itb++) {
		delete *itb;
	}
	_box.clear();

	std::vector<Material *>::iterator mit = _materials.begin();
	std::vector<Material *>::iterator mite = _materials.end();
	
	for (; mit != mite; mit++) {
		delete *mit;
	}
	_materials.clear();
}

// ************************************************************************** //
//                                  Private                                   //
// ************************************************************************** //

double Scop::get_extremum( void )
{
	double res = _max_box.x;
	if (_max_box.y > res) {
		res = _max_box.y;
	}
	if (_max_box.z > res) {
		res = _max_box.z;
	}
	if (-_min_box.x > res) {
		res = -_min_box.x;
	}
	if (-_min_box.y > res) {
		res = -_min_box.y;
	}
	if (-_min_box.z > res) {
		res = -_min_box.z;
	}
	return (res);
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
	std::cout << "\t-box x[" << _min_box.x << ':' << _max_box.x << ']' << std::endl;
	std::cout << "\t     y[" << _min_box.y << ':' << _max_box.y << ']' << std::endl;
	std::cout << "\t     z[" << _min_box.z << ':' << _max_box.z << ']' << std::endl;
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
	
	_max_box.x -= central_axis.x;
	_max_box.y -= central_axis.y;
	_max_box.z -= central_axis.z;
	_min_box.x -= central_axis.x;
	_min_box.y -= central_axis.y;
	_min_box.z -= central_axis.z;

	std::vector<Face *>::iterator it = _faces.begin();
	std::vector<Face *>::iterator ite = _faces.end();

	for (; it != ite; it++) {
		(*it)->center_object(central_axis, EXTREMUM / get_extremum());
		(*it)->set_face_normal();
	}
}

void Scop::map_img( Mlx *mlx )
{
	std::vector<Face *>::iterator it = _faces.begin();
	std::vector<Face *>::iterator ite = _faces.end();

	for (; it != ite; it++) {
		(*it)->draw_face(mlx);
	}
}

void Scop::map_box( Mlx *mlx )
{
	std::vector<Face *>::iterator it = _box.begin();
	std::vector<Face *>::iterator ite = _box.end();

	for (; it != ite; it++) {
		(*it)->draw_face(mlx);
	}
}
