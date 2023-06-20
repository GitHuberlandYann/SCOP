#include "scop.h"

/* check if line starting with "v " is correct */
void Scop::add_vertex( std::string line )
{
	t_vertex new_vertex;
	size_t index;
	size_t tmp_index;

	new_vertex.x = std::stod(line.substr(3), &tmp_index);
	index = tmp_index + 3;
	// std::cout << "index: " << index << std::endl;
	if (line[index] != ' ') {
		throw InvalidVertexException();
	}
	new_vertex.y = std::stod(line.substr(index + 1), &tmp_index);
	index += tmp_index + 1;
	// std::cout << "index: " << index << std::endl;
	if (line[index] != ' ') {
		throw InvalidVertexException();
	}
	new_vertex.z = std::stod(line.substr(index + 1), &tmp_index);
	index += tmp_index + 1;
	// std::cout << "index: " << index << std::endl;
	if (index != line.size()) {
		throw InvalidVertexException();
	}
	_vertices.push_back(new_vertex);
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
		// std::cout << "line: " << line << std::endl;
		if (line.empty() || line[0] == '#') {
			continue ;
		} else if (!line.compare(0, 2, "v ")) {
			add_vertex(line);
		}
	}
	indata.close();
}
