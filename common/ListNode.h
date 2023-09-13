//---------------------------------------------------------------
//
// ListNode.h
//

#pragma once

#include <vector>
#include <memory>

namespace Common {

//==============================================================================

struct ListNode
{
	ListNode();
	ListNode(int32_t x);
	ListNode(int32_t x, ListNode* next);

	int value = 0;

	// We can't use a unique_ptr because we can't interact with LeetCode's ListNode API that way.
	ListNode* next = nullptr;

	bool operator==(const ListNode& list1);
};

// Utility functions, usually for building test cases.
namespace ListUtil {

//==============================================================================

// The called of this assumes ownership of the returned linked list.
ListNode* toList(const std::vector<int32_t>& values);

ListNode* toListWithCycle(const std::vector<int32_t>& values, int32_t cycleIndex);

// The only reason this exists is to conform the code to what LeetCode expects.
// This must be called to avoid memory leaks.
void deleteLinkedList(ListNode* head);

// Some test cases require a list with a cycle. We know the size, so just delete up to the end.
void deleteLinkedListWithCycle(ListNode* head, int32_t size);

//==============================================================================

} // namespace ListUtil

//==============================================================================

} // namespace Common
