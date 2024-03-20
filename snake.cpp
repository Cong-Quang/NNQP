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
void print(const char *text, int x, int y, int color)
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
void RenderMenu(int vt, bool &l_Flag) // Gan vitri cho menu
{
    if (l_Flag == false)
    {
        return;
    }
    
    ClearConsole();
    for (int i = 0; i < 52; i++)
    {
        print(" ", i, 0, (i * 10) + 30);
        print(" ", i, 26, (i * 10) + 30);
    }
    switch (vt)
    {
        case 0:
        {
            print("<o> PLAY GAME   ", 18, 10, GREEN);
            print("      HELP     ", 20, 12, RED);
            print("    SETTING    ", 20, 14, RED);
            print("      EXIT     ", 20, 16, RED);
            l_Flag = false;
            break;
        }
        case 1:
        {
            print("   PLAY GAME   ", 20, 10, RED);
            print("<o>    HELP     ", 18, 12, GREEN);
            print("    SETTING    ", 20, 14, RED);
            print("      EXIT     ", 20, 16, RED);
            l_Flag = false;
            break;
        }
        case 2:
        {
            print("   PLAY GAME   ", 20, 10, RED);
            print("      HELP     ", 20, 12, RED);
            print("<o>  SETTING    ", 18, 14, GREEN);
            print("      EXIT     ", 20, 16, RED);
            l_Flag = false;
            break;
        }
        case 3:
        {
            print("   PLAY GAME   ", 20, 10, RED);
            print("      HELP     ", 20, 12, RED);
            print("    SETTING    ", 20, 14, RED);
            print("<o>    EXIT     ", 18, 16, GREEN);
            l_Flag = false;
            break;
        }
    default:
        break;
    }
}

void RenderMenusetting(int dk)
{ // Gán thanh độ khó setting.
    switch (dk)
    {
    case 0:
        print(" ", 10, 13, BLUE);
        break;
    case 1:
        for (int i = 0; i <= 16; i++)
        {
            print(" ", 10 + i, 13, BLUE);
        }
        break;
    case 2:
        for (int i = 0; i <= 32; i++)
        {
            print(" ", 10 + i, 13, BLUE);
        }
        break;
    }
}
void Manhinh()
{ // Code in menu
    for (int i = 0; i < 52; i++)
    {
        print(" ", i, 0, (i * 10) + 30);
        print(" ", i, 26, (i * 10) + 30);
    }
    print(" PLAY GAME ", 20, 10, GREEN);
    print("    HELP   ", 20, 12, RED);
    print("  SETTING  ", 20, 14, RED);
    print("    EXIT   ", 20, 16, RED);
}
// Màn hình help
void ManhinhHelp()
{
    ClearConsole();
    print("Dung 4 phim a w d s de dieu khien con ran", 6, 12, 2);
    print("Bam pham Enter de qua lai luc nay", 10, 14, 2);
    getchar();
}
// Màn hình setting (còn làm dở).
void Manhinhsetting()
{
    ClearConsole();
    for (int i = 0; i < 52; i++)
    {
        print(" ", i, 0, (i * 10) + 30);
        print(" ", i, 26, (i * 10) + 30);
    }
    print("EASY", 10, 11, 1);
    print("HARD", 38, 11, 3);
    print(" ", 9, 13, WHITE);
    print(" ", 43, 13, WHITE);
    for (int i = 0; i <= 32; i++)
    {
        print(" ", 10 + i, 12, WHITE);
        print(" ", 10 + i, 14, WHITE);
    }
    char input = readKey(); //
    int dk = 0;             //
    switch (input)          //
    {                       //
    case 'w':               //
        dk--;               // funtion chưa hoạt động.
        if (dk < 0)         //
            dk = 0;         //
        break;              //
    case 's':               //
        dk++;               //
        if (dk > 2)         //
            dk = 2;         //
        break;
    }
    RenderMenusetting(dk); // Lựa chọn lever chưa hoạt động.
    getchar();
}
static int vt = 0;

void MenuStart()
{
    // ClearConsole();
    // Manhinh(); -> del
    bool l_Flag = false;
    while (true)
    {
        char input = readKey();
        switch (input)
        {
            case 'w':
            {
                vt--;
                if (vt < 0)
                {
                    vt = 0;
                }
                l_Flag = true;
                break;
            }
            case 's':
            {
                vt++;
                if (vt > 3)
                {
                    vt = 3;
                }
                l_Flag = true;
                break;
            }
            // sử lý phím khi enter vào vt bất kỳ
            case ' ': // phím space
            {
                ClearConsole();
                print("Selected: ", 1, 1, WHITE);
                switch (vt)
                {
                    case 0:
                    {
                        print("PLAY GAME: ", 1, 2, WHITE); //  mốt thêm logic ở đâu (gọi 1 hàm khác vào các kiểu)
                        exit(0);
                    }
                    case 1:
                    {
                        system("cls");
                        ManhinhHelp(); // Lựa chọn màn hình Help.
                        break;
                    }
                    case 2:
                    {
                        system("cls");
                        Manhinhsetting(); // Lựa chọn màn hình setting.
                        break;
                    }
                    default:
                        break;
                    }
                break;
            }
            default:
                break;
        }
        RenderMenu(vt, l_Flag);
    }
}

int main()
{
    system("cls");
    system("title = Snake G");
    ClearConsole();
    Manhinh();
    MenuStart();
    return 0;
}
