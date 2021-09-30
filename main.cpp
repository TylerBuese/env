#include "raylib.h"
#include <vector>
#include <string>

void writeLetter(const char *letter, int posx, int posy, int fontSize);

int main() 
{
    const int windowWidth {1280};
    const int windowHeight {720};
    const char title[50] = "Hello, world!";
    int i = 0;

    InitWindow(windowWidth, windowHeight, title);
    char name[100] = "\0";
    int letterCount {0};
    int posX {20};
    int posY {20};

    while (!WindowShouldClose())
    {
        BeginDrawing();
        DrawRectangle(10, 10, windowWidth-20, windowHeight-20, RED);
        if (GetKeyPressed() > 0)
        {
            int key = GetCharPressed();
            name[letterCount] = (char)key;
            posX += 20;
            //posY += 20;
            if (posX >= windowWidth-20)
            {
                posY += 20;
                posX = 20;
            }

            
        }

         //DrawText(&name[letterCount], posX, posY, 50, BLUE);

         writeLetter(&name[letterCount], posX, posY, 50);
         




        ClearBackground(BLACK);
        EndDrawing();

    }
    CloseWindow();

    return 0;
}

void writeLetter(const char *letter, int posx, int posy, int fontSize) 
{
    DrawText(letter, posx, posy, fontSize, BLUE);
}
