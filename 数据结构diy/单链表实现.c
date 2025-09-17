// 单链表节点定义
typedef struct SinglyNode {
    int val;
    struct SinglyNode* next;  
} SinglyNode;

// 创建新节点
SinglyNode* createSinglyNode(int val) {
    SinglyNode* node = (SinglyNode*)malloc(sizeof(SinglyNode));
    node->val = val;
    node->next = NULL;
    return node;
}

// 在链表末尾添加节点
void appendSingly(SinglyNode** head, int val) {
    SinglyNode* newNode = createSinglyNode(val);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    // 遍历到最后一个节点（单链表必须从头遍历）
    SinglyNode* cur = *head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = newNode;
}

// 打印单链表
void printSinglyList(SinglyNode* head) {
    SinglyNode* cur = head;
    printf("单链表: ");
    while (cur != NULL) {
        printf("%d ", cur->val);
        if (cur->next != NULL) printf("-> ");
        cur = cur->next;  // 只能向后遍历
    }
    printf("\n");
}

// 测试单链表
int main() {
    SinglyNode* head = NULL;
    appendSingly(&head, 10);
    appendSingly(&head, 20);
    appendSingly(&head, 30);
    printSinglyList(head);  // 输出: 单链表: 10 -> 20 -> 30
    
    // 释放内存（省略实现）
    return 0;
}
