#include "../include/HuffmanTree.h"

// 构造函数
HuffmanTreeNode::HuffmanTreeNode(char ch, int freq) : character(ch), frequency(freq) {
    left = right = nullptr;
}

// 重写 getValue 方法，返回字符
string HuffmanTreeNode::getValue() {
    return string(1, character);  // 返回字符
}

// 获取节点频率
int HuffmanTreeNode::getFrequency() const {
    return frequency;
}

// 构建哈夫曼树
HuffmanTreeNode* HuffmanTreeNode::buildHuffmanTree(const unordered_map<char, int>& freqMap) {
    // 创建最小堆
    priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, freq_cmp> minHeap;

    // 将所有字符和频率加入堆中
    for (auto& pair : freqMap) {
        HuffmanTreeNode* node = new HuffmanTreeNode(pair.first, pair.second);
        minHeap.push(node);
    }
    
    // 合并最小的两个节点，直到堆中只剩下一个节点
    while (minHeap.size() > 1) {
        // 取出频率最小的两个节点
        HuffmanTreeNode* left = minHeap.top();
        minHeap.pop();
        HuffmanTreeNode* right = minHeap.top();
        minHeap.pop();
        
        // 创建新的内部节点，频率为两个子节点频率之和
        // 内部节点的字符设为 '\0'
        HuffmanTreeNode* parent = new HuffmanTreeNode('\0', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        
        // 将新节点加入堆中
        minHeap.push(parent);
    }

    // 返回哈夫曼树的根节点
    return minHeap.top();
}

// 打印哈夫曼树的编码（先序遍历）
void HuffmanTreeNode::printHuffmanCodes(HuffmanTreeNode* root, string code) {
    if (root == nullptr) {
        return;
    }
    
    // 如果是叶节点（存储字符的节点），打印字符及其编码
    if (root->left == nullptr && root->right == nullptr) {
        cout << root->character << ": " << code << endl;
        return;
    }
    
    // 递归遍历左子树，编码添加 '0'
    if (root->left != nullptr) {
        printHuffmanCodes(dynamic_cast<HuffmanTreeNode*>(root->left), code + "0");
    }
    
    // 递归遍历右子树，编码添加 '1'
    if (root->right != nullptr) {
        printHuffmanCodes(dynamic_cast<HuffmanTreeNode*>(root->right), code + "1");
    }
}
