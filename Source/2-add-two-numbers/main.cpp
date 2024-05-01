#define CATCH_CONFIG_MAIN

#include "Catch2/catch.hpp"
#include "common/ListNode.h"
#include "common/Log.h"
#include "common/Timer.h"

#include <algorithm>
#include <assert.h>
#include <deque>
#include <memory>
#include <vector>

/*
*
* 
You are given two non-empty linked lists representing two non-negative integers.
The digits are stored in reverse order, and each of their nodes contains a single digit.
Add the two numbers and return the sum as a linked list.

You may assume the two numbers do not contain any leading zero, except the number 0 itself.

Assume the caller takes ownership of the list memory.

*/

Common::ListNode* addTwoNumbers(Common::ListNode* l1, Common::ListNode* l2)
{
	// The head of the linked list we'll return.
	Common::ListNode* resultListHead = nullptr;

	// We need to keep track of the current result node to build the list out.
	Common::ListNode* currentResultNode = nullptr;

	// We need to keep track of the current l1 and l2 nodes as we move through and add.
	Common::ListNode* currentL1Node = l1;
	Common::ListNode* currentL2Node = l2;

	// If we have a carry value as a result of an add, we'll bring that over to the next add.
	int32_t carry = 0;
	while (true)
	{
		if (!currentL1Node && !currentL2Node)
		{
			// If we're finished but we still have a carry we need to add it to the end.
			if (carry == 1)
			{
				currentResultNode->next = new Common::ListNode(carry);
			}
			return resultListHead;
		}

		// If l1 and l2 are different lengths then the current node could be null on one and not the other.
		int32_t a = currentL1Node ? currentL1Node->value : 0;
		int32_t b = currentL2Node ? currentL2Node->value : 0;

		// Add and setup the carry if necessary.
		int32_t result = a + b + carry;
		carry = 0;
		if (result > 9)
		{
			result = result % 10;
			carry = 1;
		}

		// Lazy init the result list.
		if (!resultListHead)
		{
			resultListHead = new Common::ListNode(result);
			currentResultNode = resultListHead;
		}
		else
		{
			currentResultNode->next = new Common::ListNode(result);
			currentResultNode = currentResultNode->next;
		}

		// nullptr check just handles l1/l2 of diff lengths again, then we advance the nodes.
		currentL1Node = currentL1Node ? currentL1Node->next : nullptr;
		currentL2Node = currentL2Node ? currentL2Node->next : nullptr;
	}

	// If we get here, compiler has exploded.
	return nullptr;
}

SCENARIO("Calculate the sum of two linked lists")
{
	GIVEN("Two lists of 3 nodes")
	{
		std::vector<int32_t> numsA{ 2, 4, 3 };
		Common::ListNode* listA = Common::ListUtil::toList(numsA);

		std::vector<int32_t> numsB{ 5, 6, 4 };
		Common::ListNode* listB = Common::ListUtil::toList(numsB);

		std::vector<int32_t> resultKey{ 7, 0, 8 };
		Common::ListNode* resultKeyList = Common::ListUtil::toList(resultKey);
		Common::ListNode* resultList = addTwoNumbers(listA, listB);


		bool isValid = *resultKeyList == *resultList;
		REQUIRE(isValid);

		Common::ListUtil::deleteLinkedList(resultKeyList);
		Common::ListUtil::deleteLinkedList(resultList);
	}


	GIVEN("Two lists of 3 nodes")
	{
		std::vector<int32_t> numsA{ 2, 4, 3 };
		Common::ListNode* listA = Common::ListUtil::toList(numsA);

		std::vector<int32_t> numsB{ 5, 6, 4 };
		Common::ListNode* listB = Common::ListUtil::toList(numsB);

		std::vector<int32_t> resultKey{ 7, 0, 8 };
		Common::ListNode* resultKeyList = Common::ListUtil::toList(resultKey);
		Common::ListNode* resultList = addTwoNumbers(listA, listB);


		bool isValid = *resultKeyList == *resultList;
		REQUIRE(isValid);

		Common::ListUtil::deleteLinkedList(resultKeyList);
		Common::ListUtil::deleteLinkedList(resultList);
	}

	GIVEN("Two lists of 1 node")
	{
		std::vector<int32_t> numsA{ 0 };
		Common::ListNode* listA = Common::ListUtil::toList(numsA);

		std::vector<int32_t> numsB{ 0 };
		Common::ListNode* listB = Common::ListUtil::toList(numsB);

		std::vector<int32_t> resultKey{ 0 };
		Common::ListNode* resultKeyList = Common::ListUtil::toList(resultKey);
		Common::ListNode* resultList = addTwoNumbers(listA, listB);


		bool isValid = *resultKeyList == *resultList;
		REQUIRE(isValid);

		Common::ListUtil::deleteLinkedList(listA);
		Common::ListUtil::deleteLinkedList(resultKeyList);
		Common::ListUtil::deleteLinkedList(resultList);
	}

	GIVEN("Two lists of uneven nodes")
	{
		std::vector<int32_t> numsA{ 9, 9 };
		Common::ListNode* listA = Common::ListUtil::toList(numsA);

		std::vector<int32_t> numsB{ 9};
		Common::ListNode* listB = Common::ListUtil::toList(numsB);

		std::vector<int32_t> resultKey{ 8, 0, 1};
		Common::ListNode* resultKeyList = Common::ListUtil::toList(resultKey);
		Common::ListNode* resultList = addTwoNumbers(listA, listB);


		bool isValid = *resultKeyList == *resultList;
		REQUIRE(isValid);

		Common::ListUtil::deleteLinkedList(resultKeyList);
		Common::ListUtil::deleteLinkedList(resultList);
	}

	GIVEN("Two lists of uneven nodes")
	{
		std::vector<int32_t> numsA{ 9, 9, 9, 9, 9, 9, 9 };
		Common::ListNode* listA = Common::ListUtil::toList(numsA);

		std::vector<int32_t> numsB{ 9, 9, 9, 9 };
		Common::ListNode* listB = Common::ListUtil::toList(numsB);

		std::vector<int32_t> resultKey{ 8, 9, 9, 9, 0, 0, 0, 1 };
		Common::ListNode* resultKeyList = Common::ListUtil::toList(resultKey);
		Common::ListNode* resultList = addTwoNumbers(listA, listB);


		bool isValid = *resultKeyList == *resultList;
		REQUIRE(isValid);

		Common::ListUtil::deleteLinkedList(resultKeyList);
		Common::ListUtil::deleteLinkedList(resultList);
	}
}
