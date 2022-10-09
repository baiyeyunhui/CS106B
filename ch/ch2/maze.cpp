// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* 输入参数为Grid<bool>表示的迷宫maze和当前所在位置cur，
 * 返回值为Set<GridLocation>表示的当前位置可到达的所有相邻位置
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    //当前位置不在迷宫所在的区域内或者在墙壁上
    if (!maze.inBounds(cur) || !maze.get(cur))
        error("PLEASE INPUT YOU CURRENT POSITION AGAIN!");
    //每次只能从当前位置cur移动一步，并且只能沿东西南北(East, West, South, North)这4个方向进行移动。
    GridLocation east = GridLocation(cur.row-1, cur.col);
    GridLocation west = GridLocation(cur.row+1, cur.col);
    GridLocation south = GridLocation(cur.row, cur.col-1);
    GridLocation north = GridLocation(cur.row, cur.col+1);
    //只能在迷宫所在的区域内移动，不能越界; 并且只能沿着通道移动，不能穿过墙壁。
    if (maze.inBounds(east) && maze.get(east))
        neighbors.add(east);
    if (maze.inBounds(west) && maze.get(west))
        neighbors.add(west);
    if (maze.inBounds(south) && maze.get(south))
        neighbors.add(south);
    if (maze.inBounds(north) && maze.get(north))
        neighbors.add(north);
    //没有可移动的方向，报错
    if (neighbors.isEmpty())
        error("NO WAY TO GO!");

    return neighbors;
}

/* 输入参数为Grid<bool>表示的迷宫maze和Stack<GridLocation>表示的路径path。
 * 这段程序是用来验证所给路径可以顺利走出迷宫
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    //1.如果路径的终点不位于迷宫右下角，报错
    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }
    //从之前位置移动到现在位置时，每一步都必须是符合规则的移动。
    Set<GridLocation> recordPath = {path.peek(), };
    Set<GridLocation> validMoves = generateValidMoves(maze, path.peek());
    GridLocation prePosition;
    path.pop();

    while (!path.isEmpty()) {
        prePosition = path.pop();
        //3.判断当前位置是否已经走过
        if (recordPath.contains(prePosition)) {
             error("Path arrived at the position of maze - " + prePosition.toString() + "repeatedly");
        }
        else {
            recordPath.add(prePosition);
        }
        //4.利用上面的generateValidMoves函数来判断之前移动是否符合规则。
        if (!validMoves.contains(prePosition))
            error("Path does not arrived at the position of maze - " + prePosition.toString());
        validMoves = generateValidMoves(maze, prePosition);
    }
    //2.路径的起点不位于迷宫左上角，报错。
    GridLocation mazeEntrance = {0, 0};
    if (prePosition != mazeEntrance)
         error("Path does not start from maze entrance");
}

/* 输入参数为Grid<bool>表示的迷宫maze，输出为Stack<GridLocation>表示的正确路径path。
 * 这个函数用来找出一条迷宫的正确路径
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;
    Stack<GridLocation> mazeEntrance = {{0, 0}};
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    //创建一个空的队列，用于添加路径
    Queue<Stack<GridLocation>> pathList;
    //创建一个仅包含入口的路径，并添加到队列中
    Stack<GridLocation> first = mazeEntrance;
    pathList.enqueue(first);

    while (!pathList.isEmpty()) {
        //从队列中取出当前路径
        path = pathList.dequeue();
        //void highlightPath(Stack<GridLocation> p, std::string color, int msecsToPause = 0);
        MazeGraphics::highlightPath(path, "red", 0.2);

        //如果当前路径已经到达出口，当前路径就是最短路径，搜索结束。
        GridLocation curPos = path.peek();
        if (curPos == mazeExit)
            return path;
        //根据当前路径的末端，找出所有可以继续前进的位置。
        Set<GridLocation> validMoves = generateValidMoves(maze, curPos);
        //创建一份当前路径的副本
        Stack<GridLocation> copyPath = path;
        //当前路径已经走过的点位
        Set<GridLocation> prePos;
        while(!copyPath.isEmpty()) {
            prePos.add(copyPath.pop());
        }
        for (const GridLocation& elem : validMoves) {
            //不能包括之前走过的点位
            if (!prePos.contains(elem)) {
                //每次从当前路径开始添加
                copyPath.clear();
                copyPath = path;
                //对于每一个可行的位置，将该位置添加到副本中
                copyPath.push(elem);
                //将新生成的副本添加到队列，以便后续继续搜索。
                pathList.enqueue(copyPath);
            }
        }
    }

    return {};
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */
PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

//// TODO: add your test cases here
STUDENT_TEST("generateValidMoves on location in the center of 3x3 grid with walls") {
    Grid<bool> maze = {{true, true, true},
                       {false, true, false},
                       {true, false, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1},};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

STUDENT_TEST("generateValidMoves on location on the walls of 3x3 grid") {
    Grid<bool> maze = {{true, true, true},
                       {true, false, true},
                       {true, true, true}};
    GridLocation pos = {1, 1};

    EXPECT_ERROR(generateValidMoves(maze, pos));
}

STUDENT_TEST("generateValidMoves outsider of 2x2 grid with no walls") {
    Grid<bool> maze = {{true, true},
                       {true, true}};
    GridLocation outsider = {1, 2};

    EXPECT_ERROR(generateValidMoves(maze, outsider));
}

STUDENT_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {false, true}};
    GridLocation corner = {1, 1};

    EXPECT_ERROR(generateValidMoves(maze, corner));
}

STUDENT_TEST("no generateValidMoves of 1X1 grid with no walls") {
    Grid<bool> maze = {{true},};
    GridLocation pos = {0, 0};

    EXPECT_ERROR(generateValidMoves(maze, pos));
}

STUDENT_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, true, true},
                       {true, false, true},
                       {false, true, true}};
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2} };

    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
}

STUDENT_TEST("solveMaze on file 2x2") {
    Grid<bool> maze;
    readMazeFile("res/2x2.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 25x15") {
    Grid<bool> maze;
    readMazeFile("res/25x15.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 13x39") {
    Grid<bool> maze;
    readMazeFile("res/13x39.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 33x41") {
    Grid<bool> maze;
    readMazeFile("res/33x41.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
