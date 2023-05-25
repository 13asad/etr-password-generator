#include <raylib.h> // Graphics Library
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h> // Allows use of Boolean data type
#include <time.h> // Time related functions, such as time()
#include <string.h> // String manipulation functions, such as strcat() and strlen()



// Function to generate a random number between min and max (inclusive)
int generateRandom(int min, int max)
{
    int num = (rand() % (max - min + 1)) + min;
    return num;
}


// Function to read a random line (word) from a file with the given path
char *fileRead(char path[])
{
    char word[10];
    char * pword = malloc(sizeof(char) * 8); /*allocate memory for pword */
    FILE * pFile;
    pFile = fopen(path, "r");
    if (pFile == NULL)
    {
        return("ERROR!");
    }

    int randomLine = generateRandom(1, 500);
    int currentLine = 1;

    while (fgets(word, 10, pFile) != NULL)
    {
        if (currentLine == randomLine)
        {
            strcpy(pword, word);
            break;
        }
        currentLine++;
    }

    fclose(pFile);
    pword[strcspn(pword, "\n")] = 0; /*removes \n from pword*/
    strcat(pword, " ");

    // Return the word read from the file
    return pword;
}


// Function to generate a password with the given number of words (wordPassCounter)
char * generatePass(int wordPassCounter, char * pass)
{
    char path[100];
    pass[0] = '\0';  /*initialises the C-string to avoid runtime erros when using strcat*/
    int count = 0;
    while (count != wordPassCounter)
    {
        count++;
        sprintf(path, "assets/dictionaries/%d.txt", generateRandom(4,7));
        strcat(pass, fileRead(path));
        if (strcmp(pass, "ERROR!") == 0)
        {
        return pass;
        }
    }

    for (int i = 0; i < strlen(pass) - 1; i++)
    {
        if (pass[i] == ' ')
        {
            pass[i] = '-';
        }
    }

    // Return the generated password
    return(pass);
}


// Function to draw the Stage 0 screen, where the user can click the "Generate" button
int stageZeroDraw()
{
    Rectangle generateButton = {245, 425, 500, 125};
    ClearBackground(BEIGE); // Sets background colour
    DrawText("Easy To Remember", 170, 75, 70, BLACK);
    DrawText("Password Generator", 130, 150, 70, BLACK);
    DrawRectangleRec(generateButton, WHITE);
    DrawText("GENERATE", 297, 457, 70, BLACK);

    // checks if the user pressed the generate button
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), generateButton))
    {
        return 1;
    }

    // Return 1 if the "Generate" button is pressed, otherwise return 0
    return 0;
}


// Function to draw the Stage 1 screen, where the user can input the number of words for the password
int stageOneDraw(char *wordPass, bool finalCheck)
{

    ClearBackground(LIGHTGRAY);
    DrawText("Enter number of words", 85, 145, 70, BLACK);
    DrawRectangle(245, 300, 500, 125, WHITE);
    DrawText(wordPass, 460, 320, 100, BLACK);
    Rectangle nextButton = {245, 560, 500, 125};
    DrawRectangleRec(nextButton, WHITE);
    DrawText("NEXT", 355, 580, 100, BLACK);

    // checks if the user pressed the Next button and if all the previous checks are all true
    if ((IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), nextButton)) && finalCheck == true)
    {
        return 2;
    }
    // Return 2 if the "Next" button is pressed and all conditions are met, otherwise return 1
    return 1;
}

float loadingProgress = 0.0f;
// Function to draw the Stage 2 screen, where the generated password is displayed and the user can generate another password
void stageTwoDraw(char * pass, int wordPassCounter)
{
    // Loading animation variables
    const char spinner[] = "......";
    static int spinnerIndex = 0;
    const float spinnerDelay = 0.1f; // Adjust this value to control the spinner speed
    static float timeCounter = 0.0f;

    if (loadingProgress < 100.0f) {
        ClearBackground(GRAY);
        DrawText("Generating password...", 300, 300, 40, DARKGRAY);

        // Draw spinner
        DrawText(&spinner[spinnerIndex], 450, 400, 40, DARKGRAY);
        timeCounter += GetFrameTime();
        if (timeCounter >= spinnerDelay) {
            timeCounter = 0;
            spinnerIndex = (spinnerIndex + 1) % (sizeof(spinner) - 1);
        }

        // Simulate loading progress
        loadingProgress += 2.0f; // Adjust this value to control the loading speed
    } else {
        Rectangle generateAnotherButton = {200, 500, 600, 80};
        int anothertextWidth = MeasureText("GENERATE ANOTHER", 50);
        int textXPosition = generateAnotherButton.x + (generateAnotherButton.width - anothertextWidth) / 2;
        DrawRectangleRec(generateAnotherButton, WHITE);
        DrawText("GENERATE ANOTHER", textXPosition, 520, 50, BLACK);

        ClearBackground(GOLD);
        int textWidth = MeasureText(pass, 60);
        int spacing = (1000 - textWidth) / 2;
        if (strcmp(pass, "ERROR!") == 0) {
            DrawText("DICTIONARY NOT FOUND", 50, 300, 70, RED);
            return;
        }
        DrawText("Your Password is", 170, 145, 70, BLACK);
        if (wordPassCounter == 3) {
            DrawText(pass, spacing, 400, 60, BLACK);
        } else {
            DrawText(pass, spacing, 400, 60, BLACK);
        }
    }
}




// Main function
int main()
{
    srand(time(NULL));
    const int screenWidth = 1000;
    const int screenHeight = 750;
    int stage = 0;
    int wordPassCounter = 0;
    char wordPass[7];
    char pass[32];
    int key;

    bool checkPassGenerate = false;
    bool checkNumPresent = true;
    bool checkSingleDigit = true;
    bool checkEmptySpace = true;

    // Initialises the height and width of the screen as well as the name of the window
    InitWindow(screenWidth, screenHeight, "Easy To Remember Password Generator");
    SetTargetFPS(30);

    // Initialises the C-String wordPass which holds the number of words used for the password
    wordPass[wordPassCounter] = ' ';
    wordPass[wordPassCounter + 1] = '\0';

    // Main game loop
    // Loop exits if esc key is pressed or if window is closed
    while (!WindowShouldClose())
    {
        BeginDrawing();
        switch (stage) // stage represents the screen which the users see depending on their choices
        {
        case 0: // stage 0 is the main menu
            stage = stageZeroDraw();
            break;
        case 1: // stage 1 is where the user input the number of words used for the password
            key = GetKeyPressed();
            checkEmptySpace = true;

            if (key == KEY_BACKSPACE)
            {
                wordPassCounter--;
                checkNumPresent = true;
                checkSingleDigit = true;
                // prevents wordPassCounter from going into the negatives
                if (wordPassCounter <= 0)
                {
                    wordPassCounter = 0;
                }
                wordPass[wordPassCounter] = '\0';
            }
            else if (!(key >= 48 && key <= 57) && key != 0 && checkNumPresent == true && checkSingleDigit == true)
            {
                // used to detect if any other character other than numbers have been input
                wordPass[wordPassCounter] = key;
                wordPass[wordPassCounter + 1] = '\0';
                wordPassCounter++;
                checkNumPresent = false;
            }
            else if (key != 0 && checkNumPresent == true && checkSingleDigit == true && checkNumPresent == true && wordPassCounter < 1)
            {
                wordPass[wordPassCounter] = key;
                wordPass[wordPassCounter + 1] = '\0';
                wordPassCounter++;
                if (!(key == 51 || key == 52) || wordPassCounter > 1)
                {
                    checkSingleDigit = false;
                }
            }

            if (checkNumPresent == false)
            {
                DrawText("ONLY NUMBERS ARE ALLOWED", 115, 475, 50, RED);
            }

            if (wordPassCounter == 0)
            {
                checkEmptySpace = false;
            }

            if (checkSingleDigit == false)
            {
                DrawText("ONLY 3 OR 4 ALLOWED", 205, 475, 50, RED);
            }
            stage = stageOneDraw(wordPass, checkNumPresent && checkSingleDigit && checkEmptySpace);
            break;
        case 2:
        if (checkPassGenerate == false)
        {
            generatePass(atoi(wordPass), pass); // atoi(wordPass) converts wordPass from string to integer
            checkPassGenerate = true;
        }
            stageTwoDraw(pass, atoi(wordPass));

        // Check if the "Generate Another" button is clicked
        Rectangle generateAnotherButton = {200, 500, 600, 80};
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), generateAnotherButton)) {
            checkPassGenerate = false;
            loadingProgress = 0.0f; // Reset loading progress
        }
        break;
        }
        EndDrawing();
    }
    CloseWindow(); // Closes and unloads the window
    return 0;
}