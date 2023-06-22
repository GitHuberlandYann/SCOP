#include "scop.h"

Material::Material( std::string name, std::ifstream & indata, std::string & line ) : _name(name), _color(0xffffff), _texture_index(std::string::npos)
{
	// std::cout << "Constructor of Material called" << std::endl;
	if (name.empty()) {
		throw InvalidMltException();
	}

	while (!indata.eof()) {
		std::getline(indata, line);
		line = trim_spaces(line);
		// display_special_characters(line);
		if (line.empty() || line[0] == '#') {
			continue ;
		} else if (!line.compare(0, 3, "Kd ")) {
			set_rgb(line);
		} else if (!line.compare(0, 7, "map_Kd ")) {
			_xpm_file = line.substr(7);
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
	_color = (int)(rgb_01.x * 255) * 0x10000;
	_color += (int)(rgb_01.y * 255) * 0x100;
	_color += (int)(rgb_01.z * 255);

	// std::cout << "rgb of material set to: " << _color << std::endl;
}

// ************************************************************************** //
//                                  Public                                    //
// ************************************************************************** //

unsigned int Material::get_color( void )
{
	return (_color);
}

std::string Material::get_name( void )
{
	return (_name);
}

std::string Material::get_xpm( void )
{
	return (_xpm_file);
}

size_t *Material::get_texture_index( void )
{
	return (&_texture_index);
}

void Material::set_texture_index( size_t index )
{
	_texture_index = (int)index;
}