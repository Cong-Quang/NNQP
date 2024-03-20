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
    // ẩn con trỏ nhấp nháy
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    //
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
    COORD pos = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(hConsole, pos);
    sprintf(buffer, "%s", text);
    printf("%s", buffer);
#else

    sprintf(buffer, "\033[%d;%dH\033[%dm%s\033[0m", x, y, color, text);
    printf("\e[?25l");
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

void RenderMenu(int vt)
{
    print("MENU", 5, 5, GREEN);
    print("1. Bắt đầu", 7, 7, vt == 0 ? BLUE : WHITE);
    print("2. Hướng dẫn", 7, 8, vt == 1 ? BLUE : WHITE);
    print("3. Thoát", 7, 9, vt == 2 ? BLUE : WHITE);
}

static int vt = 0;
void MenuStart()
{
    char key = readKey();
    switch (key)
    {
    case 'w':
        vt = (vt - 1 + 3) % 3;
        break;
    case 's':
        vt = (vt + 1) % 3;
        break;
    case '\n':
        // Xử lý sự kiện chọn
        switch (vt)
        {
        case 0:
            // Bắt đầu trò chơi
            break;
        case 1:
            // Hiển thị hướng dẫn
            break;
        case 2:
            // Thoát game
            return;
        }
        break;
    }
    RenderMenu(vt);
}

int main()
{
    while (true)
    {
        MenuStart();
        pause(100);
    }
    return 0;
}
