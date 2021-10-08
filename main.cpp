#include "raylib.h"
#include "string"
#include "vector"

#define MAX_INPUT_CHARS 1024

struct commands
{
    void testPing(char text)
    {
        int sizeOfText{5};
        const char ping[sizeOfText] = "Ping";
        for (int i = 0; i > sizeOfText; i++)
        {
        }
    }
};

struct upgrades
{
    
};

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "$env:");
    if (IsKeyPressed(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER))
    {
        ToggleFullscreen();
    }
        

    char name[MAX_INPUT_CHARS + 1] = "\0"; // NOTE: One extra space required for null terminator char '\0'
    char printedLines[32][1024];
    int letterCount = 0;
    int keysPressed{0};
    int index{0};
    int x = 0;
    int y = 0;
    char test[5] = "test";

    Rectangle textBox = {10, 10, screenWidth - 20, screenHeight - 20};
    bool mouseOnText = false;

    int framesCounter = 0;

    SetTargetFPS(30); // Set our game to run at 10 frames-per-second
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

        if (IsKeyPressed(KEY_BACKSPACE) && IsKeyDown(KEY_LEFT_CONTROL))
        {
            
            for (int i = 0; i < letterCount; i++)
            {
                letterCount -= letterCount;
                keysPressed -= keysPressed;
                if (letterCount < 0)
                    letterCount = 0;
                name[0] = '\0';
            }
            
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
            //adds all items in name to prevLine
            for (int i = 0; i < keysPressed; i++)
            {
                printedLines[index][i] += name[i];
            }

            //reset terminal prompt
            for (int i = 0; i < keysPressed; i++)
            {
                name[i] = '\0';
            }
            keysPressed = 0;
            letterCount = 0;
            index++;
        }
        std::string testString = &test[0];
        for (int i = 0; i < index; i++)
        {
            y = 32;
            DrawText(&printedLines[i][0], screenWidth / 64, y * (i + 1), 40, MAROON);
            std::string userString = &printedLines[i][0];
            if (userString == testString)
            {
                DrawText("Hello to you too!", 600 + x, y, 40, MAROON);
            }
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
