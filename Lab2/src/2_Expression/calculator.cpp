#include "calculator.h"

namespace DATA_STRUCTURE {

std::string calculator::get_expr() {
    return expr;
}

// 表达式合法性
bool calculator::legal() {  
    int bracket = 0;
    bool last_is_operator = true;  // 标记上一个字符是否为运算符
    
    for (int i = 0; i < expr.length(); i++) {
        char c = expr[i];
        
        // 1. 括号匹配检查
        if (c == '(') {
            bracket++;
            last_is_operator = true;
        } else if (c == ')') {
            bracket--;
            if (bracket < 0) return false;
            last_is_operator = false;
        }
        // 2. 运算符合法性检查
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            // 连续运算符
            if (last_is_operator && i > 0 && expr[i-1] != '(') {
                return false;
            }
            // 最后一个字符
            if (i == expr.length() - 1) {
                return false;
            }
            last_is_operator = true;
        }
        // 3. 数字或小数点
        else if (isdigit(c) || c == '.') {
            last_is_operator = false;
        }
    }
    
    return bracket == 0 && !last_is_operator;
}

// 运算符的优先级
int calculator::priority_regular(char c) {
    if (c == '(') {
        return 0;  // 括号
    } else if (c == '+' || c == '-') {
        return 1;  // 加减
    } else if (c == '*' || c == '/') {
        return 2;  // 乘除
    }
    return 0;
}

// 比较两个运算符的优先级
int calculator::priority(char c1, char c2) {
    int p1 = priority_regular(c1);
    int p2 = priority_regular(c2);
    
    if (p1 > p2) return 1;
    else if (p1 == p2) return 0;
    else return -1;
}

// 从表达式字符串中读取⼀个完整的数字
struct calculator::element calculator::read_num() {
    int num = 0, expo = 0, flag = 0;
    double result = 0.0;
    bool has_dot = false;  // 是否遇到小数点
    int decimal_places = 0;  // 小数位数
    
    // 读取
    while (expr_index < expr.length() && 
           (isdigit(expr[expr_index]) || expr[expr_index] == '.')) {
        if (expr[expr_index] == '.') {
            has_dot = true;  // 遇到小数点
            flag = 1;  // 标记为浮点数
            expr_index++;
            continue;
        }
        
        if (!has_dot) { // 处理整数部分
            num = num * 10 + (expr[expr_index] - '0');
            result = result * 10 + (expr[expr_index] - '0');
        } else {    // 处理小数部分
            decimal_places++;
            result = result + (expr[expr_index] - '0') / pow(10, decimal_places);
        }
        expr_index++;
    }

    if (flag == 1) {
        return {flag, 0, result};
    } else {
        return {flag, num, 0};
    }
}

struct calculator::element calculator::operate(struct calculator::element element1, char c, struct calculator::element element2) {
    struct element result;
    
    double val1 = (element1.flag == 0) ? element1.num_int : element1.num_double;
    double val2 = (element2.flag == 0) ? element2.num_int : element2.num_double;
    
    // 如果任一操作数是浮点数，结果就是浮点数
    if (element1.flag == 1 || element2.flag == 1) {
        result.flag = 1;
        
        switch (c) {
            case '+':
                result.num_double = val1 + val2;
                break;
            case '-':
                result.num_double = val1 - val2;
                break;
            case '*':
                result.num_double = val1 * val2;
                break;
            case '/':
                result.num_double = val1 / val2;
                break;
        }
        result.num_int = 0;
    } else {
        result.flag = 0;
        result.num_double = 0;
        
        switch (c) {
            case '+':
                result.num_int = element1.num_int + element2.num_int;
                break;
            case '-':
                result.num_int = element1.num_int - element2.num_int;
                break;
            case '*':
                result.num_int = element1.num_int * element2.num_int;
                break;
            case '/':   // 整数除法，如果不能整除，转为浮点数
                if (element2.num_int != 0 && element1.num_int % element2.num_int == 0) {
                    result.num_int = element1.num_int / element2.num_int;
                } else {
                    result.flag = 1;
                    result.num_double = (double)element1.num_int / element2.num_int;
                    result.num_int = 0;
                }
                break;
        }
    }
    
    return result;
}

// 计算器的核心函数
struct calculator::element calculator::get_ans() {
    num.clear();    // num栈存储操作数
    op.clear();     // op栈存储运算符
    expr_index = 0;
    
    // 辅助函数：执行一次运算（从num栈弹出两个数，从op栈弹出一个运算符）
    auto calculate_once = [this]() {
        char operator_c = op.top();
        op.pop();
        struct element num2 = num.top();
        num.pop();
        struct element num1 = num.top();
        num.pop();
        
        struct element result = operate(num1, operator_c, num2);
        num.push(result);
    };

    // 遍历整个表达式
    while (expr_index < expr.length()) {
        char c = expr[expr_index];
        
        if (isdigit(c)) { // 如果是数字
            struct element num_elem = read_num();
            num.push(num_elem);
        } 

        else if (c == '(') {
            op.push(c);
            expr_index++;
        } 
        else if (c == ')') {// 右括号：弹出运算符直到遇到左括号
            while (!op.empty() && op.top() != '(') {
                calculate_once();
            }
            if (!op.empty()) op.pop();
            expr_index++;
        } 

        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            while (!op.empty() && op.top() != '(' && 
                   priority(op.top(), c) >= 0) {
                calculate_once();
            }
            op.push(c);
            expr_index++;
        } 

        else expr_index++;  // 跳过其他字符
    }
    
    // 处理栈中剩余的运算符
    while (!op.empty()) calculate_once();
    
    return num.top();
}

}