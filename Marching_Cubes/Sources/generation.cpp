#include "marching.h"

void MarchingCube::gen_random( void )
{
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, 10);

	_array = new int[_size[0] * _size[1] * _size[2]];
	for (size_t z = 0; z < _size[2]; z++) {
		for (size_t y = 0; y < _size[1]; y++) {
			for (size_t x = 0; x < _size[0]; x++) {
				_array[z * _size[1] * _size[0] + y * _size[0] + x] = distribution(generator);
			}
		}
	}
}

void MarchingCube::gen_small_sphere( void )
{
	// set_size(2, 2, 2);
	set_size(3, 3, 3);
	_array = new int[_size[0] * _size[1] * _size[2]];
	for (size_t z = 0; z < _size[2]; z++) {
		for (size_t y = 0; y < _size[1]; y++) {
			for (size_t x = 0; x < _size[0]; x++) {
				_array[z * _size[1] * _size[0] + y * _size[0] + x] = (x == 1 && y == 1 && z == 1) * 7;
			}
		}
	}
}

void MarchingCube::gen_Perlin( void )
{
    const siv::PerlinNoise::seed_type seed = 123456u;
	const siv::PerlinNoise perlin{ seed };

	_array = new int[_size[0] * _size[1] * _size[2]];
	for (size_t z = 0; z < _size[2]; z++) {
		for (size_t y = 0; y < _size[1]; y++) {
			for (size_t x = 0; x < _size[0]; x++) {
				// double noise = perlin.octave3D_01((double)x / 10, (double)y / 10, (double)z / 10, 8) * 10.0;
				double noise = perlin.noise3D_01((double)x / 10, (double)y / 10, (double)z / 10) * 10.0;
				std::cout << "noise: " << noise;
				_array[z * _size[1] * _size[0] + y * _size[0] + x] = noise;
			}
		}
	}
}

void MarchingCube::set_cross_arrays( std::ifstream & indata )
{
	std::string line;
	_block = 0;
	while (!indata.eof()) {
		std::getline(indata, line);
		line = trim_spaces(line);
		if (line.empty() || line[0] == '#') {
			continue ;
		}
		std::cout << "current line: " << line << std::endl;
		if (!line.compare(0, 5, "size ")) {
			std::cout << "in size" << std::endl;
			if (_block) {
				std::cout<<"ERROR\n";
				throw std::exception(); //
			}
			size_t index = 5, ixyz = 0;
			
			while (std::isdigit(line[index]))
			{
				if (ixyz > 2) {
					std::cout<<"ERROR\n";
					
					throw std::exception(); //
				}
				std::istringstream iss(line.substr(index));
				int toint;
				iss >> toint;
				if (iss.fail()) {
					std::cout<<"ERROR\n"; 
					throw std::exception();
				}
				_size[ixyz] = toint;
				while (std::isdigit(line[index]))
					++index;
				if (line[index] && line[index] != ' ') {
					std::cout<<"ERROR\n";
					throw std::exception();
				}
				++index;
				++ixyz;
			}
			std::cout << "out size line with size " << _size[0] << ", " << _size[1] << ", " << _size[2] << std::endl;
		} else if (!line.compare(0, 6, "block")) {
			std::cout << "in block " << "xyz"[(_block == 1) * 2] << "xyz"[2 - (_block != 2)] << std::endl;
			if (_block > 2) {
				std::cerr<<"ERROR _block\n";
				throw std::exception(); //
			}
			_carrays[_block] = new int[_size[(_block == 1) * 2] * _size[2 - (_block != 2)]];
			std::cout << "malloc " << _size[(_block == 1) * 2] << " x " << _size[2 - (_block != 2)] << std::endl;
			size_t y = 0;
			while (!indata.eof()) {
				if (y == _size[2 - (_block != 2)]) {
					break ;
				}
				std::getline(indata, line);
				line = trim_spaces(line);
				size_t x = 0, index = 0;
				while (std::isdigit(line[index])) {
					if (x > _size[(_block == 1) * 2]) {
						std::cerr<<"ERROR 0\n";
						throw std::exception(); //
					}
					std::istringstream iss(line.substr(index));
					int toint;
					iss >> toint;
					if (iss.fail()) {
						std::cerr<<"ERROR 1\n";
						throw std::exception();
					}
					_carrays[_block][(_size[2 - (_block != 2)] - 1 - y) * _size[(_block == 1) * 2] + x] = toint;
					while (std::isdigit(line[index]))
						++index;
					if (line[index] && line[index] != ' ') {
						std::cerr<<"ERROR 2\n";
						throw std::exception();
					}
					++x;
					++index;
				}
				if (x != _size[(_block == 1) * 2]) {
					std::cerr<<"ERROR 3, x = " << x << ", _size[_bloc] = " << _size[(_block == 1) * 2] << "\n";
					throw std::exception(); //
				}
				y++;
			}
			std::cout << "out block " << _block << std::endl;
			++_block;
		} else {
			std::cerr<<"ERROR else\n";
			throw std::exception(); //
		}
	}
	if (_block != 3) {
		std::cerr << "wrong amount of blocks" << std::endl;
		throw std::exception();
	}
	std::cout << "out set cross" << std::endl;
}

void MarchingCube::gen_cross_array( void )
{
	_array = new int[_size[0] * _size[1] * _size[2]];
	for (size_t z = 0; z < _size[2]; z++) {
		for (size_t y = 0; y < _size[1]; y++) {
			for (size_t x = 0; x < _size[0]; x++) {
				// std::cout << x << '/' << _size[0] << ", " << y << '/' << _size[1] << ", " << z << '/' << _size[2] << ": " << _carrays[1][y * _size[2] + z]  << std::endl;
				_array[z * _size[1] * _size[0] + y * _size[0] + x] = (_carrays[0][y * _size[0] + x] && _carrays[1][y * _size[2] + z] && _carrays[2][z * _size[0] + x]) * 7;
			}
		}
	}
}

void MarchingCube::display( void )
{
	for (size_t z = 0; z < _size[2]; z++) {
		std::cout << std::endl << " ---- face " << z << " ----" << std::endl;
		for (size_t y = 0; y < _size[1]; y++) {
			std::cout << '\t';
			for (size_t x = 0; x < _size[0]; x++) {
				std::cout << _array[z * _size[1] * _size[0] + y * _size[0] + x] << ' ';
			}
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}
