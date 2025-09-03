//
// Created by ivan on 8/30/25.
//

#ifndef TWOSUMCALCULATOR_H
#define TWOSUMCALCULATOR_H
#include "core/utils/StrongTypedef.h"

#include <algorithm>
#include <numeric>
#include <optional>
#include <vector>
#include <absl/strings/str_format.h>
#include <boost/mpl/assert.hpp>
#include <iostream>
#include <absl/container/flat_hash_map.h>
namespace libcore
{
namespace solutions
{
class two_sum_calculator
{
public:
	static std::optional<std::pair<int, int> > solve(const std::vector<int>& nums, int target);
};

} // namespace solutions
} // namespace libcore

#endif // TWOSUMCALCULATOR_H
