#include "calculator.h"
#include <limits>

namespace DATA_STRUCTURE {

std::string calculator::get_expr() {
    return expr;
}

// 表达式合法性
bool calculator::legal() {
    int bracket = 0;
    bool last_is_operator = true;  // 标记上一个字符是否为运算符
    bool has_number = false;  // 标记是否包含数字
    
    for (size_t i = 0; i < expr.length(); i++) {
        char c = expr[i];
        
        // 1. 括号匹配检查
        if (c == '(') {
            bracket++;
            last_is_operator = true;
        } else if (c == ')') {
            bracket--;
            if (bracket < 0) return false;
            // 检查空括号：右括号前面是左括号
            if (i > 0 && expr[i-1] == '(') return false;
            last_is_operator = false;
        }
        // 2. 运算符合法性检查
        else if (c == '+' || c == '*' || c == '/' || c == '^') {
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
        // 3. 减号特殊处理（可能是负号）
        else if (c == '-') {
            // 如果是负号（开头、左括号后、运算符后），不算作运算符
            if (i == 0 || expr[i-1] == '(' || is_operator(expr[i-1])) {
                // 负号后面必须有数字或左括号
                if (i == expr.length() - 1) return false;
                if (i + 1 < expr.length() && !isdigit(expr[i+1]) && expr[i+1] != '(' && expr[i+1] != '.') {
                    return false;
                }
                last_is_operator = false;  // 负号不算运算符
            } else {
                // 作为减法运算符
                if (last_is_operator) return false;
                if (i == expr.length() - 1) return false;
                last_is_operator = true;
            }
        }
        // 4. 数字或小数点
        else if (isdigit(c) || c == '.') {
            last_is_operator = false;
            if (isdigit(c)) { 
                has_number = true;
            }
        }
    }
    
    return bracket == 0 && !last_is_operator && has_number;
}

// 运算符的优先级
int calculator::priority_regular(char c) {
    if (c == '(') {
        return 0;  // 括号
    } else if (c == '+' || c == '-') {
        return 1;  // 加减
    } else if (c == '*' || c == '/') {
        return 2;  // 乘除
    } else if (c == '^') {
        return 3;  // 乘方（最高优先级）
    }
    return 0;
}

// 比较两个运算符的优先级
int calculator::priority(char c1, char c2) {
    int p1 = priority_regular(c1);
    int p2 = priority_regular(c2);
    
    // 乘方运算是右结合的，其他运算符是左结合的
    if (c1 == '^' && c2 == '^') return -1;  // 右结合：栈顶是^，新来的也是^，不弹出
    
    if (p1 > p2) return 1;
    else if (p1 == p2) return 0;
    else return -1;
}

// 判断是否为运算符
bool calculator::is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// 判断当前位置的'-'是否为负号（而非减法运算符）
bool calculator::is_unary_minus() {
    if (expr[expr_index] != '-') return false;
    
    // 表达式开头的'-'是负号
    if (expr_index == 0) return true;
    
    char prev = expr[expr_index - 1];
    // 左括号后或运算符后的'-'是负号
    return prev == '(' || is_operator(prev);
}

// 从表达式字符串中读取⼀个完整的数字
struct calculator::element calculator::read_num() {
    int num = 0, flag = 0;
    double result = 0.0;
    bool has_dot = false;  // 是否遇到小数点
    int decimal_places = 0;  // 小数位数
    
    // 读取
    while (expr_index < expr.length() && 
           (isdigit(expr[expr_index]) || expr[expr_index] == '.')) {
        if (expr[expr_index] == '.') {
            // 防止多个小数点
            if (has_dot) break;
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
    
    // 如果任一操作数是浮点数，或者是除法/乘方运算，结果就是浮点数
    if (element1.flag == 1 || element2.flag == 1 || c == '/' || c == '^') {
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
                if (val2 == 0) {
                    // 除零返回带符号的无穷大
                    result.num_double = (val1 >= 0) ? 
                        std::numeric_limits<double>::infinity() : 
                        -std::numeric_limits<double>::infinity();
                } else {
                    result.num_double = val1 / val2;
                }
                break;
            case '^':
                result.num_double = pow(val1, val2);
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
            case '/':   // 整数除法
                if (element2.num_int == 0) {
                    // 整数除零，转为浮点数并返回带符号的无穷大
                    result.flag = 1;
                    result.num_double = (element1.num_int >= 0) ? 
                        std::numeric_limits<double>::infinity() : 
                        -std::numeric_limits<double>::infinity();
                    result.num_int = 0;
                } else if (element1.num_int % element2.num_int == 0) {
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
        if (op.empty() || num.empty()) return;  // 安全检查
        
        char operator_c = op.top();
        op.pop();
        
        if (num.empty()) return;  // 再次检查
        struct element num2 = num.top();
        num.pop();
        
        if (num.empty()) return;  // 再次检查
        struct element num1 = num.top();
        num.pop();
        
        struct element result = operate(num1, operator_c, num2);
        num.push(result);
    };

    // 遍历整个表达式
    while (expr_index < expr.length()) {
        char c = expr[expr_index];
        
        // 处理负号：统一转换为 0 - x 的形式
        if (c == '-' && is_unary_minus()) {
            // 将 0 入栈
            struct element zero = {0, 0, 0.0};
            num.push(zero);
            // 将减号作为二元运算符入栈
            op.push('-');
            expr_index++;
        }
        
        else if (isdigit(c) || c == '.') { // 如果是数字或小数点开头
            struct element num_elem = read_num();
            num.push(num_elem);
        } 

        else if (c == '(') {
            op.push(c);
            expr_index++;
        } else if (c == ')') {  // 右括号：弹出运算符直到遇到左括号
            while (!op.empty() && op.top() != '(') {
                calculate_once();
            }
            if (!op.empty()) op.pop();  // 弹出左括号
            expr_index++;
        } 

        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            while (!op.empty() && op.top() != '(' && 
                   priority(op.top(), c) >= 0) {
                calculate_once();
            }
            op.push(c);
            expr_index++;
        } 

        else expr_index++;  // 跳过其他字符（空格等）
    }
    
    // 处理栈中剩余的运算符
    while (!op.empty()) calculate_once();
    
    // 确保结果栈不为空
    if (num.empty()) {
        return {0, 0, 0.0};  // 返回默认值0
    }
    
    return num.top();
}

}