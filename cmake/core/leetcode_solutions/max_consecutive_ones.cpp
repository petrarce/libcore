//
// Created by ifolbort on 9/4/25.
//

#include "max_consecutive_ones.h"

#include <deque>

namespace libcore
{
namespace solutions
{

namespace detail
{

template<class T>
class ciclic_buffer_deque : public std::vector<T>
{
public:
	explicit ciclic_buffer_deque(int capacity)
		: std::vector<T>(capacity)
	{
	}
	void push_back(const T& elem) { }

private:
	int start{ 0 };
	int end{ 0 };
};

class Solution
{
public:
	int longestOnes(const std::vector<int>& nums, int k)
	{
		std::deque<int> dq;
		int arrayStartIdx = 0;
		int maxOnes = 0;
		for (int i = 0; i < nums.size(); ++i)
		{
			const auto n = nums[i];
			if (!n)
			{
				dq.push_back(i);
				if (dq.size() > k)
				{
					arrayStartIdx = dq.front() + 1;
					dq.pop_front();
				}
			}
			maxOnes = std::max(maxOnes, i + 1 - arrayStartIdx);
		}
		return maxOnes;
	}
};
} // namespace detail

int max_consecutive_ones::solve(const std::vector<int>& nums, int k)
{
	return detail::Solution().longestOnes(nums, k);
}

} // namespace solutions
} // namespace libcore
