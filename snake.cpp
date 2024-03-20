#include <stdio.h>
#include <time.h>
#include <vector>


#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <ncurses.h>
#include <termios.h>
#endif
// ========== varilaber =================

struct Point { // vi tri cho ran, moi, ..... 
    int x, y;
    Point(int _x = 0, int _y = 0) : x(_x), y(_y) {}
};

enum Direction { UP, DOWN, LEFT, RIGHT }; // huong di chuyen
std::vector<Point> snake; // Danh sách các điểm của rắn

//chiều  của console
int Width = 40; 
int Height = 20;

Direction dir; // Hướng di chuyển hiện tại của rắn

// ========= system =======================

// Enum màu sắc
typedef enum {
    BLACK = 30,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    WHITE = 37
} Color;

//in ra một chuỗi s ở vị trí x y và màu sắc dựa trên int ( 1 -15)
void print(const char* text,int x, int y ,int color = 37){ 
    char buffer[200];
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
    COORD pos = {static_cast<SHORT>(x) , static_cast<SHORT>(y)}; //chuyển đổi kiểu int sang short
    SetConsoleCursorPosition(hConsole, pos);
    sprintf(buffer, "%s", text);
    printf("%s", buffer);
#else
    sprintf(buffer, "\033[%d;%dH\033[%dm%s\033[0m", x, y, color, text);
    printf("%s", buffer);
#endif
}

char readKey() {// dùng để đọc bàn phím khi đc nhấn 
#ifdef _WIN32
    if (_kbhit()) {
        return _getch();
    }
    return '\0';
#else
    char c;
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    if (read(STDIN_FILENO, &c, 1) != 1) {
        c = '\0';
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
#endif
}

void update() {
    char input = readKey();
    // Đọc phím người chơi và thiết lập hướng di chuyển
    switch (input) {
        case 'w':
            if (dir != DOWN) // Không thể di chuyển ngược lại với hướng hiện tại
                dir = UP;
            break;
        case 's':
            if (dir != UP)
                dir = DOWN;
            break;
        case 'a':
            if (dir != RIGHT)
                dir = LEFT;
            break;
        case 'd':
            if (dir != LEFT)
                dir = RIGHT;
            break;
    }
}

// Hàm dừng màn hình trong milli giây 
void pause(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

//================== Handle ==========================

void ClearConsole() { // clear console thay cho cls
   for (int i = 0; i < Height + 10; i++)
   {
        for (int i = 0; i < Width + 10; i++)
        {
            printf(" ");
        }
        printf("\n");
   }
   
}
void MenuStart(){ // khi bat dau
    ClearConsole();
    for (int i = 0; i < 50; i++) // thay doi mau sac sau 1s
    {
        print(" ",5,5,i+30);
        pause(100);
    }
    
}
int main() {
    MenuStart();
    return 0;
}
