#include "raylib.h"
#include <vector>
#include <string>

int main() 
{
    const int windowWidth {1280};
    const int windowHeight {720};
    const char title[50] = "Hello, world!";
    int i = 0;

    InitWindow(windowWidth, windowHeight, title);
    char name[100] = "\0";
    int letterCount {0};
    
    

    

    while (!WindowShouldClose())
    {
        BeginDrawing();
        DrawRectangle(10, 10, windowWidth-20, windowHeight-20, RED);
        if (GetKeyPressed() > 0)
        {
            int key = GetCharPressed();
            name[letterCount] = (char)key;
            name[letterCount+1] = '\0';
            DrawText(name/*std::to_string(key).c_str()*/, 20, 20, 50, BLUE);

        }

        DrawText(name, 20, 20, 50, BLUE);


        ClearBackground(BLACK);
        EndDrawing();

    }
    CloseWindow();

    return 0;
}