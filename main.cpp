#include "raylib.h"
#include "string"
#include "vector"
#include "irc.h"

#define MAX_INPUT_CHARS 1024
bool wordWrap{false};
struct Commands
{
    void testCommand(std::string inputText, float x, float y)
    {
        int numberOfCommands{2};
        const char commands[50][1000]{
            "ping",
            ""};
        std::string pingString = &commands[0][0]; //ping

        for (int i = 0; i < numberOfCommands; i++) //loops through all commands, if it finds one, executes
        {
            /*start commands*/
            std::string ping = &commands[0][0]; //ping command
            /*end commands*/
            if (inputText == ping)
            {
                DrawText("Pinging...", x, y, 30, MAROON);
            }
            else
            {
                std::string errorText = "The command \"" + inputText + "\" was not found. Please check capitalization/spelling and try again.";
                std::string copyErrorText = errorText;
                int j = errorText.length();
                int text = MeasureText(errorText.c_str(), 20);
                if (text > GetScreenWidth() - 20)
                {
                    std::string tempText = errorText;
                    do
                    {
                        if (MeasureText(errorText.c_str(), 20) > GetScreenWidth() - 30)
                        {
                            tempText[j - 1] = '\0';
                            errorText = tempText;
                            j -= 1;
                        }
                        else
                        {
                            break;
                        }
                    } while (true);
                }
                DrawText(&copyErrorText[j], x, y + 20, 20, MAROON);
                DrawText(errorText.c_str(), x, y, 20, MAROON);
                wordWrap = false;
            }
        }
    }
};

struct Upgrades
{
};

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "$env:");

    char cmd[MAX_INPUT_CHARS + 1] = "\0"; // NOTE: One extra space required for null terminator char '\0'
    char irc[MAX_INPUT_CHARS + 1] = "\0";
    char printedLines[32][1024];
    char ircLines[100][1024];
    int letterCount = 0;
    int ircLetterCount {0};
    int keysPressed{0};
    int ircKeysPressed {0};
    int index{0};
    int x = 0;
    int y = 0;
    char test[5] = "test";
    //screen altering functions
    const char clear[6] = "clear";
    const char enterIRC[4] = "irc";
    bool clearScreen{false};
    bool inIRC{false};
    bool overText{false};

    Rectangle textBox = {10, 10, screenWidth - 20, screenHeight - 20};
    Rectangle OuterBox = {0, 0, screenWidth, screenHeight};
    Rectangle IRCBox = {10, 10, screenWidth / 2, screenHeight - 20};
    bool mouseOnText = false;

    int framesCounter = 0;

    SetTargetFPS(30); // Set our game to run at 10 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        if (IsKeyDown(KEY_LEFT_ALT) && IsKeyDown(KEY_ENTER))
        {
            ToggleFullscreen();
        }
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
                cmd[letterCount] = (char)key;
                cmd[letterCount + 1] = '\0'; // Add null terminator at the end of the string.
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
            cmd[letterCount] = '\0';
        }

        if (IsKeyPressed(KEY_BACKSPACE) && IsKeyDown(KEY_LEFT_CONTROL))
        {

            for (int i = 0; i < letterCount; i++)
            {
                letterCount -= letterCount;
                keysPressed -= keysPressed;
                if (letterCount < 0)
                    letterCount = 0;
                cmd[0] = '\0';
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

        if (!inIRC)
        {
            //put al non-irc stuff in here
        }

        ClearBackground(RAYWHITE);
        DrawRectangleRec(OuterBox, BLACK);
        DrawRectangleRec(textBox, LIGHTGRAY);
        if (mouseOnText)
            DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, RED);
        else
            DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, DARKGRAY);
        if (MeasureText(cmd, 40) <= GetScreenWidth() - 30)
        {
            overText = false;
        }
        else
        {
            overText = true;
            DrawText("Too many characters", screenHeight / 2, screenWidth / 2, 40, RED);
        }

        if (clearScreen)
        {
            for (int i = 0; i < index; i++)
            {
                std::string tempLine = &printedLines[i][0];
                int numOfCharacters = tempLine.length();
                for (int j = 0; i < numOfCharacters; j++)
                {
                    printedLines[i][j] = '\0';
                }
            }
            clearScreen = false;
            index = 0;
        }

        if (inIRC)
        {
            DrawRectangleRec(IRCBox, GRAY);
            if (((framesCounter / 20) % 2) == 0)
                DrawText(">", 20, 670, 40, MAROON);

            DrawText(irc, 40, 670, 40, MAROON);
            
            int ircKey = GetKeyPressed();
            while (ircKey > 0)
            {
                // NOTE: Only allow keys in range [32..125]
                if ((ircKey >= 32) && (ircKey <= 125) && (ircLetterCount < MAX_INPUT_CHARS))
                {
                    irc[ircLetterCount] = (char)ircKey;
                    irc[ircLetterCount + 1] = '\0'; // Add null terminator at the end of the string.
                    ircLetterCount++;
                    ircKeysPressed++;
                }
                ircKey = GetCharPressed(); // Check next character in the queue
            }


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
