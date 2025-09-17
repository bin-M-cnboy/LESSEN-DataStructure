#include <stdlib.h>

typedef struct NODE {
    int val;
    struct NODE* prev;
    struct NODE* next;
} node;

typedef struct {
    node* head;
    node* tail;
    int len;
} MyLinkedList;


MyLinkedList* myLinkedListCreate() {
    MyLinkedList* new = (MyLinkedList*)malloc(sizeof(MyLinkedList));
    new->head = NULL;
    new->tail = NULL;
    new->len = 0;
    return new;
}

int myLinkedListGet(MyLinkedList* obj, int index) {
    if (obj == NULL || index < 0 || index >= obj->len) {
        return -1;
    }
    node* cur = obj->head;
    for (int i = 0; i < index; i++) {
        cur = cur->next;
    }
    return cur->val;
}

void myLinkedListAddAtHead(MyLinkedList* obj, int val) {
    node* new = (node*)malloc(sizeof(node));
    new->val = val;
    new->prev = NULL;
    
    // 检查原链表是否非空
    if (obj->head != NULL) {
        obj->head->prev = new;
        new->next = obj->head;
    } else {
        obj->tail = new;
        new->next = NULL;
    }

    obj->head = new;
    obj->len++;
}

void myLinkedListAddAtTail(MyLinkedList* obj, int val) {
    node* new = (node*)malloc(sizeof(node));
    new->val = val;
    new->next = NULL;

    if (obj->head != NULL) {
        obj->tail->next = new;
        new->prev = obj->tail;
    } else {
        obj->head = new;
        new->prev = NULL;
    }

    obj->tail = new;
    obj->len++;
}

void myLinkedListAddAtIndex(MyLinkedList* obj, int index, int val) {
    if (obj == NULL || index < 0 || index > obj->len) {
        return;
    }
    if (index == 0) {
        myLinkedListAddAtHead(obj, val);
        return;
    }
    if (index == obj->len) {
        myLinkedListAddAtTail(obj, val);
        return;
    }

    node* new = (node*)malloc(sizeof(node));
    new->val = val;

    node* cur = obj->head;
    for (int i = 0; i < index - 1; i++) {
        cur = cur->next;
    }
    new->prev = cur;
    new->next = cur->next;
    cur->next->prev = new;
    cur->next = new;
    obj->len++;
}

void myLinkedListDeleteAtIndex(MyLinkedList* obj, int index) {
    if (obj == NULL || index < 0 || index >= obj->len) {
        return;
    }
    node* todel;

    // 分类讨论：删除头节点、尾节点、中间节点【注意】
    if (index == 0) {
        todel = obj->head;
        if (todel->next != NULL) {// 是否为空链表
            todel->next->prev = NULL;
        } else {
            obj->tail = NULL;
        }
        obj->head = todel->next;
    } 
    else if (index == obj->len - 1) {
        todel = obj->tail;
        todel->prev->next = NULL;
        obj->tail = todel->prev;
    } 
    else {
        node* cur = obj->head;
        for (int i = 0; i < index - 1; i++) {
            cur = cur->next;
        }
        todel = cur->next;
        cur->next = todel->next;
        todel->next->prev = cur;
    }

    free(todel);  
    obj->len--;
}

void myLinkedListFree(MyLinkedList* obj) {
    node* cur = obj->head;
    while (cur != NULL) {
        node* tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    obj->head = NULL;
    obj->tail = NULL;
    obj->len = 0;
    free(obj);
}

/**
 * Your MyLinkedList struct will be instantiated and called as such:
 * MyLinkedList* obj = myLinkedListCreate();
 * int param_1 = myLinkedListGet(obj, index);
 
 * myLinkedListAddAtHead(obj, val);
 
 * myLinkedListAddAtTail(obj, val);
 
 * myLinkedListAddAtIndex(obj, index, val);
 
 * myLinkedListDeleteAtIndex(obj, index);
 
 * myLinkedListFree(obj);
*/
