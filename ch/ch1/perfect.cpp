/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include "testing/SimpleTest.h"
#include <iostream>
#include "perfect.h"
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {//找到除本身外的因数，进行相加求和
            total += divisor;
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {//完美数是非零正数，且等于除其本身外所有因数的和
    return (n != 0) && (n == divisorSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {//查找区间[1,stop)上的完美数
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0)
            cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* 这个函数输入正整数n,返回除n本身外所有因数的和，与divisorSum功能相同。
 * 然而不同的是，这个函数只遍历从2到n的平方根，找出这个区间的除数，再通过
 * 配对每个除数对应的成对因子，找到从n的平方根到n的除数。因为1是所有正整数
 * 的除数，所以将初始total赋予1。
 */
long smarterSum(long n) {
    // edge case
    if (n <= 1)
        return 0;

    long total = 1;
    for (long divisor = 2; divisor <= sqrt(n); divisor++) {
        if (n % divisor == 0) {
            total += divisor;
            if (divisor < sqrt(n)) { // edge case
                long corresponding = n / divisor;
                total += corresponding;
            }
        }
    }

    return total;
}

//与isPerfectS一样，只不过这里完美数判断用的是smarterSum(n)与n相等
bool isPerfectSmarter(long n) {
    return (n != 0) && (n == smarterSum(n));
}

//findPerfects一样，只不过这里完美数判断用的是isPerfectSmarter
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0)
            cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/*输入一个梅森数n,判断n是否是质数,
 * 如果是返回true,否返回false。
 */
bool isPrime(long n) {
    return smarterSum(n) == 1;
}

/*输入完美数序数n,返回第n个完美数
 * 1、从k=1开始
 * 2、计算第k个梅森数
 * 3、如果第k个梅森数是质数，且完美数序数n变为0，则pow(2, k - 1) * (pow(2, k) - 1)就是第n个完美数，返回值
 * 4、如果第k个梅森数不是质数，或完美数序数n没有变为0，则k递增。
*/
long findNthPerfectEuclid(long n) {
    long k = 1;
    while (true) {
        long m = pow(2, k) - 1; // A Mersenne number
        if (isPrime(m)) {//梅森数是质数
            n -= 1;
            if (n == 0)//返回第n个完美数，退出此函数
                return pow(2, k - 1) * (pow(2, k) - 1); // the nth perfect number
        }
        k++;
    }
    return 0;
}



/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
    EXPECT_EQUAL(divisorSum(4), 3);
}

PROVIDED_TEST("Confirm smarterSum of small inputs") {
    EXPECT_EQUAL(smarterSum(1), 0);
    EXPECT_EQUAL(smarterSum(6), 6);
    EXPECT_EQUAL(smarterSum(12), 16);
    EXPECT_EQUAL(smarterSum(4), 3);
}

PROVIDED_TEST("divisorSum vs smarterSum") {
    for (long i = 0; i < 1000; i++) {
        EXPECT_EQUAL(divisorSum(i), smarterSum(i));
    }
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfectSmarter(6));
    EXPECT(isPerfectSmarter(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfectSmarter(12));
    EXPECT(!isPerfectSmarter(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfectSmarter(0));
    EXPECT(!isPerfectSmarter(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfectSmarter(33550336));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10000, findPerfects(10000));
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10000, findPerfectsSmarter(10000));
    TIME_OPERATION(20000, findPerfectsSmarter(20000));
    TIME_OPERATION(40000, findPerfectsSmarter(40000));
}

// TODO: add your student test cases here

STUDENT_TEST("my time trials of findPerfects on doubleing input sizes")
{
//    TIME_OPERATION(1000, findPerfects(1000));
//    TIME_OPERATION(2000, findPerfects(2000));

    TIME_OPERATION(15000, findPerfects(15000));
    TIME_OPERATION(30000, findPerfects(30000));
    TIME_OPERATION(60000, findPerfects(60000));
    TIME_OPERATION(120000, findPerfects(120000));
}

STUDENT_TEST("my time trials of isPerfect on different input sizes")
{
    TIME_OPERATION(10, isPerfect(10));
    TIME_OPERATION(100, isPerfect(100));
    TIME_OPERATION(1000, isPerfect(1000));
    TIME_OPERATION(10000, isPerfect(10000));
}

STUDENT_TEST("my trials of isPerfect on some negative integers")
{
        EXPECT(!isPerfect(-1));
        EXPECT(!isPerfect(-10));
        EXPECT(!isPerfect(-100));
}

STUDENT_TEST("my trials of divisorSum vs smarterSum")
{
    EXPECT_EQUAL(divisorSum(-1), smarterSum(-1));
    EXPECT_EQUAL(divisorSum(6), smarterSum(6));
    EXPECT_EQUAL(divisorSum(90000), smarterSum(90000));
}

STUDENT_TEST("my time trials of findPerfectSmarter on doubleing input sizes")
{
    TIME_OPERATION(300000, findPerfectsSmarter(300000));
    TIME_OPERATION(600000, findPerfectsSmarter(600000));
    TIME_OPERATION(1200000, findPerfectsSmarter(1200000));
    TIME_OPERATION(2400000, findPerfectsSmarter(2400000));
}

STUDENT_TEST("my trials of findNthPerfectEuclid")
{
    for (int i=1; i<6; i++)
        EXPECT(isPerfect(findNthPerfectEuclid(i)));

    EXPECT_EQUAL(6, findNthPerfectEuclid(1));
    EXPECT_EQUAL(28, findNthPerfectEuclid(2));
    EXPECT_EQUAL(496, findNthPerfectEuclid(3));
    EXPECT_EQUAL(8128, findNthPerfectEuclid(4));
}

STUDENT_TEST("Test oddballs: 4 and 10 are not prime") {
    EXPECT(!isPrime(4));
    EXPECT(!isPrime(10));
}

PROVIDED_TEST("Test oddballs: 1 and 5 are prime") {
    EXPECT(!isPrime(1));
    EXPECT(isPrime(5));
}

