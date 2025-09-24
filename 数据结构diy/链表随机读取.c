/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */



typedef struct {
    int * arr;
    int length;
} Solution;


Solution* solutionCreate(struct ListNode* head) {
    Solution * obj = (Solution *)malloc(sizeof(Solution));
    assert(obj != NULL);    // 检查内存分配是否成功

    obj->length = 0;
    struct ListNode * node = head;
    while (node) {
        node = node->next;
        obj->length++;
    }
    obj->arr = (int *)malloc(sizeof(int) * obj->length);
    assert(obj->arr != NULL);
    
    node = head;
    for (int i = 0; i < obj->length; i++) {
        obj->arr[i] = node->val;
        node = node->next;
    }
    return obj;
}

int solutionGetRandom(Solution* obj) {
    return obj->arr[rand() % obj->length];  // rand() % obj->length // 0,length-1之间的随机索引
}

void solutionFree(Solution* obj) {
    free(obj->arr);
    free(obj);
}

/**
 * Your Solution struct will be instantiated and called as such:
 * Solution* obj = solutionCreate(head);
 * int param_1 = solutionGetRandom(obj);
 
 * solutionFree(obj);
*/