//
// Created by ivan on 1/17/26.
//

#ifndef PROBLEM_28_H
#define PROBLEM_28_H

#include "solution_base.h"
#include <cstdint>
namespace euler_project
{

class problem_28 : public solution_base
{
	friend class test_problem_28;

public:
	std::string solve() override;

private:
	uint64_t level_sum(uint64_t level);
	uint64_t sum_till_level(uint64_t level);
};

} // namespace euler_project

#endif // PROBLEM_28_H
