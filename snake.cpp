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

// Struct lưu vị trí
struct Point
{
    int x, y;
    Point(int _x = 0, int _y = 0) : x(_x), y(_y) {}
};

// Enum màu sắc
typedef enum
{
    BLACK = 30,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    WHITE = 37
} Color;

// Enum hướng di chuyển
enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// Hàm in chuỗi tại vị trí và màu sắc cho trước
void print(const char *text, int x, int y, int color = 37)
{
    char buffer[200];
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
    COORD pos = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(hConsole, pos);
    sprintf(buffer, "%s", text);
    printf("%s", buffer);
#else
    sprintf(buffer, "\033[%d;%dH\033[%dm%s\033[0m", x, y, color, text);
    printf("%s", buffer);
#endif
}

// Đọc phím
char readKey()
{
#ifdef _WIN32
    if (_kbhit())
    {
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

    if (read(STDIN_FILENO, &c, 1) != 1)
    {
        c = '\0';
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
#endif
}

// Dừng màn hình trong milliseconds
void pause(int milliseconds)
{
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

// Xóa màn hình
void ClearConsole()
{
#ifdef _WIN32
    COORD topLeft = {0, 0};
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
#else
    printf("\033[2J\033[1;1H"); 
#endif
}

// Hiển thị menu với màu sắc tương ứng với vị trí
void RenderMenu(int vt)
{
    switch (vt)
    {
    case 0:
        print("Quang ", 5, 5, RED);
        break;
    case 1:
        print("Quang ", 5, 5, GREEN);
        break;
    case 2:
        print("Quang ", 5, 5, GREEN);
        break;
    case 3:
        print("Quang ", 5, 5, BLUE);
        break;
    case 4:
        print("Quang ", 5, 5, YELLOW);
        break;
    default:
        break;
    }
}

static int vt = 0;
void MenuStart()
{
    char input = readKey();
    switch (input)
    {
    case 'w':
        vt--;
        if (vt < 0)
            vt = 0;
        break;
    case 's':
        vt++;
        if (vt > 4)
            vt = 4;
        break;
    default:
        break;
    }
    RenderMenu(vt);
}

int main()
{
    while (1)
    {
        MenuStart();
        pause(50);
    }
    return 0;
}