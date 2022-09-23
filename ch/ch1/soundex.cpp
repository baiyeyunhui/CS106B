/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
#include "set.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * 如果s[0]是标点符号，数字，空格，result中就会包含非字母字符，
 * 所以可以将初始result赋予空字符串。
 */
string removeNonLetters(string s) {
//    string result = charToString(s[0]);//bug
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}

/* 输入只有字母的字符串s，按一定对应规则为字母设置标识字符，
 * 如果字符串s元素的大写与标识字符所包含的字母一致，则进行相应的编码，
 * 输出相应的数字字符串来表示字符串s的编码
 */
string encode(string s) {
    string result = "";
    Set<char> zero{'A', 'E', 'I', 'O', 'U', 'H', 'W', 'Y'};
    Set<char> one{'B', 'F', 'P', 'V'};
    Set<char> two{'C', 'G', 'J', 'K', 'Q', 'S', 'X', 'Z'};
    Set<char> three{'D', 'T'};
    Set<char> four{'L'};
    Set<char> five{'M', 'N'};
    Set<char> six{'R'};

    for (int i = 0; i < s.length(); i++) {
        if (zero.contains(toUpperCase(s[i])))
            result += "0";
        if (one.contains(toUpperCase(s[i])))
            result += "1";
        if (two.contains(toUpperCase(s[i])))
            result += "2";
        if (three.contains(toUpperCase(s[i])))
            result += "3";
        if (four.contains(toUpperCase(s[i])))
            result += "4";
        if (five.contains(toUpperCase(s[i])))
            result += "5";
        if (six.contains(toUpperCase(s[i])))
            result += "6";
    }

    return result;
}

/* 输入字符串，输出相邻没有重复字符的字符串。
 * 用临时字符cur表示s中的前一个字符，当s中的后一个字符
 * 与前一个字符相同时，跳过本次循环；否则，将后一个字符
 * 赋给cur，并增添进记录字符串result中。rusult的初始
 * 赋值是s的第一个字符组成字符串
 *
 */
string coalesce(string s) {
    string result = charToString(s[0]);//将字符装换成字符串
    char cur = s[0];
    for (int i = 1; i < s.length(); i++) {//遍历s中的除去第一个字符外的所有字符
        if (cur == s[i])
            continue;//如果相邻字符相同，跳过本次循环

        cur = s[i];
        result += s[i];
    }
    return result;
}

/* 输入字符串s和字母字符串origin，输出第一个字符被字母字符串origin第一个字母大写替换的字符串s
 *
 */
string header(string s, string orgin)
{
//    s[0] = toUpperCase(orgin[0]);
  s.replace(0, 1, charToString(toUpperCase(orgin[0])));//.replace(int, int, string)
  return s;
}


/* 输入字符串s, 输出不含有'0'的字符串
 * 遍历s中所有的字符，如果字符不是'0'，
 * 就将字符记录在result；否则，跳过本次循环。
 */
string remove_zero(string s) {
    string result = charToString(s[0]);
    for (int i = 1; i < s.length(); i++) {
//        if (charToInteger(s[i]) == 0)
        if (s[i] == '0')
            continue;
        result += s[i];
    }
    return result;
}

/* 输入字符串s, 输出占有4位的字符串
 * 只遍历4次，如果字符串s中的字符数大于4，
 * 直接赋值，记录在result中；否则，无字符位
 * 直接用"0"进行补位。
 */
string round(string s) {
    string result = "";
    for (int i = 0; i < 4; i++) {
        if (i >= s.length())
            result += "0";
        else
            result += s[i];
    }
    return result;
}

/* 最终目标函数
 */
string soundex(string s) {
    return round(remove_zero(header(coalesce(encode(removeNonLetters(s))), s)));
}


/* 输入数据库文件名，输出所有具有相同编码且排好序的姓氏
 * 1.通过向量读取姓氏数据库文件，并输出数据库大小
 * 2.如果用户一直查询，就要一直提示用户输入要查询的姓氏，
 * 并输出其soundex编码作为提示信息，之后与数据库姓氏的
 * soundex编码逐一比对。若soundex编码相同，将这一姓氏增
 * 添到存储匹配相同姓氏的向量，最后进行排序，打印。
 * 3.如果用户没有查询，输入回车，退出此函数
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames

    /* TODO: Fill in the remainder of this function. */

    while (true) {
      string name = getLine("Enter a surname (RETURN to quit): ");
      if (name.empty()) {
        cout << "All done!" << endl;
        break;
      }

      string code = soundex(name);

      cout << "Soundex code is " << code << endl;

      Vector<string> matchNames;
      for (const string& name : databaseNames) {
        if (soundex(name) == code)
          matchNames.add(name);
      }
      matchNames.sort();
      cout << "Matches from database: " << matchNames << endl;
    }
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing punctuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}

PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "-xyz";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "xyz");
}

PROVIDED_TEST("encode string") {
    EXPECT_EQUAL(encode("Curie"), "20600");
}

PROVIDED_TEST("coalesce encoder") {
    EXPECT_EQUAL(coalesce("20600"), "2060");
    EXPECT_EQUAL(coalesce("222025"), "2025");
}

PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here

STUDENT_TEST("my trials of removeNonLetters")
{
    string s = " 1, 1 ";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
}

STUDENT_TEST("my trials of encode")
{
    string s = "sun";
    string result = encode(s);
    EXPECT_EQUAL(result, "205");
}

STUDENT_TEST("my trials of encode")
{
    string s = "sun";
    string result = encode(s);
    EXPECT_EQUAL(result, "205");
}

STUDENT_TEST("my trials of coalesce")
{
    string s = "205";
    string result = coalesce(s);
    EXPECT_EQUAL(result, "205");

    s = "0";
    result = coalesce(s);
    EXPECT_EQUAL(result, "0");

    s = "000";
    result = coalesce(s);
    EXPECT_EQUAL(result, "0");

    s = "001100";
    result = coalesce(s);
    EXPECT_EQUAL(result, "010");
}

STUDENT_TEST("my trials of header")
{
    string s = "205";
    string orgin = "sun";
    string result = header(s, orgin);
    EXPECT_EQUAL(result, "S05");
    EXPECT_EQUAL(header(s, "SUN"), "S05");

    EXPECT_EQUAL(header(s, "&S. U, 7=&N"), "S05");//bug
}

STUDENT_TEST("my trials of remove_zero")
{
    string s = "S0005050";
    string result = remove_zero(s);
    EXPECT_EQUAL(result, "S55");

    s = "S000000";
    result = remove_zero(s);
    EXPECT_EQUAL(result, "S");
}

STUDENT_TEST("my trials of round")
{
    string s = "S5512445";
    string result = round(s);
    EXPECT_EQUAL(result, "S551");

    s = "S55";
    result = round(s);
    EXPECT_EQUAL(result, "S550");

    s = "S";
    result = round(s);
    EXPECT_EQUAL(result, "S000");
}

STUDENT_TEST("my trials of soundex")
{
    EXPECT_EQUAL(soundex("sun"), "S500");
    EXPECT_EQUAL(soundex("Sun"), "S500");
    EXPECT_EQUAL(soundex("Angelou"), "A524");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");

    EXPECT_EQUAL(soundex("_sun"), "S500");//bug
}
