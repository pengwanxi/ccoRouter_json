#ifndef _DLIST_H
#define _DLIST_H

typedef struct ListNode
{
    void *data;
    int dataSize;
    struct ListNode *prev;
    struct ListNode *next;
} ListNode_t;

typedef struct ListHead
{
    ListNode_t *head;
    ListNode_t *tail;
} ListHead_t;

ListHead_t *CreateList(void);                                           // 创建双向链表首节点
void ClearList(ListHead_t *listHead, void (*NodeProcessFunc)(void *));  // 清空双向链表数据
void DeleteList(ListHead_t *listHead, void (*NodeProcessFunc)(void *)); // 删除双向链表
void ListAddNode(ListHead_t *listHead, void *data, const int dataSize);
struct ListNode *ListGetNode(ListHead_t *listHead);
void DestroyListNodeData(ListNode_t *listNode, void (*FreeFunc)(void *));
struct ListNode *ListGetTailNode(ListHead_t *ListHead);
int GetListLength(ListHead_t *ListHead); // 获取链表长度
struct ListNode *ListGetNodeNum(ListHead_t *ListHead, int Num);

#endif