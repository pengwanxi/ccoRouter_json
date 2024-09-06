#include "Dlist.h"
#include <stdio.h>
#include <stdlib.h>

// 创建双向链表首节点
ListHead_t *CreateList(void)
{
    ListHead_t *listHead = (ListHead_t *)malloc(sizeof(ListHead_t));
    if (listHead == NULL)
    {
        return NULL;
    }
    listHead->head = NULL;
    listHead->tail = NULL;
    return listHead;
}

// 清除整个链表，不清除头结点
void ClearList(ListHead_t *listHead, void (*NodeProcessFunc)(void *))
{

    if (listHead == NULL)
    {
        return;
    }
    ListNode_t *listNode = listHead->head;

    while (listNode != NULL)
    {
        ListNode_t *oldListNode = listNode;
        listNode = listNode->next;
        if (NodeProcessFunc != NULL)
        {
            if (oldListNode->data != NULL)
            {
                NodeProcessFunc(oldListNode->data);
            }
            oldListNode->data = NULL;
        }
        else if (oldListNode->data != NULL)
        {
            free(oldListNode->data);
            oldListNode->data = NULL;
        }
        if (oldListNode != NULL)
        {
            free(oldListNode);
        }
        oldListNode = NULL;
    }
    listHead->head = NULL;
    listHead->tail = NULL;
}

// 删除整个链表
void DeleteList(ListHead_t *listHead, void (*NodeProcessFunc)(void *))
{
    if (listHead == NULL)
    {
        return;
    }
    ClearList(listHead,NodeProcessFunc);
    if (listHead != NULL)
    {
        free(listHead);
    }
    listHead = NULL;
}

void ListAddNode(ListHead_t *listHead, void *data, const int dataSize)
{
    if (listHead == NULL || data == NULL || dataSize <= 0)
    {
        return;
    }
    ListNode_t *listNode = (ListNode_t *)malloc(sizeof(ListNode_t));
    if (listNode == NULL)
    {
        return;
    }

    listNode->prev = NULL;
    listNode->next = NULL;
    listNode->data = data;
    listNode->dataSize = dataSize;

    if (listHead->head == NULL)
    {
        listHead->head = listNode;
    }
    else
    {
        listNode->prev = listHead->tail;
        listHead->tail->next = listNode;
    }
    listHead->tail = listNode;
}

// 从双向链表获取头数据结点，注意: 数据结点在使用完后记得free
struct ListNode *ListGetNode(ListHead_t *listHead)
{
    if (listHead == NULL)
    {
        return NULL;
    }
    if (listHead->head == NULL)
    {
        return NULL;
    }

    ListNode_t *listNode = listHead->head;
    if (listHead->head == listHead->tail)
    {
        listHead->head = NULL;
        listHead->tail = NULL;
    }
    else
    {
        listHead->head = listHead->head->next;
        if (listHead->head != NULL)
        {
            listHead->head->prev = NULL;
        }
    }
    listNode->prev = NULL;
    listNode->next = NULL;
    return listNode;
}

// 销毁一个节点
void DestroyListNodeData(ListNode_t *listNode, void (*FreeFunc)(void *))
{
    if (listNode == NULL || FreeFunc == NULL)
    {
        return;
    }
    FreeFunc(listNode->data);
    listNode->data = NULL;
    free(listNode);
    listNode = NULL;
}

struct ListNode *ListGetTailNode(ListHead_t *listHead)
{
    if (listHead == NULL)
    {
        return NULL;
    }

    ListNode_t *listNode = listHead->tail;

    if (listHead->head == listHead->tail)
    {
        listHead->head = NULL;
        listHead->tail = NULL;
    }
    else
    {
        listHead->tail = listHead->tail->prev;
        listHead->tail->next = NULL;
        listNode->prev = NULL;
        listNode->next = NULL;
    }
    return listNode;
}
int GetListLength(ListHead_t *listHead)
{
    if (listHead == NULL)
    {
        return 0;
    }

    ListNode_t *listNode = listHead->head;
    int nLen = 0;

    while (listNode != NULL)
    {
        listNode = listNode->next;
        nLen++;
    }
    return nLen;
}
struct ListNode *ListGetNodeNum(ListHead_t *listHead, int Num)
{
    if (listHead == NULL)
    {
        return NULL;
    }

    int index = 0;
    ListNode_t *listNode = listHead->head;
    while (listNode != NULL)
    {
        index++;
        if (index >= Num)
        {
            return listNode;
        }
        listNode = listNode->next;
    }
    return NULL;
}
