/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

/* 输入参数是Queue表示的已经按照从小到大的顺序增加队列a和b，
 * 返回值是一个包含队列a和b中所有元素并且从小到大的顺序增加的队列result。
 *
 * 这个函数将两个独立的有序序列合并成一个更大的有序序列。
 */
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    //当两列都有元素时
    while (!a.isEmpty() && !b.isEmpty()) {
        //将较小者添加到合并序列中
        int resultMem = a.peek() < b.peek() ? a.peek() : b.peek();
        result.enqueue(resultMem);
        //将原序列已经排过序的元素去掉
        a.peek() < b.peek() ? a.dequeue() : b.dequeue();
        //强制合并前提条件
        if (!a.isEmpty() && !b.isEmpty()) {
            int record = a.peek() < b.peek() ? a.peek() : b.peek();
            if (resultMem > record) {
                error("There's something wrong with the sequence inputted");
            }
        }
    }

    //当只有一列还有元素时
    Queue<int> restOne = a.size() > b.size() ? a : b;
    while (!restOne.isEmpty()) {
        int record = restOne.dequeue();
        result.enqueue(record);
        //强制合并前提条件
        if (!restOne.isEmpty() && record > restOne.peek()) {
            error("There's something wrong with the sequence inputted");
        }
    }

    return result;

/*
 * 递归方式
 */
//    Queue<int> result;
//    //当其中一列已经没有元素时
//    if (b.isEmpty() || a.isEmpty()) {
//        Queue<int> c = (a.size() > b.size() ? a : b);
//        while (!c.isEmpty()) {
//            result.enqueue(c.dequeue());
//        }
//        return result;
//    }
//    //将较小者添加到合并序列中
//    int resultMem = a.peek() < b.peek() ? a.peek() : b.peek();
//    result.enqueue(resultMem);
//    //去掉已经排过序的元素
//    a.peek() < b.peek() ? a.dequeue() : b.dequeue();
//    //等价于result += binaryMerge(a, b); 不过Queue没有+=操作符
//    Queue<int> replace = binaryMerge(a, b);
//    while (!replace.isEmpty()) {
//        result.enqueue(replace.dequeue());
//    }

//    return result;
}

/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

/*
 *
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    // base case
    if (all.size() == 1) {
        return binaryMerge(all.get(0), result);
    } else if (all.size() == 2) {
        return binaryMerge(all.get(0), all.get(1));
    }
    //前半部分是向量中的前一半序列，后半部分是向量中的后一半序列。
    Vector<Queue<int>> frontPart = all.subList(0, all.size()/2);
    Vector<Queue<int>> backPart = all.subList(all.size()/2);
    //分别处理前半部分和后半部分序列，得到两个排好序的子序列
    Queue<int> frontResult = recMultiMerge(frontPart);
    Queue<int> backResult = recMultiMerge(backPart);
    //合并上述两个子序列
    Vector<Queue<int>> composition = {frontResult, backResult};
    result = recMultiMerge(composition);

    return result;
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}

/* * * * * * Student Cases * * * * * */
STUDENT_TEST("binaryMerge, two sequences of unequal length ") {
    Queue<int> a = {1, 2, 2};
    Queue<int> b = {1, 3};
    EXPECT_EQUAL(binaryMerge(a, b), {1, 1, 2, 2, 3});
    EXPECT_EQUAL(binaryMerge(b, a), {1, 1, 2, 2, 3});
}

STUDENT_TEST("binaryMerge, two short sequences of unequal length with one wrong") {
    Queue<int> a = {-2, 4, 5};
    Queue<int> b = {1, 2, -3, 4};//wrong
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));
}

STUDENT_TEST("naiveMultiMerge, small collection of same sequences") {
    Vector<Queue<int>> all = {{1, 1, 2, 2, 3},
                              {1, 1, 2, 2, 3},
                              {1, 1, 2, 2, 3},
                              {1, 1, 2, 2, 3},
                             };
    Queue<int> expected = {1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

STUDENT_TEST("naiveMultiMerge, edge test") {
    //0 个队列
    Vector<Queue<int>> all = {};
    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
    //多个空队列
    all = {{},{},{},{}};
    expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}
