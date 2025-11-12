#include "../include/BinaryTree.h"
#include <iostream>
#include <queue>

using namespace std;

// 构造函数的实现，初始化左右孩子为 nullptr
BinaryTreeNode::BinaryTreeNode() : left(nullptr), right(nullptr) {}

// 先序遍历的实现：访问根节点，然后先序遍历左子树，再先序遍历右子树
void BinaryTreeNode::PreOrderTraverse(BinaryTreeNode* root) {
    // 使用getValue()函数获得节点的值
    if (root == nullptr) {
        return;
    }
    cout << root->getValue() << " ";  // 访问根节点
    PreOrderTraverse(root->left);     // 先序遍历左子树
    PreOrderTraverse(root->right);    // 先序遍历右子树
}

// 中序遍历的实现：先中序遍历左子树，再访问根节点，最后中序遍历右子树
void BinaryTreeNode::InOrderTraverse(BinaryTreeNode* root) {
    if (root == nullptr) {
        return;
    }
    InOrderTraverse(root->left);      // 中序遍历左子树
    cout << root->getValue() << " ";  // 访问根节点
    InOrderTraverse(root->right);     // 中序遍历右子树
}

// 后序遍历的实现：先后序遍历左子树，再后序遍历右子树，最后访问根节点
void BinaryTreeNode::PostOrderTraverse(BinaryTreeNode* root) {
    if (root == nullptr) {
        return;
    }
    PostOrderTraverse(root->left);    // 后序遍历左子树
    PostOrderTraverse(root->right);   // 后序遍历右子树
    cout << root->getValue() << " ";  // 访问根节点
}

// 层次遍历的实现：使用队列，按层级从上到下、从左到右遍历树
void BinaryTreeNode::LevelOrderTraverse(BinaryTreeNode* root) {
    if (root == nullptr) {
        return;
    }
    queue<BinaryTreeNode*> q;
    q.push(root);  // 根节点入队
    
    while (!q.empty()) {
        BinaryTreeNode* node = q.front();
        q.pop();
        cout << node->getValue() << " ";  // 访问队首节点
        
        // 如果左孩子存在，入队
        if (node->left != nullptr) {
            q.push(node->left);
        }
        // 如果右孩子存在，入队
        if (node->right != nullptr) {
            q.push(node->right);
        }
    }
}

// 辅助函数：通过中序和后序遍历构建树（递归实现）
BinaryTreeNode* BinaryTreeNode::buildTreeFromInorderPostorderHelper(const std::vector<std::string>& inorder, int inStart, int inEnd,
                                                                    const std::vector<std::string>& postorder, int postStart, int postEnd,
                                                                    std::unordered_map<std::string, int>& inorderMap) {
    // 递归终止条件
    if (inStart > inEnd || postStart > postEnd) {
        return nullptr;
    }

    // 后序遍历的最后一个元素是根节点
    BinaryTreeNode* root = new BinaryTreeNode();
    root->name = postorder[postEnd];

    // 获取根节点在中序遍历中的位置并计算左子树的大小
    int inRootIndex = inorderMap[root->name];
    int leftTreeSize = inRootIndex - inStart;

    // 递归构建左子树和右子树
    root->left = buildTreeFromInorderPostorderHelper(inorder, inStart, inRootIndex - 1,
                                                      postorder, postStart, postStart + leftTreeSize - 1,
                                                      inorderMap);
    root->right = buildTreeFromInorderPostorderHelper(inorder, inRootIndex + 1, inEnd,
                                                       postorder, postStart + leftTreeSize, postEnd - 1,
                                                       inorderMap);

    return root;
}

// 构建树的函数，调用辅助函数来构建二叉树
BinaryTreeNode* BinaryTreeNode::buildTreeFromInorderPostorder(const std::vector<std::string>& inorder, const std::vector<std::string>& postorder) {
    std::unordered_map<std::string, int> inorderMap;
    // 构建中序遍历值到索引的映射，方便快速查找
    for (int i = 0; i < inorder.size(); ++i) {
        inorderMap[inorder[i]] = i;
    }

    // 调用辅助函数开始构建树
    return buildTreeFromInorderPostorderHelper(inorder, 0, inorder.size() - 1, postorder, 0, postorder.size() - 1, inorderMap);
}

// 辅助函数：通过中序和前序遍历构建树（递归实现）
BinaryTreeNode* BinaryTreeNode::buildTreeFromInorderPreorderHelper(const vector<string>& inorder, int inStart, int inEnd, 
                                                                     const vector<string>& preorder, int preStart, int preEnd, 
                                                                     unordered_map<string, int>& inorderMap) {
    // 递归终止条件
    if (inStart > inEnd || preStart > preEnd) {
        return nullptr;
    }

    // 前序遍历的第一个元素是根节点
    BinaryTreeNode* root = new BinaryTreeNode();
    root->name = preorder[preStart];

    // 获取根节点在中序遍历中的位置并计算左子树的大小
    int inRootIndex = inorderMap[root->name];
    int leftTreeSize = inRootIndex - inStart;

    // 递归构建左子树和右子树
    root->left = buildTreeFromInorderPreorderHelper(inorder, inStart, inRootIndex - 1,
                                                     preorder, preStart + 1, preStart + leftTreeSize,
                                                     inorderMap);
    root->right = buildTreeFromInorderPreorderHelper(inorder, inRootIndex + 1, inEnd,
                                                      preorder, preStart + leftTreeSize + 1, preEnd,
                                                      inorderMap);

    return root;
}

// 构建树的函数，调用辅助函数来构建二叉树
BinaryTreeNode* BinaryTreeNode::buildTreeFromInorderPreorder(const vector<string>& inorder, const vector<string>& preorder) {
    unordered_map<string, int> inorderMap;
    // 构建中序遍历值到索引的映射
    for (int i = 0; i < inorder.size(); ++i) {
        inorderMap[inorder[i]] = i;
    }

    // 调用辅助函数开始构建树
    return buildTreeFromInorderPreorderHelper(inorder, 0, inorder.size() - 1, preorder, 0, preorder.size() - 1, inorderMap);
}

// getValue 虚函数的实现，返回当前节点的名字
string BinaryTreeNode::getValue() {
    return this->name;
}
