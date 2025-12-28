//
// Created by ivan on 12/28/25.
//

#ifndef PROBLEM_26_H
#define PROBLEM_26_H
#include "solution_base.h"
namespace euler_project
{

class problem_26 : public solution_base
{

public:
	friend class test_problem_26;
	std::string solve() override;

private:
	static int _find_max_recurrent(int upper_bound);
	// find size of recurrence for 1 / val
	static int _recurrence_size(int val);
};

} // namespace euler_project

#endif // PROBLEM_26_H
