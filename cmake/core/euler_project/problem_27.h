//
// Created by ivan on 12/30/25.
//

#ifndef PROBKEM_27_H
#define PROBKEM_27_H
#include "solution_base.h"
namespace euler_project
{

class problem_27 : public solution_base
{
public:
	std::string solve() override;
	static int _count(int a, int b);
};

} // namespace euler_project

#endif // PROBKEM_27_H
