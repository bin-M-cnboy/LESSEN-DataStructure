#include "../include/ExpressionBinaryTree.h"
#include <stdexcept>
#include <climits>

using namespace std;

// 构造函数，初始化节点的值
ExpressionBinaryTreeNode::ExpressionBinaryTreeNode(string val) : value(val) {}

// 获取节点的值
string ExpressionBinaryTreeNode::getValue() {
    return value;
}

// 从后缀表达式构建表达式树
ExpressionBinaryTreeNode* ExpressionBinaryTreeNode::buildFromPostfix(const string& postfix) {
    stack<ExpressionBinaryTreeNode*> nodeStack;  // 用于存储节点的栈
    
    // 从左到右扫描后缀表达式
    for (char ch : postfix) {
        if (isdigit(ch)) {
            // 如果是操作数（数字），创建节点并入栈
            ExpressionBinaryTreeNode* node = new ExpressionBinaryTreeNode(string(1, ch));
            nodeStack.push(node);
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            // 如果是操作符，弹出两个操作数节点
            ExpressionBinaryTreeNode* right = nodeStack.top();
            nodeStack.pop();
            ExpressionBinaryTreeNode* left = nodeStack.top();
            nodeStack.pop();
            
            // 创建新的操作符节点，将弹出的两个节点作为其左右子树
            ExpressionBinaryTreeNode* opNode = new ExpressionBinaryTreeNode(string(1, ch));
            opNode->left = left;
            opNode->right = right;
            
            // 将新构建的子树入栈
            nodeStack.push(opNode);
        }
    }
    
    // 栈顶元素即为表达式树的根节点
    return nodeStack.top();
}

// 从前缀表达式构建表达式树
ExpressionBinaryTreeNode* ExpressionBinaryTreeNode::buildFromPrefix(const string& prefix) {
    stack<ExpressionBinaryTreeNode*> nodeStack;  // 用于存储节点的栈
    
    // 从右向左扫描前缀表达式
    for (int i = prefix.length() - 1; i >= 0; i--) {
        char ch = prefix[i];
        
        if (isdigit(ch)) {
            // 如果是操作数（数字），创建节点并入栈
            ExpressionBinaryTreeNode* node = new ExpressionBinaryTreeNode(string(1, ch));
            nodeStack.push(node);
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            // 如果是操作符，弹出两个操作数节点
            // 注意：前缀表达式从右向左扫描，所以先弹出的是左子树
            ExpressionBinaryTreeNode* left = nodeStack.top();
            nodeStack.pop();
            ExpressionBinaryTreeNode* right = nodeStack.top();
            nodeStack.pop();
            
            // 创建新的操作符节点，将弹出的两个节点作为其左右子树
            ExpressionBinaryTreeNode* opNode = new ExpressionBinaryTreeNode(string(1, ch));
            opNode->left = left;
            opNode->right = right;
            
            // 将新构建的子树入栈
            nodeStack.push(opNode);
        }
    }
    
    // 栈顶元素即为表达式树的根节点
    return nodeStack.top();
}

// 从中缀表达式构建表达式树
ExpressionBinaryTreeNode* ExpressionBinaryTreeNode::buildFromInfix(const string& infix) {
    stack<ExpressionBinaryTreeNode*> operandStack;  // 存储操作数的栈
    stack<char> operatorStack;  // 存储运算符的栈
    
    for (size_t i = 0; i < infix.size(); ++i) {
        char ch = infix[i];
        
        if (isdigit(ch)) {
            // 如果是数字，创建新的操作数节点并压入操作数栈
            operandStack.push(new ExpressionBinaryTreeNode(string(1, ch)));
        } else if (ch == '(') {
            // 如果是左括号，直接入运算符栈
            operatorStack.push(ch);
        } else if (ch == ')') {
            // 如果是右括号，弹出运算符直到遇到左括号
            while (!operatorStack.empty() && operatorStack.top() != '(') {
                char op = operatorStack.top();
                operatorStack.pop();
                ExpressionBinaryTreeNode* right = operandStack.top();
                operandStack.pop();
                ExpressionBinaryTreeNode* left = operandStack.top();
                operandStack.pop();
                ExpressionBinaryTreeNode* node = new ExpressionBinaryTreeNode(string(1, op));
                node->left = left;
                node->right = right;
                operandStack.push(node);
            }
            // 弹出左括号
            if (!operatorStack.empty() && operatorStack.top() == '(') {
                operatorStack.pop();
            }
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            // 处理运算符
            while (!operatorStack.empty() && getPrecedence(operatorStack.top()) >= getPrecedence(ch)) {
                char op = operatorStack.top();
                operatorStack.pop();
                ExpressionBinaryTreeNode* right = operandStack.top();
                operandStack.pop();
                ExpressionBinaryTreeNode* left = operandStack.top();
                operandStack.pop();
                ExpressionBinaryTreeNode* node = new ExpressionBinaryTreeNode(string(1, op));
                node->left = left;
                node->right = right;
                operandStack.push(node);
            }
            operatorStack.push(ch);
        }
    }

    // 处理剩余的运算符
    while (!operatorStack.empty()) {
        char op = operatorStack.top();
        operatorStack.pop();
        ExpressionBinaryTreeNode* right = operandStack.top();
        operandStack.pop();
        ExpressionBinaryTreeNode* left = operandStack.top();
        operandStack.pop();
        ExpressionBinaryTreeNode* node = new ExpressionBinaryTreeNode(string(1, op));
        node->left = left;
        node->right = right;
        operandStack.push(node);
    }

    // 返回最终构建好的表达式树的根节点
    return operandStack.top();
}

// 获取运算符的优先级
// 参数:
//    op - 运算符字符
// 返回:
//    返回运算符的优先级，数字越大优先级越高
int ExpressionBinaryTreeNode::getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;  // 加减优先级较低
    if (op == '*' || op == '/') return 2;  // 乘除优先级较高
    return INT_MAX;  // 不支持的运算符返回最大优先级
}

// 计算表达式树的值
// 返回:
//    返回表达式树的计算结果
int ExpressionBinaryTreeNode::evaluate() {
    // 如果是叶节点（没有子节点），则直接返回该节点的数字值
    if (!left && !right) {
        return stoi(value);  // 使用stoi将字符串转换为整数
    }
    
    // 否则递归计算左右子树的值
    int leftVal = dynamic_cast<ExpressionBinaryTreeNode*>(left)->evaluate();    
    int rightVal = dynamic_cast<ExpressionBinaryTreeNode*>(right)->evaluate();
    
    // 根据当前节点的运算符计算值
    if (value == "+") return leftVal + rightVal;
    if (value == "-") return leftVal - rightVal;
    if (value == "*") return leftVal * rightVal;
    if (value == "/") return leftVal / rightVal;
    
    // 如果是无效的运算符，抛出异常
    throw invalid_argument("Invalid operator");
}
