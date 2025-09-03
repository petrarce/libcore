//
// Created by ivan on 8/31/25.
//

#include "linked_list_number_sums.h"

#include <cstdint>
#include <span>
#include <vector>

namespace libcore
{
namespace solutions
{
namespace detail
{
class Solution
{
public:
	ListNode* addTwoNumbers(ListNode* l1, ListNode* l2)
	{
		if (l1 == nullptr && l2 == nullptr)
			return nullptr;

		auto curv1 = l1;
		auto curv2 = l2;
		ListNode* head = new ListNode(0);
		auto curHead = head;
		head->val = (curv1 ? curv1->val : 0) + (curv2 ? curv2->val : 0);
		int overflow = head->val / 10;
		head->val = head->val % 10;

		while (curv1 && curv1->next || curv2 && curv2->next)
		{
			curv1 = (curv1 && curv1->next) ? curv1->next : nullptr;
			curv2 = (curv2 && curv2->next) ? curv2->next : nullptr;
			int v1 = curv1 ? curv1->val : 0;
			int v2 = curv2 ? curv2->val : 0;
			curHead->next = new ListNode((v1 + v2 + overflow) % 10);
			curHead = curHead->next;
			overflow = (v1 + v2 + overflow) / 10;
		}

		if (overflow)
			curHead->next = new ListNode(overflow);

		return head;
	}
};
} // namespace detail

int ListNode::toInt(ListNode* node)
{
	if (node == nullptr)
		return 0;
	int v = node->val;
	int exp = 10;
	ListNode* cur = node->next;
	while (cur != nullptr)
	{
		v += exp * cur->val;
		exp *= 10;
		cur = cur->next;
	}
	return v;
}

ListNode* ListNode::fromInt(int v)
{
	if (v <= 0)
		return nullptr;

	std::vector<int8_t> nums;
	while (v != 0)
	{
		nums.push_back(v % 10);
		v = v / 10;
	}
	ListNode* head = new ListNode(nums[0]);
	auto cur = head;
	for (int i = 1; i < nums.size(); ++i)
	{
		cur->next = new ListNode(nums[i]);
		cur = cur->next;
	}
	return head;
}

ListNode* linked_list_number_sums::sum(ListNode* l, ListNode* r)
{
	return detail::Solution().addTwoNumbers(l, r);
}

} // namespace solutions
} // namespace libcore
