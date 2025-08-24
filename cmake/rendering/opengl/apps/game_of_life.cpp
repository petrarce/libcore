#include "programs/Program.h"

#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <rendering/opengl/core/evaluators/ComputeEvaluator.h>
namespace po = boost::program_options;

struct GameConfig
{
	int width;
	int height;
	std::string config_path;
};

GameConfig parse_command_line(int argc, char* argv[])
{
	GameConfig config;
	po::options_description desc("Game of Life options");
	desc.add_options()("help,h", "Show help message")(
		"width,w", po::value<int>(&config.width)->default_value(500), "Grid width")(
		"height,t", po::value<int>(&config.height)->default_value(500),
		"Grid height")("config-path,c", po::value<std::string>(&config.config_path)->required(),
					   "Path to initial configuration file");

	po::variables_map vm;
	try
	{
		po::store(po::parse_command_line(argc, argv, desc), vm);

		if (vm.count("help"))
		{
			std::cout << desc << "\n";
			exit(0);
		}

		po::notify(vm);
	}
	catch (const po::error& e)
	{
		std::cerr << "Error: " << e.what() << "\n";
		std::cerr << desc << "\n";
		exit(1);
	}

	return config;
}

int main(int argc, char* argv[])
{
	auto config = parse_command_line(argc, argv);

	std::cout << "Starting Game of Life with:\n"
			  << "Width: " << config.width << "\n"
			  << "Height: " << config.height << "\n"
			  << "Config path: " << config.config_path << "\n";

	core_gfx::open_gl::InputBuffer current(std::vector(config.width * config.height, false));
	core_gfx::open_gl::OutputBuffer next(std::vector(config.width * config.height, false));

	core_gfx::open_gl::ComputeShader compute(
		std::filesystem::path("GameOfLifeAssets/generation.comp"));
	core_gfx::open_gl::Program prog;
	prog.Setup(compute);
	prog.Use();
	core_gfx::open_gl::ComputeEvaluator evaluator;

	for (int i = 0;; ++i)
	{
		std::cout << "Generation " << i << std::endl;
		evaluator.Evaluate({ config.width, config.height, 1 }, current, next);
		current->swap(*next);
	}
	return 0;
}
