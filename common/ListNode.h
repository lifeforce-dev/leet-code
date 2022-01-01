//---------------------------------------------------------------
//
// ListNode.h
//

#pragma once

struct ListNode
{
	int value = 0;
	ListNode* next;
	ListNode() : value(0), next(nullptr) {}
	ListNode(int x) : value(x), next(nullptr) {}
	ListNode(int x, ListNode* next) : value(x), next(next) {}
};
