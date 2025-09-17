// 双链表节点定义
typedef struct DoublyNode {
    int val;
    struct DoublyNode* prev;  // 指向前一个节点
    struct DoublyNode* next;  // 指向后一个节点
} DoublyNode;

// 创建新节点
DoublyNode* createDoublyNode(int val) {
    DoublyNode* node = (DoublyNode*)malloc(sizeof(DoublyNode));
    node->val = val;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

// 在链表末尾添加节点
void appendDoubly(DoublyNode** head, int val) {
    DoublyNode* newNode = createDoublyNode(val);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    // 遍历到最后一个节点
    DoublyNode* cur = *head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    // 双向关联
    cur->next = newNode;
    newNode->prev = cur;
}

// 正向打印双链表（从头到尾）
void printDoublyForward(DoublyNode* head) {
    DoublyNode* cur = head;
    printf("双链表(正向): ");
    while (cur != NULL) {
        printf("%d ", cur->val);
        if (cur->next != NULL) printf("-> ");
        cur = cur->next;
    }
    printf("\n");
}

// 反向打印双链表（从尾到头）
void printDoublyBackward(DoublyNode* head) {
    if (head == NULL) return;
    // 先定位到尾节点
    DoublyNode* cur = head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    // 从尾节点向前遍历
    printf("双链表(反向): ");
    while (cur != NULL) {
        printf("%d ", cur->val);
        if (cur->prev != NULL) printf("-> ");
        cur = cur->prev;
    }
    printf("\n");
}

// 测试双链表
int main() {
    DoublyNode* head = NULL;
    appendDoubly(&head, 10);
    appendDoubly(&head, 20);
    appendDoubly(&head, 30);
    printDoublyForward(head);  // 输出: 双链表(正向): 10 -> 20 -> 30
    printDoublyBackward(head); // 输出: 双链表(反向): 30 -> 20 -> 10
    
    // 释放内存（省略实现）
    return 0;
}
