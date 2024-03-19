#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
// ========== varilaber =================

//chiều  của console
int Width = 40; 
int Height = 20;
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
    COORD pos = { x, y };
    SetConsoleCursorPosition(pos,hConsole);
    sprintf(buffer, "%s", text);
    printf("%s", buffer);
#else
    sprintf(buffer, "\033[%d;%dH\033[%dm%s\033[0m", x, y, color, text);
    printf("%s", buffer);
#endif
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
        print("Game Snake\n",Height/2,10,i+30);
        pause(1000);
    }
    
}
int main() {
    
    MenuStart();
    return 0;
}
