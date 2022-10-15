/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "gtypes.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/*
 * 输入参数是GPoint表示的三角形三个顶点以及int表示阶数order，
 * 返回值是int表示的绘制的黑色三角形总个数numOfTriangles。
 *
 * 这个函数将绘制一个由三个顶点和指定阶数定义的Sierpinski三角形，
 * 并记录绘制黑色三角形的总个数。
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    if (order == 0) {
        fillBlackTriangle(window, one, two, three);
        return 1;
    }

    //各边中点
    GPoint leftMidPoint((one.x + two.x)/2, (one.y + two.y)/2);
    GPoint rightMidPoint((one.x + three.x)/2, (one.y + three.y)/2);
    GPoint downMidPoint((three.x + two.x)/2, (three.y + two.y)/2);

    int numOfTriangles = 0;

    numOfTriangles += drawSierpinskiTriangle(window, one, leftMidPoint, rightMidPoint, order-1);
    numOfTriangles += drawSierpinskiTriangle(window, two, leftMidPoint, downMidPoint, order-1);
    numOfTriangles += drawSierpinskiTriangle(window, three, downMidPoint, rightMidPoint, order-1);

    return numOfTriangles;

}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

