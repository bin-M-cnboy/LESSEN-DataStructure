#include <stdio.h>
#include <stdlib.h>

typedef struct NODE{
    int val;
    struct NODE* next;
} node;

typedef struct {
    node* head;
    int length;  // 修正拼写：lenth -> length
} MyLinkedList;


MyLinkedList* myLinkedListCreate() {
    MyLinkedList* obj = (MyLinkedList*)malloc(sizeof(MyLinkedList));
    obj->head = NULL;
    obj->length = 0;
    return obj;
}

int myLinkedListGet(MyLinkedList* obj, int index) {
    if (obj == NULL || index < 0 || index >= obj->length) {
        return -1;
    }
    node* cur = obj->head;
    for (int i = 0; i < index; i++) {
        cur = cur->next;
    }
    return cur->val;
}

void myLinkedListAddAtHead(MyLinkedList* obj, int val) {
    node* newNode = (node*)malloc(sizeof(node));
    newNode->val = val;
    newNode->next = obj->head;
    obj->head = newNode;
    obj->length++;
}

void myLinkedListAddAtTail(MyLinkedList* obj, int val) {
    node* newNode = (node*)malloc(sizeof(node));
    newNode->val = val;
    newNode->next = NULL;

    if (obj->head == NULL) {  
        obj->head = newNode;
    } else {
        node* cur = obj->head;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = newNode;
    }
    obj->length++;  
}

void myLinkedListAddAtIndex(MyLinkedList* obj, int index, int val) {
    if (obj == NULL || index < 0 || index > obj->length) {
        return;
    }

    if (index == 0) {
        myLinkedListAddAtHead(obj, val);
        return;  
    }
    if (index == obj->length) {
        myLinkedListAddAtTail(obj, val);
        return;  
    }

    node* newNode = (node*)malloc(sizeof(node));
    newNode->val = val;

    node* cur = obj->head;
    for (int i = 0; i < index - 1; i++) {
        cur = cur->next;
    }
    newNode->next = cur->next;
    cur->next = newNode;
    obj->length++; 
}

void myLinkedListDeleteAtIndex(MyLinkedList* obj, int index) {
    // 检查obj有效性及索引合法性【注意！！！】
    if (obj == NULL || index < 0 || index >= obj->length) {
        return;
    }
    node* todel;

    if (index == 0) {  // 删除头节点
        todel = obj->head;
        obj->head = obj->head->next;
    } 
    else {
        node* cur = obj->head;
        for (int i = 0; i < index - 1; i++) {
            cur = cur->next;
        }
        todel = cur->next;
        cur->next = todel->next;
    }

    free(todel);  
    obj->length--;
}

void myLinkedListFree(MyLinkedList* obj) {
    node* cur = obj->head;
    // 修复：循环条件改为cur != NULL，确保所有节点被释放
    while (cur != NULL) {
        node* tmp = cur;
        cur = cur->next;
        free(tmp);  
    }
    obj->head = NULL;
    obj->length = 0;
    free(obj);  // 释放链表对象本身【注意！！！】
}
