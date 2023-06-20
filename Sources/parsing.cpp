#include "scop.h"

/* check if line starting with "vnt " is correct */
t_vertex get_vertex( std::string line, size_t index, bool texture )
{
	t_vertex new_vertex;
	size_t tmp_index;

	new_vertex.x = std::stod(line.substr(index), &tmp_index);
	index += tmp_index;
	if (line[index] != ' ') {
		throw InvalidVertexException();
	}
	new_vertex.y = std::stod(line.substr(index + 1), &tmp_index);
	index += tmp_index + 1;
	if (line[index] != ' ') {
		throw InvalidVertexException();
	}
	if (texture) {
		return (new_vertex);
	}
	new_vertex.z = std::stod(line.substr(index + 1), &tmp_index);
	index += tmp_index + 1;
	if (index != line.size()) {
		throw InvalidVertexException();
	}
	return (new_vertex);
}

/* read .obj file provided and store its informations if they are correct */
void Scop::parse( std::string file )
{
	std::ifstream indata(file.c_str());
	if (!indata.is_open()) {
		throw InvalidFileException();
	}
	std::string line;
	while (!indata.eof()) {
		std::getline(indata, line);
		line = trim_spaces(line);
		// display_special_characters(line);
		if (line.empty() || line[0] == '#') {
			continue ;
		} else if (!line.compare(0, 2, "v ")) {
			_vertices.push_back(get_vertex(line, 2, false));
		} else if (!line.compare(0, 3, "vt ")) {
			_vertices_textures.push_back(get_vertex(line, 3, true));
		} else if (!line.compare(0, 3, "vn ")) {
			_vertices_normals.push_back(get_vertex(line, 3, false));
		} else if (!line.compare(0, 2, "f ")) {
			_faces.push_back(get_face(line));
		}
	}
	indata.close();
}
