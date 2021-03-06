#ifndef LINKED_LIST_HEADER
#define LINKED_LIST_HEADER

typedef struct ListNode {
	void* value;
	struct ListNode* next;
}ListNode;

typedef ListNode LinkedList;

LinkedList* appendToList(LinkedList* root, void* data);
void mapList(LinkedList* root, void func());
void* reduceList(LinkedList* root, void* compare());
int lengthOfList(LinkedList* root);
void* findItemInList(ListNode*, const void*, int (*)());
LinkedList* deleteItemFromList(ListNode* node, const void* item, int compare(), void freeValue());
LinkedList* deleteNodeFromList(ListNode* node, void** returnItem, int index);
ListNode* findNodeInList(ListNode*, const void*, int (*)());
void freeList(LinkedList* node, void freeData());
void printList(LinkedList* root, void printData());

#endif
