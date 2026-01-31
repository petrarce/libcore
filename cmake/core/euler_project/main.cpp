#include "problem_26.h"
#include "problem_27.h"
#include "problem_28.h"
#include "problem_29.h"

#include <iostream>
#include <concepts>
#include <boost/program_options.hpp>
#include <fmt/format.h>
#include "solution_base.h"

namespace po = boost::program_options;
using namespace euler_project;

class dummy_solution : public solution_base
{
public:
	std::string solve() override { return "my dummy solution"; }
};

template<std::derived_from<solution_base> T>
T& gen_solution_class()
{
	static T sol;
	return sol;
}
solution_base& solutions_factory(int solution_number)
{
	if (solution_number == 0)
		return gen_solution_class<dummy_solution>();
	if (solution_number == 26)
		return gen_solution_class<problem_26>();
	if (solution_number == 27)
		return gen_solution_class<problem_27>();
	if (solution_number == 28)
		return gen_solution_class<problem_28>();
	if (solution_number == 29)
		return gen_solution_class<problem_29>();
	throw std::invalid_argument(
		fmt::format("solution number {} doesn't exist yet", solution_number));
}

int main(int ac, char* av[])
{
	po::options_description desc("Generic options");
	desc.add_options()("help,h", " <problem number>")("problem", po::value<int>(),
													  "problem number to be executed");

	po::positional_options_description positional;
	positional.add("problem", -1);

	po::variables_map vm;
	po::store(po::command_line_parser(ac, av).options(desc).positional(positional).run(), vm);

	if (vm.contains("help") || !vm.contains("problem"))
	{
		std::cout << desc << std::endl;
		return 1;
	}

	int problem = vm["problem"].as<int>();
	std::cout << fmt::format("{}\n", solutions_factory(problem).solve());
	return 0;
}
