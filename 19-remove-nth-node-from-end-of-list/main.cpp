#define CATCH_CONFIG_MAIN

#include "Catch2/catch.hpp"
#include "common/ListNode.h"

#include <memory>

using ListNodePtr = std::unique_ptr<Common::ListNode>;
Common::ListNode* removeNthFromEnd(Common::ListNode* head, int n)
{
	Common::ListNode* current = head;

	// get the length of the list
	int listLength = 0;
	while (current != nullptr)
	{
		current = current->next;
		listLength++;
	}

	// special case, remove the only element.
	if (listLength == 1 && n == 1)
	{
		head = nullptr;
		return head;
	}

	// Remove from right side.
	if (n == 1)
	{
		current = head;
		int targetNodeDepth = listLength - n;
		// Travel to the node just before the one we want to delete
		for (int i = 0; i < targetNodeDepth - 1; ++i)
		{
			current = current->next;
		}
		current->next = nullptr;
		return head;
	}

	// Remove from left side
	if (n == listLength)
	{
		Common::ListNode* target = head;
		head = head->next;
		target = nullptr;
		return head;
	}

	// Remove from middle
	// We need the left neighbor of are deletion target.
	int targetNodeDepth = listLength - n;
	current = head;

	// Travel to the node just before the one we want to delete
	for (int i = 0; i < targetNodeDepth - 1; ++i)
	{
		current = current->next;
	}

	Common::ListNode* leftNeighbor = current;
	Common::ListNode* target = leftNeighbor->next;
	Common::ListNode* rightNeighbor = target->next;

	// Hook that node with the deleted node's next.
	leftNeighbor->next = rightNeighbor;

	return head;
}

SCENARIO("Remove nth node from end of linked list")
{
	GIVEN("A linked list [1,2,3,4,5], n = 2")
	{
		ListNodePtr five = std::make_unique<Common::ListNode>(5);
		ListNodePtr four = std::make_unique<Common::ListNode>(4, five.get());
		ListNodePtr three = std::make_unique<Common::ListNode>(3, four.get());
		ListNodePtr two = std::make_unique<Common::ListNode>(2, three.get());
		ListNodePtr one = std::make_unique<Common::ListNode>(1, two.get());

		Common::ListNode* current = one.get();
		REQUIRE(current->value == 1);
		current = current->next;
		REQUIRE(current->value == 2);
		current = current->next;
		REQUIRE(current->value == 3);
		current = current->next;
		REQUIRE(current->value == 4);
		current = current->next;
		REQUIRE(current->value == 5);
		current = current->next;
		REQUIRE(current == nullptr);

		WHEN("Removing 2nd node from the end")
		{
			Common::ListNode* head = removeNthFromEnd(one.get(), 2);

			current = head;
			REQUIRE(current != nullptr);
			REQUIRE(current->value == 1);
			current = current->next;
			REQUIRE(current->value == 2);
			current = current->next;
			REQUIRE(current->value == 3);
			current = current->next;
			REQUIRE(current->value == 5);
			current = current->next;
			REQUIRE(current == nullptr);
		}
	}

	GIVEN("A linked list [1], n = 1")
	{
		ListNodePtr one = std::make_unique<Common::ListNode>(1);
		Common::ListNode* current = one.get();
		REQUIRE(current->value == 1);
		REQUIRE(current->next == nullptr);

		WHEN("Removing the only node")
		{
			Common::ListNode* head = removeNthFromEnd(one.get(), 1);

			current = head;
			REQUIRE(current == nullptr);
		}
	}

	GIVEN("A linked list [1,2], n = 1")
	{
		ListNodePtr two = std::make_unique<Common::ListNode>(2);
		ListNodePtr one = std::make_unique<Common::ListNode>(1, two.get());

		Common::ListNode* current = one.get();
		REQUIRE(current->value == 1);
		current = current->next;
		REQUIRE(current->value == 2);
		REQUIRE(current->next == nullptr);

		WHEN("Removing one node")
		{
			Common::ListNode* head = removeNthFromEnd(one.get(), 1);

			current = head;
			REQUIRE(current != nullptr);
			REQUIRE(current->value == 1);
			REQUIRE(current->next == nullptr);
		}
	}

	GIVEN("A linked list [1,2,3,4,5], n = 5")
	{
		ListNodePtr five = std::make_unique<Common::ListNode>(5);
		ListNodePtr four = std::make_unique<Common::ListNode>(4, five.get());
		ListNodePtr three = std::make_unique<Common::ListNode>(3, four.get());
		ListNodePtr two = std::make_unique<Common::ListNode>(2, three.get());
		ListNodePtr one = std::make_unique<Common::ListNode>(1, two.get());

		Common::ListNode* current = one.get();
		REQUIRE(current->value == 1);
		current = current->next;
		REQUIRE(current->value == 2);
		current = current->next;
		REQUIRE(current->value == 3);
		current = current->next;
		REQUIRE(current->value == 4);
		current = current->next;
		REQUIRE(current->value == 5);
		current = current->next;
		REQUIRE(current == nullptr);

		WHEN("Removing 5th node from the end")
		{
			Common::ListNode* head = removeNthFromEnd(one.get(), 5);

			current = head;
			REQUIRE(current != nullptr);
			REQUIRE(current->value == 2);
			current = current->next;
			REQUIRE(current->value == 3);
			current = current->next;
			REQUIRE(current->value == 4);
			current = current->next;
			REQUIRE(current->value == 5);
			current = current->next;
			REQUIRE(current == nullptr);
		}
	}
}
