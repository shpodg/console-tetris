
#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_
using namespace std;
class ConsoleCanvas {
public:
    static int strLen(const string& s);

    char board[26][55];
    int width,height;

    ConsoleCanvas();

    void show();

    void cls();

    void text(const string& s, int x = -1, int y = 0);

    void put(const string& s, int x, int y);

    void _put(const string& s, int x, int y);

    char get(int x, int y);

    void note(const string& s, int wait = 2000);

private:
    int _width,_height;

};

#endif   //_BACKGROUND_H_ 
