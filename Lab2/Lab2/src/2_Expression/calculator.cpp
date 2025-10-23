#include "calculator.h"

namespace DATA_STRUCTURE {

std::string calculator::get_expr() {
    return expr;
}

bool calculator::legal() {
    int bracket = 0;
    // TODO
    return bracket == 0;
}

int calculator::priority_regular(char c) {
    // TODO
    return 0;
}

int calculator::priority(char c1, char c2) {
    // TODO
    return 0;
}

struct calculator::element calculator::read_num() {
    int num = 0, expo = 0, flag = 0;
    // TODO

    return {flag, num, 0};
}

struct calculator::element calculator::operate(struct calculator::element element1, char c, struct calculator::element element2) {
    // TODO
    return {0, 0, 0};
}

struct calculator::element calculator::get_ans() {
    // TODO
    return {0, 0, 0};
}

}