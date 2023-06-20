#include "scop.h"

/* check if line starting with "vnt " is correct */
static t_vertex parse_vertex( std::string line, size_t index, bool texture )
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
	if (texture) {
		return (new_vertex);
	}
	if (line[index] != ' ') {
		throw InvalidVertexException();
	}
	new_vertex.z = std::stod(line.substr(index + 1), &tmp_index);
	index += tmp_index + 1;
	if (index != line.size()) {
		throw InvalidVertexException();
	}
	return (new_vertex);
}

t_vertex *Scop::get_vertex( int num )
{
	if (num == 0) {
		throw InvalidFaceException();
	} else if (num < 0) {
		num += _vertices.size() + 1;
		if (num < 0) {
			throw InvalidFaceException();
		}
	} else if (num > (int) _vertices.size()) {
		throw InvalidFaceException();
	}
	return (&_vertices[num - 1]);
}

t_vertex *Scop::get_texture( int num )
{
	if (num == 0) {
		throw InvalidFaceException();
	} else if (num < 0) {
		num += _vertices_textures.size() + 1;
		if (num < 0) {
			throw InvalidFaceException();
		}
	} else if (num > (int) _vertices_textures.size()) {
		throw InvalidFaceException();
	}
	return (&_vertices_textures[num - 1]);
}

t_vertex *Scop::get_normal( int num )
{
	if (num == 0) {
		throw InvalidFaceException();
	} else if (num < 0) {
		num += _vertices_normals.size() + 1;
		if (num < 0) {
			throw InvalidFaceException();
		}
	} else if (num > (int) _vertices_normals.size()) {
		throw InvalidFaceException();
	}
	return (&_vertices_normals[num - 1]);
}

/* parse v[/vt/vn] and get given vertices from storage */
void Scop::add_vertex_face( Face *face, std::string line, size_t & index )
{
	size_t tmp_index;

	int v_num = std::stoi(line.substr(index), &tmp_index);
	int vt_num = 0;
	int vn_num = 0;
	index += tmp_index;
	if (line[index] == ' ' || !line[index]) {
		if (_face_mode == UNSET) {
			_face_mode = ONLY_V;
		} else if (_face_mode != ONLY_V) {
			throw InvalidFaceException();
		}
	} else if (line[index] != '/') {
		throw InvalidFaceException();
	} else {
		if (line[index + 1] == '/') {
			if (_face_mode == UNSET) {
				_face_mode = ONLY_VN;
			} else if (_face_mode != ONLY_VN) {
				throw InvalidFaceException();
			}
			vn_num = std::stoi(line.substr(index + 2), &tmp_index);
			index += tmp_index + 2;
			if (line[index] && line[index] != ' ') {
				throw InvalidFaceException();
			}
		} else if (std::isdigit(line[index + 1]) || line[index + 1] == '-') {
			vt_num = std::stoi(line.substr(index + 1), &tmp_index);
			index += tmp_index + 1;
			if (line[index] == ' ' || !line[index]) {
				if (_face_mode == UNSET) {
					_face_mode = ONLY_VT;
				} else if (_face_mode != ONLY_VT) {
					throw InvalidFaceException();
				}
			} else if (line[index] != '/') {
				throw InvalidFaceException();
			} else {
				if (std::isdigit(line[index + 1]) || line[index + 1] == '-') {
					vn_num = std::stoi(line.substr(index + 1), &tmp_index);
					index += tmp_index + 1;
					if (line[index] == ' ' || !line[index]) {
						if (_face_mode == UNSET) {
							_face_mode = VTN;
						} else if (_face_mode != VTN) {
							throw InvalidFaceException();
						}
					} else {
						throw InvalidFaceException();
					}
				} else {
					throw InvalidFaceException();
				}
			}
		} else {
			throw InvalidFaceException();
		}
	}

	switch (_face_mode) {
		case UNSET:
			throw InvalidFaceException();
		case ONLY_V:
			face->add_vertex(get_vertex(v_num), NULL, NULL);
			break;
		case ONLY_VT:
			face->add_vertex(get_vertex(v_num), get_texture(vt_num), NULL);
			break;
		case ONLY_VN:
			face->add_vertex(get_vertex(v_num), NULL, get_normal(vn_num));
			break;
		case VTN:
			face->add_vertex(get_vertex(v_num), get_texture(vt_num), get_normal(vn_num));
	}
}

/* check if face line is correct, return instance of class Face */
void Scop::get_face( std::string line )
{
	Face *new_face = new Face();
	_faces.push_back(new_face);
	size_t index = 2;

	while (std::isdigit(line[index]) || line[index] == '-') {
		add_vertex_face( new_face, line, index );
		if (line[index] == ' ') {
			++index;
		}
	}
	size_t face_size = new_face->get_size();
	if (face_size < 3) {
		throw InvalidFaceException();
	}
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
			_vertices.push_back(parse_vertex(line, 2, false));
		} else if (!line.compare(0, 3, "vt ")) {
			_vertices_textures.push_back(parse_vertex(line, 3, true));
		} else if (!line.compare(0, 3, "vn ")) {
			_vertices_normals.push_back(parse_vertex(line, 3, false));
		} else if (!line.compare(0, 2, "f ")) {
			get_face(line);
		}
	}
	indata.close();
}
