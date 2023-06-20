#include "scop.h"

Material::Material( std::string name, std::ifstream & indata ) : _name(name), _color{255, 255, 255}
{
	// std::cout << "Constructor of Material called" << std::endl;
	if (name.empty()) {
		throw InvalidMltException();
	}

	std::string line;
	while (!indata.eof()) {
		std::getline(indata, line);
		line = trim_spaces(line);
		// display_special_characters(line);
		if (line.empty() || line[0] == '#') {
			continue ;
		} else if (!line.compare(0, 3, "kd ")) {
			set_rgb(line);
		} else if (!line.compare(0, 7, "map_kd ")) {
			continue ; //TODO add texture_img to scop and remember index here
		} else if (!line.compare(0, 7, "newmtl ")) {
			return ;
		}
	}
}

Material::~Material( void )
{
	// std::cout << "Destructor of Material called" << std::endl;
}

// ************************************************************************** //
//                                  Private                                   //
// ************************************************************************** //

void Material::set_rgb( std::string line )
{
	t_vertex rgb_01 = parse_vertex(line, 3, false);

	if (rgb_01.x < 0 || rgb_01.x > 1 || rgb_01.y < 0 || rgb_01.y > 1
		|| rgb_01.z < 0 || rgb_01.z > 1) {
		throw InvalidRGBException();
	}
	_color[0] = rgb_01.x * 255;
	_color[1] = rgb_01.y * 255;
	_color[2] = rgb_01.z * 255;
}
