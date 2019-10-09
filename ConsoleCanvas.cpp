#include <iostream>
#include <conio.h>
#include <windows.h>
#include "ConsoleCanvas.h"

ConsoleCanvas::ConsoleCanvas() {
    _width = 55, _height = 26, width = (_width - 7) / 2, height = _height - 2;
    int i, j;
    for (i = 0; i < _width; i++)
        for (j = 0; j < _height; j++)
            board[j][i] = ' ';
    for (i = 0; i < _height; i++) {
        _put("┃", _width - 7, i);
        board[i][_width - 5] = '\n';
        _put("\t\t", _width - 4, i);
        _put("┃", _width - 2, i);
    }
    for (j = 0; j < _width - 7; j += 2) {
        _put("︽", j, 0);
        _put("︽", j, _height - 1);
    }
    _put("┓", _width - 7, 0);
    _put("┗", _width - 2, _height - 2);
    _put("┛", _width - 7, _height - 1);
    board[_height - 1][_width - 2] = '\0';
}

void ConsoleCanvas::cls() {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++)
            _put("  ", i * 2, j + 1);
    }
}

void ConsoleCanvas::put(const string &s, int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height)return;
    board[y + 1][x * 2] = s[0];
    board[y + 1][x * 2 + 1] = s[1];
}

void ConsoleCanvas::_put(const string &s, int x, int y) {
    board[y][x] = s[0];
    board[y][x + 1] = s[1];
}

char ConsoleCanvas::get(int x, int y) {
    return board[y + 1][x * 2 + 1];
}

void ConsoleCanvas::note(const string &s, int wait) {
    system("color 02");
    cls();
    text(s, -1, _height / 2);
    show();
    Sleep(wait);
    while (kbhit()) getch();
}

void ConsoleCanvas::show() {
    system("cls");

    std::cout << "\n\t\t┏" << *board;
}

void ConsoleCanvas::text(const string &s, int x, int y) {
    int len = strLen(s);
    if (x == -1) x = (_width - len - 7) / 2;
    if (x > 0 && y >= 0) {
        if (len % 2 == 1) len--; //处理成偶数
        for (int i = 0; i < len; i++)
            board[y][i + x] = s[i];
    } else {
        x = 2, y = 5;
        int n = 0, _n = 0;
        while (n < len) {
            (s[n] >> 7 == -1) && _n++;
            if (s[n] == '\n') {
                n++, x = 2, y += 2;
                continue;
            }
            board[y][x++] = s[n];
            if (x >= _width - 7) {
                if (_n % 2) board[y][x - 1] = ' ', _n--, n--;
                x = 2, y += 2;
            }
            n++;
            if (!(_n % 2))show(), Sleep(15);
        }
    }
}

int ConsoleCanvas::strLen(const string &s) {
    return s.length();
}

