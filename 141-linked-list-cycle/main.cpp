#define CATCH_CONFIG_MAIN

#include "Catch2/catch.hpp"
#include "common/Log.h"
#include "common/Timer.h"
#include "common/ListNode.h"

#include <algorithm>
#include <assert.h>
#include <vector>

/*
* Given head, the head of a linked list, determine if the linked list has a cycle in it.

There is a cycle in a linked list if there is some node in the list that can be reached again
by continuously following the next pointer. Internally, pos is used to denote the index of the
node that tail's next pointer is connected to. Note that pos is not passed as a parameter.

Return true if there is a cycle in the linked list. Otherwise, return false.
*/

bool hasCycle(Common::ListNode* head) {

	static const int32_t s_fastStep = 2;
	static const int32_t s_slowStep = 1;

	Common::ListNode* fastHead = head;
	Common::ListNode* slowHead = head;

	bool doesHaveCycle = false;

	while (true)
	{
		for (int i = 0; i < s_fastStep; ++i)
		{
			fastHead = fastHead ? fastHead->next : nullptr;
		}

		for (int i = 0; i < s_slowStep; ++i)
		{
			slowHead = slowHead ? slowHead->next : nullptr;
		}

		if (fastHead == nullptr || slowHead == nullptr)
		{
			return false;

		}

		if (fastHead == slowHead)
		{
			return true;
		}
	}

	return false;
}

SCENARIO("Detecting a cycle given a linked list")
{
	GIVEN("A small list with a cycle at index 1")
	{
		std::vector<int32_t> values{ 3, 2, 0, 4 };
		Common::ListNode* list = Common::ListUtil::toListWithCycle(values, 1);

		REQUIRE(hasCycle(list));
		Common::ListUtil::deleteLinkedListWithCycle(list, values.size());
	}

	GIVEN("A small list with no cycle")
	{
		std::vector<int32_t> values{ 3, 2, 0, 4 };
		Common::ListNode* list = Common::ListUtil::toList(values);

		REQUIRE(!hasCycle(list));
		Common::ListUtil::deleteLinkedList(list);
	}

	GIVEN("A single value list with no cycle")
	{
		std::vector<int32_t> values{ 1 };
		Common::ListNode* list = Common::ListUtil::toList(values);

		REQUIRE(!hasCycle(list));
		Common::ListUtil::deleteLinkedList(list);
	}

	GIVEN("A 2 value list with a cycle")
	{
		std::vector<int32_t> values{ 1, 2 };
		Common::ListNode* list = Common::ListUtil::toListWithCycle(values, 1);

		REQUIRE(hasCycle(list));
		Common::ListUtil::deleteLinkedListWithCycle(list, values.size());
	}
}
