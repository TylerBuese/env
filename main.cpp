#include "raylib.h"
#include "string"
#include "vector"

#define MAX_INPUT_CHARS 1024
bool wordWrap{false};
std::string IrcMessage(int Message);
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

    //irc variables
    int messageCount {0};
    int selIndex{0};
    std::string npcMessage[50];
    std::string userMessage[50];
    std::string npcMessageText[50];
    std::string userMessageText[50];
    bool displayMessage[50]{false};
    bool level[10]{false};
    int currentMessageNum {0}; //
    int userMessageNum {0};


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

            if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0)
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
                    for (int j = 0; j == 3; j++)
                    {
                        printedLines[i][j] = '\0';
                    }
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
        }
        else if (inIRC)
        {

            //irc variables
            int npcIrcX{20};
            int npcIrcY{20};
            int userIrcX{500};
            int userIrcY{50};

            Rectangle npcTextBoxes[50]{0, 0, 0, 0};
            Rectangle userTextBoxes[50]{0, 0, 0, 0};
            Rectangle InputBox{20, 635, 1240, 75};
            int sizeOfDecisionBox = 3;
            Rectangle DecisionBox[sizeOfDecisionBox]{0, 0, 0, 0};
            BeginDrawing();
            ClearBackground(GRAY);
            DrawRectangleRec(InputBox, BLUE);
            int decBoxWidth {375};
            DecisionBox[0].x = 50;
            DecisionBox[0].y = 642;
            DecisionBox[0].width = decBoxWidth;
            DecisionBox[0].height = 60;
            DecisionBox[1].x = 450;
            DecisionBox[1].y = 642;
            DecisionBox[1].width = decBoxWidth;
            DecisionBox[1].height = 60;
            DecisionBox[2].x = 850;
            DecisionBox[2].y = 642;
            DecisionBox[2].width = decBoxWidth;
            DecisionBox[2].height = 60;
            DrawRectangleRec(DecisionBox[0], GRAY);
            DrawRectangleRec(DecisionBox[1], GRAY);
            DrawRectangleRec(DecisionBox[2], GRAY);

            //select box
            int npcMessageNum {0};
            
            std::string npcMessage = IrcMessage(2000);

            //choose user message
            
            std::string userMessage = IrcMessage(2001 + currentMessageNum);
            userMessageText[messageCount] = userMessage;
            npcTextBoxes[messageCount].width = MeasureText(npcMessage.c_str(), 20) + 25;
            npcMessageText[messageCount] = npcMessage;

            if (IsKeyPressed(KEY_LEFT))
            {
                if (selIndex >= 0 && selIndex <= 2)
                {
                    selIndex--; 
                    currentMessageNum = selIndex + userMessageNum;
                }
                        
            }

            if (IsKeyPressed(KEY_RIGHT))
            {
                if (selIndex >= 0 && selIndex <= 2)
                {
                    selIndex++;
                    currentMessageNum = selIndex + userMessageNum;
                }
                    
            }

            DrawText(std::to_string(currentMessageNum).c_str(), 600, 600, 20, WHITE);

            if (IsKeyPressed(KEY_ENTER))
            {
                //submit option
                selIndex = 0;
                messageCount++;

            }

            if (selIndex >= 2)
            {
                selIndex = 2;
                currentMessageNum = selIndex + userMessageNum;
            }

            else if (selIndex <= 0)
            {
                selIndex = 0;
                currentMessageNum = selIndex + userMessageNum;
            }

            DrawRectangleRec(DecisionBox[selIndex], DARKGRAY);
            DrawRectangleLines(DecisionBox[selIndex].x, DecisionBox[selIndex].y, DecisionBox[selIndex].width, DecisionBox[selIndex].height, DARKGRAY);

            if (IsKeyDown(KEY_TAB))
            {
                inIRC = false;
            }

            //print npc text boxes
            for (int i = 0; i <= messageCount; i++)
            {
                npcTextBoxes[i].height = 30;
                npcTextBoxes[i].width = MeasureText(npcMessage.c_str(), 20) + 25;
                npcTextBoxes[i].x = npcIrcX;
                npcTextBoxes[i].y = npcIrcY;
                DrawRectangleRec(npcTextBoxes[i], RED);
                DrawText(npcMessage.c_str(), npcTextBoxes[i].x, npcTextBoxes[i].y, 20, BLACK);
                npcIrcY += 75;
            }

            //print player boxes
            for (int i = 0; i <= messageCount; i++)
            {
                userTextBoxes[i].height = 30;
                userTextBoxes[i].width = MeasureText(npcMessage.c_str(), 20) + 25;
                userTextBoxes[i].x = userIrcX;
                userTextBoxes[i].y = userIrcY;
                DrawRectangleRec(userTextBoxes[i], BLUE);
                DrawText(userMessageText[i].c_str(), userTextBoxes[i].x, userTextBoxes[i].y, 20, BLACK);
                userIrcY += 75;
            }

            //print text in decision box
            DrawText(IrcMessage(2001).c_str(), DecisionBox[0].x, DecisionBox[0].y, 20, WHITE);
            DrawText(IrcMessage(2002).c_str(), DecisionBox[1].x, DecisionBox[1].y, 20, WHITE);
            DrawText(IrcMessage(2003).c_str(), DecisionBox[2].x, DecisionBox[2].y, 20, WHITE);
            



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

std::string IrcMessage(int message)
{
    /* ==MESSAGES==
    
    messages are structured as so;
        - Each message and it's responses are contiguously ordered in an array.
        - So if the message "Hello, you" is stored in array index [0], "Hello to you too", "Hey", and "Howdy!" are [1], [2], and [3]
            - i.e., messagesArray[4] {"Hello, you", "Hello to you too", "Hey", "Howdy!"};
        -This is done to keep the number of arrays down, and imo is just as managable as maintaining mutiple arrays, since there are multiple responses for each message
    */
    std::string messages[3000]{"\0"};
    //challenge messages

    //campaign messages

    //tutorial messages
    messages[2000] = "Hey, it's Q. You get in yet? We're dying to see what's in their db.";
    messages[2001] = "I'm sorry... Who?";
    messages[2002] = "Uhh, yeah. I got in...";
    messages[2003] = "<Don't respond>";
    

    return messages[message];
}
