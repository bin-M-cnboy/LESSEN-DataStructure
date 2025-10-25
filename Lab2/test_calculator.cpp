#include <iostream>
#include "src/2_Expression/calculator.h"

void test_expression(const std::string& expr_str) {
    DATA_STRUCTURE::calculator calc(expr_str);
    std::cout << "表达式: " << expr_str << std::endl;
    
    if (calc.legal()) {
        std::cout << "  合法性: ✓" << std::endl;
        auto result = calc.get_ans();
        std::cout << "  结果 = ";
        if (result.flag == 0)
            std::cout << result.num_int << std::endl;
        else
            std::cout << result.num_double << std::endl;
    } else {
        std::cout << "  合法性: ✗ (非法表达式)" << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "========== 表达式计算器测试 ==========" << std::endl << std::endl;
    
    // 基本运算
    std::cout << "--- 基本运算 ---" << std::endl;
    test_expression("1+2");
    test_expression("10-3");
    test_expression("4*5");
    test_expression("10/2");
    
    // 浮点数运算
    std::cout << "--- 浮点数运算 ---" << std::endl;
    test_expression("3.14+2.86");
    test_expression("10*3.2");
    test_expression("7/2");
    
    // 负数运算
    std::cout << "--- 负数运算 ---" << std::endl;
    test_expression("-5+3");
    test_expression("10+(-3)");
    test_expression("-5*-3");
    test_expression("-(2+3)");
    test_expression("-5.5+2.5");
    
    // 乘方运算
    std::cout << "--- 乘方运算 ---" << std::endl;
    test_expression("2^3");
    test_expression("2^10");
    test_expression("2.5^2");
    test_expression("2^3^2");  // 右结合：2^(3^2) = 2^9 = 512
    test_expression("(-2)^3");
    
    // 除零运算
    std::cout << "--- 除零运算 ---" << std::endl;
    test_expression("5/0");
    test_expression("-5/0");
    test_expression("0/0");
    
    // 复杂表达式
    std::cout << "--- 复杂表达式 ---" << std::endl;
    test_expression("1+2*(3-1+4)");
    test_expression("10*3.2+2*(2+3/2)");
    test_expression("2^3+4*5-6/2");
    test_expression("(2+3)*(4-1)");
    test_expression("-5+10*2-(-3)");
    test_expression("2^(3+1)*5");
    
    // 非法表达式
    std::cout << "--- 非法表达式测试 ---" << std::endl;
    test_expression("1++2");
    test_expression("(1+2");
    test_expression("1+2)");
    test_expression("1+*2");
    test_expression("1+");
    
    return 0;
}
