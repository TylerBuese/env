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
    char printedLines[1024][32];
    int sizeOfLines {32};
    bool lines[sizeOfLines] {false};
    int letterCount = 0;
    int keysPressed {0};
    int index {0};
    int oldItems {0};
    int lineItem {0};


    Rectangle textBox = {10, 10, screenWidth - 20, screenHeight - 20};
    bool mouseOnText = false;

    int framesCounter = 0;

    SetTargetFPS(30); // Set our game to run at 10 frames-per-second

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
        if (IsKeyPressed(KEY_ENTER))
        {
            lines[index] = true;
            //adds all items in name to prevLine
            for (int i = 0; i < keysPressed; i++)
            {
                prevLine[i] += name[i];
                printedLines[i][0] += name[i]; 
            }
            printedLines[keysPressed + 1][0] = '\0';


            //reset terminal prompt
            for (int i = 0; i < keysPressed; i++)
            {
                name[i] = '\0';
            }
            oldItems = keysPressed;
            keysPressed = 0;
            letterCount = 0;
            
            // DrawText(&prevLine[0], posX, posY + 40, 40, MAROON);   
        }

        
        int j = 0;
        //DrawText(&prevLine[0], screenWidth / 64, posY, 40, MAROON);
        for (int i = 0; i < oldItems + 1; i++)
        {
            DrawText(&printedLines[i][0], screenWidth / 64 + j, posY, 40, MAROON);
            j += 25;
        }
                                 
        



        //DrawText(std::to_string(index).c_str(), screenWidth / 64, posY, 40, MAROON);
        
        

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


