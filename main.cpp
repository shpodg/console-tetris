/*
  name : Tetris.cpp
  author :Smile @ Life
  time    :2013-01-12
*/
#include<iostream>
//#include<fstream>
#include<cstdlib> //rand()
#include<windows.h>//Sleep()
#include<conio.h>//kbhit()
#include<ctime>
#include"ConsoleCanvas.h"

#define N 7
#define infow 7
//std::ofstream file("log.txt");

using namespace std;
/**
 * 所有形状
 */
const int TETRIS_SHAPES[7][4][2] = {
        {-1, -1, 0,  -1, -1, 0, 0,  0},//o
        {0,  -2, 0,  -1, 0,  0, 0,  1},//1
        {0,  -1, 0,  0,  0,  1, 1,  1},//L2
        {-1, -1, 0,  -1, 0,  0, 1,  0},//Z
        {0,  -1, 0,  0,  0,  1, 1,  0},//T
        {0,  -1, 0,  0,  0,  1, -1, 1},//{-1,-1,-1,0,-1,1,0,1},//L
        {-1, -1, -1, 0,  0,  0, 0,  1}//s
};

int score;

ConsoleCanvas canvas;


int getKey(bool block = false);

void about();

void help();

class Shape {
public:
    friend void drawShadow();

    string style;
    int x, y;

    explicit Shape(const string &s = "■");

    void rotate();

    void cls();

    void draw();

    bool move(int direction);

    bool movable(int direction);

private:
    int n, loc[4][2]{};
};

Shape *currentShape, *nextShape, shadow("□");

void drawShadow() {
    shadow.x = currentShape->x, shadow.y = currentShape->y;
    for (int i = 0; i < 4; i++)
        shadow.loc[i][0] = currentShape->loc[i][0], shadow.loc[i][1] = currentShape->loc[i][1];
    while (shadow.movable(1))shadow.y++;
    shadow.draw();
}

Shape::Shape(const string &s) {
    int i;
    style = s;
    n = rand() % N;
    x = canvas.width - infow + 3, y = 4;
    for (i = 0; i < 4; i++)
        loc[i][0] = TETRIS_SHAPES[n][i][0],
                loc[i][1] = TETRIS_SHAPES[n][i][1];
}

/**
 * 旋转
 */
void Shape::rotate() {
    if (n == 0)return;
    int i, t = x, a[4][2], b[4][2];
    for (i = 0; i < 4; i++)
        b[i][0] = loc[i][0], b[i][1] = loc[i][1],
        a[i][0] = -loc[i][1], a[i][1] = loc[i][0];
    for (i = 0; i < 4; i++) {
        if (x + a[i][0] < 0)
            x = -a[i][0];
        else if (x + a[i][0] >= (canvas.width - infow))
            x = (canvas.width - infow) - a[i][0] - 1;
    }
    for (i = 0; i < 4; i++)
        loc[i][0] = a[i][0], loc[i][1] = a[i][1];
    if (!movable(0)) {
        x = t;
        for (i = 0; i < 4; i++)
            loc[i][0] = b[i][0], loc[i][1] = b[i][1];
    }
}

void Shape::cls() {
    for (auto &i : loc)
        canvas.put("  ", x + i[0], y + i[1]);
}

void Shape::draw() {
    for (int i = 0; i < 4; i++)
        canvas.put(style, x + loc[i][0], y + loc[i][1]);
}

bool Shape::movable(int direction) {
    int a, b, t[2];
    if (direction == 0)a = b = 0;
    else if (direction == 1)a = 0, b = 1;
    else if (direction == 2)a = -1, b = 0;
    else if (direction == 3)a = 1, b = 0;
    for (int i = 0; i < 4; i++) {
        t[0] = (x + loc[i][0] + a), t[1] = (y + loc[i][1] + b);
        if (t[0] < 0 || t[0] >= (canvas.width - infow) || t[1] >= canvas.height || canvas.get(t[0], t[1]) == "■"[1])
            return false;
    }
    return true;
}

bool Shape::move(int direction) {
    if (direction == 0) {
        rotate();
        shadow.cls();
        drawShadow();
    }//旋转
    else if (direction == 1) {
        while (movable(1))y++;
        return true;
    } else if (direction == 2 && movable(2)) {
        while (true) {
            currentShape->cls();
            if (!movable(2)) break;
            x--;
            shadow.cls();
            drawShadow();
            currentShape->draw();
            canvas.show();
            if (getKey() != 2)break;
            Sleep(80);
        }
        currentShape->cls();
    } else if (direction == 3 && movable(3)) {
        while (true) {
            currentShape->cls();
            if (!movable(3)) break;
            x++;
            shadow.cls();
            drawShadow();
            currentShape->draw();
            canvas.show();
            if (getKey() != 3)break;
            Sleep(80);
        }
        currentShape->cls();
    } else if (direction == 13) {
        ConsoleCanvas pause;
        while (kbhit())getch();
        pause.text("按任意键继续！", -1, pause.height / 2);
        pause.show();
        getKey(true);
    }
    if (!movable(1))
        return true;
    return false;
}

bool blankRow(int i) {
    for (int j = (canvas.width - infow - 1); j >= 0; j--)
        if (canvas.get(j, i) != ' ')return false;
    return true;
}

void gone(int i) {
    int j;
    for (; i > 0; i--) {
        for (j = (canvas.width - infow - 1); j >= 0; j--)
            canvas.put("  ", j, i),
                    canvas.board[i + 1][j * 2] = canvas.board[i][j * 2],
                    canvas.board[i + 1][j * 2 + 1] = canvas.board[i][j * 2 + 1];
        if (blankRow(i))break;
    }
}

//检查有方块行，返回消去的行数。
int check() {
    int i, j, n = 0;
    for (i = canvas.height - 1; i >= 0; i--) {
        if (blankRow(i))break;
        for (j = (canvas.width - infow - 1); j >= 0; j--)
            if (canvas.get(j, i) == ' ')break;
        if (j == -1)gone(i), n++;
    }
    return n;
}

void over() {
    delete currentShape, delete nextShape;
    canvas.text("               ", -1, canvas.height / 2 - 1);
    canvas.text(" Game Over!    ", -1, canvas.height / 2);
    canvas.text("               ", -1, canvas.height / 2 + 1);
    canvas.show();
    Sleep(1000);
    system("pause");
}

void refreshInfo() {
    for (int x = canvas.width - infow + 2; x < canvas.width; x++)
        for (int y = 2; y < 2 + 4; y++)
            canvas.put("  ", x, y);
    delete currentShape;//更新方块
    currentShape = nextShape;
    currentShape->x = (canvas.width - infow) / 2, currentShape->y = -1;
    nextShape = new Shape();
    nextShape->draw();
    for (int i = rand() % 4; i > 0; i--)
        nextShape->rotate();
    canvas.board[10][50 - 7 + 3] = '0';
    for (int i = 3, n = score; n > 0; i--)//刷新分数
    {
        canvas.board[10][50 - 7 + i] = 48 + n % 10, n /= 10;
    }
}

void init() {
    canvas.cls();
    for (int i = 0; i <= canvas.height; i++)
        canvas.put("│", canvas.width - infow, i);
    canvas.text("下一个：", 50 - 13, 1);
    canvas.put("├", canvas.width - infow, 7);
    for (int x = canvas.width - infow + 1; x < canvas.width; x++)
        canvas.put("≒", x, 7);
    canvas.text("分数：", 50 - 13, 9);
    srand(time(0));
    score = 0;
    currentShape = new Shape();
    nextShape = new Shape();
    refreshInfo();
    drawShadow();
}

void runGame() {
    init();
    while (true) {
        currentShape->cls();
        if (currentShape->move(getKey())) {
            currentShape->draw();
            switch (check()) {
                case 0:
                    refreshInfo();
                    drawShadow();
                    if (!currentShape->movable(1)) {
                        currentShape->draw();
                        canvas.show();
                        over();
                        return;
                    }
                    break;
                case 1:
                    score += 10;
                    break;
                case 2:
                    score += 20;
                    break;
                case 3:
                    score += 40;
                    break;
                case 4:
                    score += 60;
                    break;
            }
        }
        currentShape->y++;
        currentShape->draw();
        canvas.show();
        Sleep(360);
    }
}

int main() {
    int index = 0;
    string s[7] = {"开始游戏", "帮助", "关于", "退出游戏", "", "", "使用上下左右和回车键！"};
    while (true) {
        canvas.cls();
        canvas.text("俄 罗 斯 方 块", -1, 5);
        canvas.text("≈≈≈≈≈≈≈", -1, 6);
        for (int i = 0; i < 7; i++) {
            if (i == index)
                canvas.put("->", (canvas.width - ConsoleCanvas::strLen(s[i]) / 2) / 2 - 2, 9 + i * 2);
            canvas.text(s[i], -1, 10 + i * 2);
        }
        canvas.show();
        switch (getKey(true)) {
            case 0:
                index = (index + 3) % 4;
                break;
            case 1:
                index = (index + 1) % 4;
                break;
            case 13:
                switch (index) {
                    case 0:
                        runGame();
                        break;
                    case 1:
                        help();
                        break;
                    case 2:
                        about();
                        break;
                    case 3:
                    default:
                        canvas.note("再见！", 1000);
                        return 0;
                }
        }
    }
    return 0;
    //file.close();
}

/**
 * 读取按键
 * @param block 是否阻塞
 * @return 返回方向
 */
int getKey(bool block) {
    static int k = -1;
    if (block || kbhit()) {
        k = getch();
        if (k == 224) {
            k = getch();
            if (k == 72)k = 0;
            else if (k == 80)k = 1;
            else if (k == 75)k = 2;
            else if (k == 77)k = 3;
        } else if (k == 119)k = 0;
        else if (k == 115)k = 1;
        else if (k == 97)k = 2;
        else if (k == 100)k = 3;
        return k;
    }
    return -1;
}

void help() {
    canvas.cls();
    canvas.text("帮助", -1, 2);
    canvas.text("菜单界面:\n  上下滚动，回车键确定\n游戏界面:\n  上：旋转，下：快速下落，回车键：暂停游戏\n其它:\n  建议调整好屏幕后再进行游戏！");
    canvas.show();
    system("pause");
}

void about() {
    string s[] = {
            " （0 0）",
            "+-----oOO----（_）----------+ ",
            "|                           | ",
            "| https://github.com/shpodg | ",
            "|                           | ",
            "+-------------------oOO-----+ ",
            " |__|__|",
            " || ||",
            " ooO Ooo"
    };
    canvas.cls();
    canvas.text("关于", -1, 2);
    for (int i = 0; i < 9; i++) {
        canvas.text(s[i], -1, i + 5);
    }
    canvas.show();
    system("pause");
}
