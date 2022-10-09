// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "simpio.h"  // for getLine()
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* 输入参数为string表示的字符串s，返回值是一个“纯净版”的字符串。
 * 这个函数是用来去除字符串首尾的标点，并将所有字母转换为小写，
 * 也会检查字符串是否全为非字母字符，如果都是非字母，则丢弃该字符串并返回一个空字符串。
 */
string cleanToken(string s)
{
    //只去除字符串首尾的标点符号，但不要管字符串内部的标点
    string rest = "";
    for (int i = 0; i < s.length(); i++) {
        if (!ispunct(s[i])) {
            rest = s.substr(i);
            break;
        }
    }
    s.erase(0);//无论s是否全部是标点，都先清空s
    for (int i = rest.length()-1; i >= 0; i--) {
        if (!ispunct(rest[i])) {
            s = rest.substr(0, i+1);
            break;
        }
    }
    //确认字符串至少包含一个字符
    for (char elem : s) {
        if (isalpha(elem)) {
            //将字符串转换为小写
            s = toLowerCase(s);
            return s;
        }
    }
    return "";//字符串没有一个字符，返回空字符串
}

/* 输入参数是一个string表示的的字符串text，输出的是Set<string>表示的字符串集合tokens。
 * 这个函数的作用是从一个包含网页正文文本的字符串中提取出所有不重复的字符串的集合。
 */
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    //把正文文本拆分成一个个独立的子字符串
    Vector<string> v = stringSplit(text, " ");
    //调用cleanTokens处理每个子字符串，并将结果存放到集合中
    for (string elem : v) {
        //单词经过标准化处理,变成“纯净版”的字符串
        elem = cleanToken(elem);
        //存放网页文本中所有单词不能重复且不能是空字符串
        if (!tokens.contains(elem) && elem != "") {
            tokens.add(elem);
        }
    }
    //没有“纯净版”的字符串，返回空字符串
    if (tokens.isEmpty()) {
        return {""};
    }
    return tokens;
}

/* 输入参数是string表示的网页数据库文件名字dbfile和
 * Map<string, Set<string>>表示的反向索引数据index，
 * 返回值是in表示的从数据库文件中处理过的网页数量netPages。
 *
 * 这个函数从数据库文件读取内容，并将其处理为反向索引的形式。
 */
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    //把文件打开，并把内容存入一个字符串向量
    ifstream in;
    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);
    Vector<string> lines;
    readEntireFile(in, lines);
    //没有考虑网址重复的情况
    int netPages = lines.size()/2;
    //从url到该页面上所有“纯净版”字符串的映射
    for (int i=0; i<netPages; i++) {
        string memOfUrl = lines.get(2*i);
        //提取出网页文本中独一无二的字符串集合
        Set<string> NetText = gatherTokens(lines.get(2*i+1));
        //根据每一个字符串，将该页面的URL添加到反向索引的条目中
        for (const string& item : NetText) {
            if (!index.containsKey(item)) {
                index.put(item, {memOfUrl});
                continue;
            }
            Set<string> urlOfNetText = index.get(item);
//            if (!urlOfNetText.contains(memOfUrl)) {//考虑同一网页有多个相同单词
                urlOfNetText.add(memOfUrl);
                index.put(item, urlOfNetText);
//            }

//            if (!index.get(item).contains(memOfUrl)) {
//                index.get(item).add(memOfUrl);
//                index.put(item, index.get(item));//这个为什么没有把memOfUrl加进去呢
//            }
        }
    }

    return netPages;
}

/* 输入参数是Map<string, Set<string>>表示的反向索引数据index
 * 和string表示的搜索关键词query，返回值是Set<string>表示的是
 * 反向索引数据中与搜索关键词匹配的网址。
 *
 * 这个函数利用给定查询关键词在反向索引数据中进行搜索，返回与搜索关键词匹配的网址。
 */
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    //把查询的关键词拆分成一个个独立的子字符串
    query = toLowerCase(query);
    Vector<string> v = stringSplit(query, " ");
    //对于单个搜索关键词
    if (v.size() == 1) {
        result = index.get(v.get(0));
        return result;
    }
    //对于组合搜索关键词
    result = index.get(v.get(0));
    for (string elem : v.subList(1, v.size()-1)) {
        //关键词使用+号连接,求交集
        if (elem[0] == '+') {
            elem = cleanToken(elem);
            result.intersect(index.get(elem));
            continue;
        }
        //关键词使用-号连接，求差集
        if (elem[0] == '-') {
            elem = cleanToken(elem);
            result.difference(index.get(elem));
            continue;
        }
        //使用修饰符连接关键词，求并集
        elem = cleanToken(elem);
        result.unionWith(index.get(elem));
    }

    return result;
}

/* 输入参数是Map<string, Set<string>>表示的反向索引数据index
 * 和string表示的搜索关键词query，返回值是Set<string>表示的是
 * 反向索引数据中与搜索关键词匹配的网址。
 *
 * 这个函数利用给定查询关键词在反向索引数据中进行搜索，返回与搜索关键词匹配的网址。
 */
void searchEngine(string dbfile)
{
    //根据数据库文件内容，构建一个反向索引
    Map<string, Set<string>> index;
    //构建反向索引用到的网页数量及所有页面中包含独一无二单词的个数
    int netPages = buildIndex(dbfile, index);
    int numOfWords = index.keys().size();
    cout << "Stand by while building index..." << endl;
    cout << "Indexed " << netPages << " pages containing "
         << numOfWords << " unique terms\n" << endl;

    while (true) {
        //提示用户输入查询关键词
        string query = getLine("Enter query sentence (RETURN/ENTER to quit): ");
        //当用户输入空字符串时，程序结束
        if (query == "") {
            break;
        }
        //对于每一个查询关键词，找出所有匹配页面并打印出 URLs
        Set<string> result = findQueryMatches(index, query);
        cout << "Found " << result.size() << " matching pages" << endl;
        cout << result << endl << endl;
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}

// TODO: add your test cases here
STUDENT_TEST("cleanToken on strings") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLDdd!!!"), "worlddd");
    EXPECT_EQUAL(cleanToken("~!!!^_^"), "");
    EXPECT_EQUAL(cleanToken("section-10"), "section-10");
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("fDsf*234B"), "fdsf*234b");
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("6!!!"), "");
    EXPECT_EQUAL(cleanToken("F6!!!"), "f6");
    EXPECT_EQUAL(cleanToken(""), "");
    EXPECT_EQUAL(cleanToken("i"), "i");
    EXPECT_EQUAL(cleanToken("1O6"), "1o6");
}

STUDENT_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {""};
    EXPECT_EQUAL(gatherTokens("1949 10 01!"), expected);
    EXPECT_EQUAL(gatherTokens("1* ** 10 0*!!"), expected);
    EXPECT_EQUAL(gatherTokens("1*a *b* 1F0 0*!!"), {"1*a", "b", "1f0"});
    EXPECT_EQUAL(gatherTokens("-1*a-"), {"1*a"});

}

STUDENT_TEST("buildIndex from tiny.txt") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
    EXPECT_EQUAL(index.get("red").size(), 2);
}

//STUDENT_TEST("buildIndex from tiny1.txt") {
//    Map<string, Set<string>> index;
//    int nPages = buildIndex("res/tiny1.txt", index);
//    EXPECT_EQUAL(nPages, 5);
//    EXPECT_EQUAL(index.size(), 11);
//    EXPECT(index.containsKey("fish"));
//}

STUDENT_TEST("time testing of buildIndex.txt") {
    Map<string, Set<string>> index;
//    TIME_OPERATION(1, buildIndex("res/tiny1.txt", index));
    TIME_OPERATION(1, buildIndex("res/website.txt", index));

}

STUDENT_TEST("findQueryMatches") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "RED");
    EXPECT_EQUAL(matchesRed.size(), 2);
    Set<string> matches = findQueryMatches(index, "Red Fish -I");
    EXPECT_EQUAL(matches.size(), 3);
    matches = findQueryMatches(index, "red +fish +greeN");
    EXPECT_EQUAL(matches.size(), 0);
    matches = findQueryMatches(index, "red -fish rEd");
    EXPECT_EQUAL(matches.size(), 2);


//    buildIndex("res/tiny1.txt", index);
//    matches = findQueryMatches(index, "Fish");
//    EXPECT_EQUAL(matches.size(), 3);
}

//STUDENT_TEST("findQueryMatches") {
//    Map<string, Set<string>> index;
//    buildIndex("res/tiny1.txt", index);
//    Set<string>  matches = findQueryMatches(index, "Fish");
//    EXPECT_EQUAL(matches.size(), 3);
//}
