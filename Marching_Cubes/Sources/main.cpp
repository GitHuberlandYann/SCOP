# include "marching.h"

int main( int ac, char **av )
{
	(void)av;
	if (ac != 1) {
		std::cerr << "Usage: ./marching";
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
	mc.set_size(20, 20, 20);
	// mc.gen_random();
	// mc.gen_small_sphere();
	mc.gen_Perlin();
	mc.display();

	//second, let cubes march over dataset
	mc.vMarchingCubes(outdata);

	outdata.close();

}
