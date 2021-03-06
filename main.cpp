#include "raylib.h"
#include "string"
#include "vector"
#include "irc.h"
#define MAX_INPUT_CHARS 1024

//Global variables
std::string IrcMessage(int Message);
struct Commands
{
    void testCommand(std::string inputText, float x, float y)
    {
        int numberOfCommands{2};
        const char commands[50][1000]{
            "ping",
            "ipconfig"};
        std::string pingString = &commands[0][0]; //ping

        for (int i = 0; i < numberOfCommands; i++) //loops through all commands, if it finds one, executes
        {
            /*start commands*/
            std::string ping = &commands[0][0]; //ping command
            std::string ipconfig = &commands[1][0];
            /*end commands*/
            if (inputText == ping)
            {
                DrawText("Pinging...", x, y, 30, MAROON);
            } else if (inputText == ipconfig)
            {
                DrawText("Current IP: 10.10.0.9", x, y, 30, MAROON);
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
            }
        }
    }
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

    //screen altering functions
    const char clear[6] = "clear";
    const char enterIRC[4] = "irc";
    bool clearScreen{false};
    
    bool overText{false};

    //irc variables
    int messageCount{0};
    int selIndex{0};
    std::string npcMessage[50];
    std::string userMessage[50];
    std::string npcMessageText[50] {"\0"};
    std::string oldNpcMessageText[50];
    std::string userMessageText[50] {"\0"};
    std::string oldUserMessageText[50];
    bool displayMessage[50]{false};
    bool level[100]{false};
    int currentMessageNum{0}; //
    int userMessageNum{0};
    int selectedOption{-1};
    int npcChatNumber = 2000;
    bool start{false};

    //game states
    bool inIRC {false};
    bool paused {false};
    Rectangle textBox = {10, 10, screenWidth - 20, screenHeight - 20};
    Rectangle OuterBox = {0, 0, screenWidth, screenHeight};
    Rectangle IRCBox = {10, 10, screenWidth - 40, screenHeight - 40};
    bool mouseOnText = false;

    int framesCounter = 0;

    SetTargetFPS(60); // Set our game to run at 10 frames-per-second
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
            int decBoxWidth{375};
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

            //campaign logic?
            if (selectedOption == -1)
            {
                npcChatNumber = 2000;
            }
            npcMessageText[messageCount] = IrcMessage(npcChatNumber);
            //choose user message
            std::string userMessage = IrcMessage(npcChatNumber + 1 + currentMessageNum);
            userMessageText[messageCount] = userMessage;
            npcTextBoxes[messageCount].width = MeasureText(npcMessageText[messageCount].c_str(), 20) + 25;

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
                selectedOption = selIndex;
                selIndex = 0;
                start = true;
            }

            //campign
            if (start)
            {
                Irc irc;
                if (messageCount == 1)
                {
                    npcChatNumber = irc.level1(selectedOption);
                }
                 if (messageCount == 2)
                 {
                     npcChatNumber = irc.level2(selectedOption);
                 }
                 if (messageCount == 3)
                 {
                     //npcChatNumber = irc.level3(selectedOption);
                 }
                
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
                npcTextBoxes[i].width = MeasureText(npcMessageText[i].c_str(), 20) + 25;
                npcTextBoxes[i].x = npcIrcX;
                npcTextBoxes[i].y = npcIrcY;
                DrawRectangleRec(npcTextBoxes[i], RED);
                DrawText(npcMessageText[i].c_str(), npcTextBoxes[i].x, npcTextBoxes[i].y, 20, BLACK);
                npcIrcY += 75;
            }

            //print player boxes
            for (int i = 0; i <= messageCount; i++)
            {
                userTextBoxes[i].height = 30;
                userTextBoxes[i].width = MeasureText(userMessageText[i].c_str(), 20) + 25;
                userTextBoxes[i].x = userIrcX;
                userTextBoxes[i].y = userIrcY;
                DrawRectangleRec(userTextBoxes[i], BLUE);
                DrawText(userMessageText[i].c_str(), userTextBoxes[i].x, userTextBoxes[i].y, 20, LIGHTGRAY);
                userIrcY += 75;
            }

            //print text in decision box
            DrawText(IrcMessage(npcChatNumber + 1).c_str(), DecisionBox[0].x, DecisionBox[0].y, 20, WHITE);
            DrawText(IrcMessage(npcChatNumber + 2).c_str(), DecisionBox[1].x, DecisionBox[1].y, 20, WHITE);
            DrawText(IrcMessage(npcChatNumber + 3).c_str(), DecisionBox[2].x, DecisionBox[2].y, 20, WHITE);

            if (IsKeyPressed(KEY_ENTER))
            {
                messageCount++;
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

std::string IrcMessage(int message)
{
    /* ==MESSAGES==
    
    messages are structured as so;
        - Each message and it's responses are contiguously ordered in an array.
        - So if the message "Hello, you" is stored in array index [0], "Hello to you too", "Hey", and "Howdy!" are [1], [2], and [3]
            - i.e., messagesArray[3] {"Hello, you", "Hello to you too", "Hey", "Howdy!"};
        -This is done to keep the number of arrays down, and imo is just as managable as maintaining mutiple arrays, since there are multiple responses for each message
    */
    std::string messages[3000]{"\0"};
    //challenge messages

    //campaign messages

    //tutorial messages
    messages[2000] = "Hey, it's Q. You get in yet? We're dying to see what's in their db.";
    messages[2001] = "I'm sorry... Who?"; //2000
    messages[2002] = "Uhh, yeah. I got in..."; //2000
    messages[2003] = "<Don't respond>"; //2000
    messages[2004] = "Uhh... Q. Man, we've beed at this for weeks, don't be getting weird on me now..";
    messages[2005] = "I'm sorry, I just don't remember. \nDo I know you IRL?"; //2004
    messages[2006] = "Oh, Q! Yeeah, I remember now."; //2004
    messages[2007] = "<Leave on read>"; //2004
    messages[2008] = "YES!!! Dude, I knew you could do it! Are you seeding those docs yet?";
    messages[2009] = "Nope, no seeding yet"; //2008
    messages[2010] = "Of course, i'll send the address in the group"; //2008
    messages[2011] = "No... I won't be either."; //2008
    messages[2012] = "Hello..? Anyone there?";
    messages[2013] = "Yeah I'm here, sorry I was AFK";
    messages[2014] = "No, no one's home lol";
    messages[2015] = "<Leave on read>";
    messages[2016] = "No, you don't.. Hey, if you don't wanna do this just let me know, I can get someone else.";
    messages[2017] = "No no no, I'm just kidding. I got in."; //2016
    messages[2018] = "Oh yeah I do! You're the string bean lookin' fella!"; //2016
    messages[2019] = "Get someone else? I think you're talking to the wrong guy"; //2016



    return messages[message];
}
