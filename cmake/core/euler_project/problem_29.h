//
// Created by ivan on 1/25/26.
//

#ifndef PROBLEM_29_H
#define PROBLEM_29_H
#include "solution_base.h"
namespace euler_project
{

class problem_29 : public solution_base
{
	friend class tets_problem_29;

public:
	std::string solve() override;
	;

private:
	static unsigned int number_unique_values(unsigned int a, unsigned int b);
};

} // namespace euler_project

#endif // PROBLEM_29_H
