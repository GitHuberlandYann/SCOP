# include "marching.h"

int main( int ac, char **av )
{
	bool cross = false;
	std::string arg1, arg2;

	if (ac == 3) {
		arg1 = av[1];
		arg2 = av[2];
		if (arg1.compare("-c") || arg2.size() < 6 || arg2.compare(arg2.size() - 6, 6, ".cross")) {
			std::cerr << "Usage ./march -c <file_path>.cross" << std::endl;
			return (1);
		}
		cross = true;
	} else if (ac != 1) {
		std::cerr << "Usage: ./march" << std::endl;
		return (1);
	}

	std::string file_name = "march.obj";
	std::ofstream outdata(file_name.c_str(), std::ofstream::trunc);
	if (!outdata.is_open()) {
		std::cerr << "could not open file" << std::endl;
		return (1);
	}
	outdata << "# author GitHuberlandYann\n# made with ./march\n" << std::endl;

	//first, generate dataset
	MarchingCube mc = MarchingCube();
	mc.set_size(60, 20, 20);
	if (cross) {
		std::ifstream indata(arg2);
		if (!indata.is_open()) {
			std::cerr << "Error\nCould not open file " << arg2 << std::endl;
			return (1);
		}
		try {
			mc.set_cross_arrays(indata);
			mc.gen_cross_array();
		} catch (std::exception & e) {
			std::cerr << e.what() << std::endl;
			indata.close();
			return (1);
		}
		indata.close();
	} else {
		// mc.gen_random();
		// mc.gen_small_sphere();
		mc.gen_Perlin();
	}
	mc.display();

	//second, let cubes march over dataset
	mc.vMarchingCubes(outdata);

	outdata.close();

}
