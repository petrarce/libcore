#ifndef SOLUTION_BASE_H
#define SOLUTION_BASE_H
#include <string>

namespace euler_project
{

class solution_base
{
public:
	virtual ~solution_base() = default;
	virtual std::string solve() = 0;
};

} // namespace euler_project

#endif // SOLUTION_BASE_H
