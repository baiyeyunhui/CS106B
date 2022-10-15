/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include <string>
#include "strlib.h"
#include "testing/SimpleTest.h"

using namespace std;

/*
 * 输入参数是一个string表示的字符串str，返回值是一个string表示的只包含括号操作符的字符串result。
 *
 * 这个函数是从输入的字符串中提取出只包含括号操作符的字符串。
 */
string operatorsFrom(string str) {    
    if (str.empty()) {
       return "";
    }

    //做递归调用处理剩下的字符串并保存结果
    string rest = str.substr(1);
    string result = operatorsFrom(rest);
    //处理第一个字符，确定是否可以放到最终结果中
    if (str[0] == '{' || str[0] == '[' || str[0] == '(' || str[0] == ')' || str[0] == ']' || str[0] == '}') {
        //整合第一个字符处理结果和递归调用处理结果，
        result = str[0] + result;
    }
    //返回只包含括号操作符的最终形式。
    return result;
}

/*
 * 输入参数是一个string表示的只包含括号操作符的字符串ops，返回值是一个判断括号是否配对的bool数据
 *
 * 这个函数是确定在只包含括号操作符的字符串中所有括号是否配对
 */
bool operatorsAreMatched(string ops) {
    if (ops.size() == 0) {//字符串为空
        return true;
    }

    //字符串包括"()", "[]", or "{}"这样的子字符串，并且在剔除后，剩下的部分依然平衡
    string rest = "";
    if (ops.find("()") != string::npos) {
        rest = ops.erase(ops.find("()"), 2);
        return operatorsAreMatched(rest);
    }
    if (ops.find("{}") != string::npos) {
        rest = ops.erase(ops.find("{}"), 2);
        return operatorsAreMatched(rest);
    }
    if (ops.find("[]") != string::npos) {
        rest = ops.erase(ops.find("[]"), 2);
        return operatorsAreMatched(rest);
    }

    return false;
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}

/* * * * * * Student Cases * * * * * */

STUDENT_TEST("operatorsFrom on examples") {
    EXPECT_EQUAL(operatorsFrom("dsd{[(e)e]}as"), "{[()]}");
    EXPECT_EQUAL(operatorsFrom("ds(ds)fs{[4(e)+(e)e]}da"), "(){[()()]}");
    EXPECT_EQUAL(operatorsFrom("dsa(ds)fs{[(4(e)+5)+(e)e]dsa}da"), "(){[(())()]}");
}

STUDENT_TEST("operatorsAreMatched on balanced examples from writeup") {
    EXPECT(operatorsAreMatched("{[()]}"));
    EXPECT(operatorsAreMatched("(){[()()]}"));
    EXPECT(operatorsAreMatched("(){[(())()]}"));
}

STUDENT_TEST("operatorsAreMatched on non-balanced examples from writeup") {
    EXPECT(!operatorsAreMatched("( (  ] )"));
    EXPECT(!operatorsAreMatched("} ) ( [ ["));
    EXPECT(!operatorsAreMatched("{ ( [] } { )"));
    EXPECT(!operatorsAreMatched("({ a })"));
}
