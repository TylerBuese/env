// #include "raylib.h"
// #include <vector>
// #include <string>

// void writeLetter(const char *letter, float posx, float posy, int fontSize);

// struct Letter
// {
//     float lposX[1000]{};
//     float lposY[1000]{};
//     char letters[1000]{"\0"};
// };
// int main()
// {
//     const int windowWidth{1280};
//     const int windowHeight{720};
//     const char title[50] = "Hello, world!";
//     int i = 0;

//     InitWindow(windowWidth, windowHeight, title);
//     std::vector<char> items;
//     int letterCount{0};
//     int posX{20};
//     int posY{20};

//     while (!WindowShouldClose())
//     {
//         BeginDrawing();
//         DrawRectangle(10, 10, windowWidth - 20, windowHeight - 20, RED);
//         Letter letter;
//         if (GetKeyPressed() > 0)
//         {
//             int key = GetCharPressed();
//             letterCount++;
//             items.push_back((char)key);
//             if (posX >= windowWidth - 20)
//             {
//                 posY += 20;
//                 posX = 20;
//             }

//             letter.letters[letterCount] = (char)key;
//         }

//         DrawText(std::to_string(letterCount).c_str(), 500, 500, 50, BLUE);
//         DrawText(&items[0], 600, 500, 50, BLUE);
//         for (int i = 0; i < items.size(); i++)
//         {
//             posX = 20;
//             posY = 20;
//             writeLetter(&items[0], posX, posY, 50);
//             posX += 20;
//         }
//         //writeLetter(&items[0], posX, posY, 50);

//         //DrawText(&name[letterCount], posX, posY, 50, BLUE);

//         //writeLetter(&name[letterCount], posX, posY, 50);

//         ClearBackground(BLACK);
//         EndDrawing();
//     }
//     CloseWindow();

//     return 0;
// }

// void writeLetter(const char *letter, float posx, float posy, int fontSize)
// {
//     DrawText(letter, posx, posy, fontSize, BLUE);
// }

/*******************************************************************************************
*
*   raylib [text] example - Input Box
*
*   This example has been created using raylib 3.5 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2017 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "string"
#include "vector"

#define MAX_INPUT_CHARS 1024

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "raylib [text] example - input box");

    char name[MAX_INPUT_CHARS + 1] = "\0"; // NOTE: One extra space required for null terminator char '\0'
    char prevLine[1024] {*name};
    int letterCount = 0;
    int keysPressed {0};

    Rectangle textBox = {10, 10, screenWidth - 20, screenHeight - 20};
    bool mouseOnText = false;

    int framesCounter = 0;

    SetTargetFPS(10); // Set our game to run at 10 frames-per-second

    float posX {20};
    float posY {20};
    bool lineOne {false};
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (CheckCollisionPointRec(GetMousePosition(), textBox))
            mouseOnText = true;
        else
            mouseOnText = false;

        if (mouseOnText)
        {
            // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
        }
        else
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS))
            {
                name[letterCount] = (char)key;
                name[letterCount + 1] = '\0'; // Add null terminator at the end of the string.
                letterCount++;
                keysPressed++;


                // if (keysPressed > 10)
                // {
                //     const char prevLine {*name};
                // }
            }

            key = GetCharPressed(); // Check next character in the queue
        }

        // Get char pressed (unicode character) on the queue

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            letterCount--;
            keysPressed--;
            if (letterCount < 0)
                letterCount = 0;
            name[letterCount] = '\0';
        }

        if (mouseOnText)
            framesCounter++;
        else
            framesCounter = 0;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawRectangleRec(textBox, LIGHTGRAY);
        if (mouseOnText)
            DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, RED);
        else
            DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, DARKGRAY);
        
        DrawText(name, 40, 670, 40, MAROON);
        if (IsKeyDown(KEY_ENTER))
        {
            lineOne = true;
            for (int i = 0; i < keysPressed; i++)
            {
                prevLine[i] += name[i];
            }

            for (int i = 0; i < keysPressed; i++)
            {
                name[i] = '\0';
            }

            keysPressed = 0;
            // DrawText(&prevLine[0], posX, posY + 40, 40, MAROON);   
        }

        if (lineOne)
        {
            DrawText(prevLine, screenWidth / 64, posY, 40, MAROON);
        }
        
        

        //const char prevLines[] = {'A', 'B', 'C'}; //gwhen you call the array, it gives you everything up to the null term. i.e., prevLines[1] gives you b,c -not a
        if (mouseOnText)
        {
            if (letterCount < MAX_INPUT_CHARS)
            {
                // Draw blinking underscore char
                if (((framesCounter / 20) % 2) == 0)
                    // DrawText("_", textBox.x + 8 + MeasureText(name, 40), textBox.y + 12, 40, MAROON);
                    DrawText(">", 20, 670, 40, MAROON);
            }
            else
                DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Check if any key is pressed
// NOTE: We limit keys check to keys between 32 (KEY_SPACE) and 126
bool IsAnyKeyPressed()
{
    bool keyPressed = false;
    int key = GetKeyPressed();

    if ((key >= 32) && (key <= 126))
        keyPressed = true;

    return keyPressed;
}
