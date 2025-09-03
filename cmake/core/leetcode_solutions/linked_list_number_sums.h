//
// Created by ivan on 8/31/25.
//

#ifndef LINKED_LIST_NUMBER_SUMS_H
#define LINKED_LIST_NUMBER_SUMS_H
#include <memory>

namespace libcore
{
namespace solutions
{

struct ListNode
{
	int val{ 0 };
	ListNode* next{ nullptr };

	ListNode(int v, ListNode* n = nullptr)
		: val(v)
		, next(n){};
	static ListNode* fromInt(int value);
	static int toInt(ListNode* node);
	int toInt() { return toInt(this); }
};
class linked_list_number_sums
{
public:
	static ListNode* sum(ListNode* l, ListNode* r);
};

} // namespace solutions
} // namespace libcore

#endif // LINKED_LIST_NUMBER_SUMS_H
