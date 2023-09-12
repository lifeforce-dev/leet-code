#include "ListNode.h"

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

void deleteLinkedList(ListNode* head)
{
	while (head) {
		ListNode* temp = head;
		head = head->next;
		delete temp;
	}
}

//==============================================================================

} // namespace ListUtil

//==============================================================================

} // namespace Common
