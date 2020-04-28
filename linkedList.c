#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "linkedList.h"

void* reduceHelper(ListNode* node, void* leader, void* compare());
ListNode* createNode(void*);
ListNode* createBlankNode();

LinkedList* appendToList(ListNode* root, void* data) {
	LinkedList* nextNode;

	if (root == NULL) {
		return createNode(data);
	} else if (root->next == NULL) {
		nextNode = createNode(data);
		root->next = nextNode;
	} else {
		appendToList(root->next, data);
	}
	return root;
}

void mapList(LinkedList* root, void func()) {
	if (root == NULL) {
		return;
	}

	func(root->value);
	mapList(root->next, func);
}

void* reduceList(LinkedList* root, void* func()) {
	if (root == NULL) {
		return NULL;
	}

	return reduceHelper(root->next, root->value, func);
}

void* reduceHelper(ListNode* node, void* leader, void* func()) {
	if (node == NULL) {
		return leader;
	}

	return reduceHelper(node->next, func(leader, node->value), func);
}

int lengthOfList(LinkedList* root) {
	if (root == NULL) {
		return 0;
	} else {
		return lengthOfList(root->next) + 1;
	}
}

void* findItemInList(LinkedList* root, const void* item, int compare()) {

	ListNode* node = findNodeInList(root, item, compare);
	if (node) {
		return node->value;
	}
	else {
		return NULL;
	}
}

LinkedList* deleteItemFromList(ListNode* node, const void* item, int compare(), void freeValue()) {
	ListNode* next = node->next;
	if (node == NULL) {
		return NULL;
	} else if (compare(item, node->value)) {
		freeValue(node->value);
		free(node);
		return next;
	} else if (next == NULL) {
		return node;
	} else if (compare(item, next->value)) {
		ListNode* nextNext = next->next;
		freeValue(next->value);
		free(next);
		node->next = nextNext;
	} else {
		deleteItemFromList(next, item, compare, freeValue);
	}
	return node;
}

ListNode* findNodeInList(ListNode* node, const void* item, int compare()) {
	if (node == NULL) {
		return NULL;
	} else if (compare(item, node->value)) {
		return node;
	} else {
		return findNodeInList(node->next, item, compare);
	}
}

ListNode* createNode(void* data) {
	LinkedList* node = createBlankNode();
	node->value = data;
	return node;
}

ListNode* createBlankNode() {
	LinkedList* node = malloc(sizeof(*node));

	node->value = NULL;
	node->next = NULL;

	return node;
}

void freeList(ListNode* node, void freeData()) {

	LinkedList* next;

	if (node == NULL) {
		return;
	}

	next = node->next;

	if (node->value && freeData != NULL) {
		freeData(node->value);
	}

	free(node);
	freeList(next, freeData);
}

void printList(ListNode* root, void printData()) {
	if (root == NULL) {
		return;
	}

	printData(root->value);
	printList(root->next, printData);
}

