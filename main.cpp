#include "raylib.h"
#include "string"
#include "vector"

#define MAX_INPUT_CHARS 1024
bool wordWrap{false};
struct Commands
{
    void ping()
    {
    }

    void testCommand(std::string inputText, float x, float y)
    {
        int sizeOfPing{5};
        int numberOfCommands{2};
        bool foundCommand{false};
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

    char name[MAX_INPUT_CHARS + 1] = "\0"; // NOTE: One extra space required for null terminator char '\0'
    char printedLines[32][1024];
    int letterCount = 0;
    int keysPressed{0};
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
    int selIndex {0};

    Rectangle textBox = {10, 10, screenWidth - 20, screenHeight - 20};
    Rectangle OuterBox = {0, 0, screenWidth, screenHeight};
    Rectangle IRCBox = {10, 10, screenWidth - 40, screenHeight - 40};
    bool mouseOnText = false;

    int framesCounter = 0;

    SetTargetFPS(30); // Set our game to run at 10 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        //controles
        if (IsKeyDown(KEY_LEFT_ALT) && IsKeyDown(KEY_ENTER))
        {
            ToggleFullscreen();
        }
        // Update
        //----------------------------------------------------------------------------------
        if (!inIRC)
        {
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
            DrawRectangleRec(OuterBox, BLACK);
            DrawRectangleRec(textBox, LIGHTGRAY);
            if (mouseOnText)
                DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, RED);
            else
                DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, DARKGRAY);
            if (MeasureText(name, 40) <= GetScreenWidth() - 30)
            {
                overText = false;
            }
            else
            {
                overText = true;
                DrawText("Too many characters", screenHeight / 2, screenWidth / 2, 40, RED);
            }

            DrawText(name, 40, 670, 40, MAROON);
            if (IsKeyPressed(KEY_ENTER) && !clearScreen && !IsKeyDown(KEY_LEFT_ALT) && !overText)
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
            Commands command;
            for (int i = 0; i < index; i++)
            {
                y = 32;
                std::string userString = &printedLines[i][0];
                if (userString == clear) //If input is "clear", clear screen
                {
                    clearScreen = true;
                }
                else if (userString == enterIRC) //if input is enterIrc, draw IRC box
                {
                    inIRC = true;
                    printedLines[i][0] = '\0';
                    clearScreen = true;
                }
                else
                {
                    command.testCommand(userString, screenWidth / 64, y * (i + 1));
                }
            }

            //const char prevLines[] = {'A', 'B', 'C'}; //gwhen you call the array, it gives you everything up to the null term. i.e., prevLines[1] gives you b,c -not a
            if (mouseOnText)
            {
                if (letterCount < MAX_INPUT_CHARS)
                {
                    // Draw blinking underscore char
                    if (((framesCounter / 20) % 2) == 0)

                        DrawText(">", 20, 670, 40, MAROON);
                }
                else
                    DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);
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

            EndDrawing();
            //----------------------------------------------------------------------------------
        } else if (inIRC)
        {

            //irc variables
            bool selected {false}; 

            Rectangle TextBoxes[10] {0, 0, 0, 0};
            TextBoxes[0].height = 75;
            TextBoxes[0].width = 800; //relace with measure text
            TextBoxes[0].x = 20;
            TextBoxes[0].y = 20;
            Rectangle InputBox {20, 635, 800, 75};
            int sizeOfDecisionBox = 3;
            Rectangle DecisionBox[sizeOfDecisionBox] {0, 0, 0, 0};
            BeginDrawing();
            ClearBackground(GRAY);
            DrawRectangleRec(TextBoxes[0], RED);
            DrawRectangleRec(InputBox, BLUE);
            DecisionBox[0].x = 35;
            DecisionBox[0].y = 642;
            DecisionBox[0].width = 200;
            DecisionBox[0].height = 60;
            DecisionBox[1].x = 300;
            DecisionBox[1].y = 642;
            DecisionBox[1].width = 200;
            DecisionBox[1].height = 60;
            DecisionBox[2].x = 600;
            DecisionBox[2].y = 642;
            DecisionBox[2].width = 200;
            DecisionBox[2].height = 60;
            DrawRectangleRec(DecisionBox[0], GRAY);
            DrawRectangleRec(DecisionBox[1], GRAY);
            DrawRectangleRec(DecisionBox[2], GRAY);

            //select box
            if (IsKeyPressed(KEY_LEFT))
            {  
                if (selIndex >= 0 && selIndex <= 3) selIndex--;
            }

            if (IsKeyPressed(KEY_RIGHT))
            {
                if (selIndex >= 0 && selIndex <= 3) selIndex++;
            }

            DrawRectangleRec(DecisionBox[selIndex], WHITE);



            DrawRectangleLines(DecisionBox[selIndex].x, DecisionBox[selIndex].y, DecisionBox[selIndex].width, DecisionBox[selIndex].height, DARKGRAY);

            if (IsKeyDown(KEY_TAB))
            {
                inIRC = false;
            }

            

            EndDrawing();
        }
 
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
