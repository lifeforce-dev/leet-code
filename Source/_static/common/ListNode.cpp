#include "ListNode.h"
#include <assert.h>

namespace Common {

//==============================================================================

ListNode::ListNode() : value(0), next(nullptr)
{
}

ListNode::ListNode(int32_t x) : value(x), next(nullptr)
{
}

ListNode::ListNode(int32_t x, ListNode* next) : value(x), next(next)
{
}

bool ListNode::operator==(const ListNode& rhs)
{
	// Base case: If either of the nodes is null, they should both be null for equality.
	if (!next && !rhs.next) {
		return value == rhs.value;
	}

	// If one of the nodes is null and the other is not, they are not equal.
	if (!next || !rhs.next) {
		return false;
	}

	// Compare the values of the current nodes and recursively compare the next nodes.
	return (value == rhs.value) && (*next == *rhs.next);
}

namespace ListUtil {

//==============================================================================

Common::ListNode* Common::ListUtil::toList(const std::vector<int32_t>& values)
{
	if (values.empty())
	{
		return nullptr;
	}

	ListNode* listHead = new ListNode(values[0]);
	Common::ListNode* current = listHead;
	for (int32_t i = 1; i < values.size(); ++i)
	{
		current->next = new ListNode(values[i]);
		current = current->next;
	}

	return listHead;
}

Common::ListNode* Common::ListUtil::toListWithCycle(const std::vector<int32_t>& values,
	int32_t cycleIndex)
{
	assert(cycleIndex < values.size());

	if (values.empty())
	{
		return nullptr;
	}

	ListNode* cycleIndexNode = nullptr;
	ListNode* listHead = nullptr;
	Common::ListNode* current = nullptr;

	for (int32_t i = 0; i < values.size(); ++i)
	{
		if (!listHead)
		{
			listHead = new ListNode(values[i]);
			current = listHead;
		}
		else
		{
			current->next = new ListNode(values[i]);
			current = current->next;
		}

		if (i == cycleIndex)
		{
			cycleIndexNode = current;
		}
	}

	current->next = cycleIndexNode;

	return listHead;

}

void deleteLinkedList(ListNode* head)
{
	while (head) {
		ListNode* temp = head;
		head = head->next;
		delete temp;
	}
}

void deleteLinkedListWithCycle(ListNode* head, int32_t size)
{
	for (int i = 0; i < size; ++i)
	{
		ListNode* temp = head;
		head = head->next;
		delete temp;
	}
}

//==============================================================================

} // namespace ListUtil

//==============================================================================

} // namespace Common
