//
// Created by ivan on 12/28/25.
//

#include <ranges>
#include <absl/container/flat_hash_map.h>
#include <fmt/format.h>
#include <map>
#include "problem_26.h"

namespace
{

struct value_fraction_t
{
	int val = 0;
	int recurrence_size = 0;
	bool operator<(const value_fraction_t& other) const
	{
		if (val == other.val)
			return recurrence_size < other.recurrence_size;
		return val < other.val;
	}
};
} // namespace

namespace euler_project
{

std::string problem_26::solve()
{
	return fmt::format("number with largest recurring cycle: {}", _find_max_recurrent(999));
}

int problem_26::_find_max_recurrent(int upper_bound)
{
	value_fraction_t max_value_with_recurrence{ 0, 0 };
	for (auto val : std::views::iota(2, upper_bound))
	{
		auto recurrence_size = _recurrence_size(val);
		if (recurrence_size > max_value_with_recurrence.recurrence_size)
			max_value_with_recurrence = { val, recurrence_size };
	}
	return max_value_with_recurrence.val;
}
int problem_26::_recurrence_size(const int val)
{
	int current_rest = 10;
	std::map<value_fraction_t /*fraction and generated rest*/, int /*position*/> fraction_numbers;
	int fraction_number_pos = 0;

	while (current_rest || fraction_number_pos > 1000)
	{
		if (current_rest < val)
		{
			current_rest *= 10;
			// increment, since we insert 0 here
			// while calculating the number
			fraction_number_pos++;
			continue;
		}

		value_fraction_t fraction_and_rest{ current_rest / val, current_rest % val * 10 };

		if (fraction_numbers.contains(fraction_and_rest))
			return fraction_number_pos - fraction_numbers[fraction_and_rest];

		fraction_numbers[fraction_and_rest] = fraction_number_pos;
		current_rest = current_rest % val * 10;
		fraction_number_pos++;
	}
	return 0;
}

} // namespace euler_project
