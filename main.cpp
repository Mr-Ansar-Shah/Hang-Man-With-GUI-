#include <iostream>
#include <raylib.h>
#include <fstream>
#include <vector>
#include <cstring>
#include <ctime>  // Add missing include for time()
using namespace std;

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 600;
Font font; // Declare font as global variable
char inputText[40] = "\0";
char inputWord[20] = "\0";
int letterCount = 0;
int WordCount = 0;

// Function prototypes
void text_3d(const string& text, int d, int j, int size, int spacing, Color color);
void input_hint();
void input_word();
void guess_word();
void p1();
void single(Rectangle button, float roundness, int a);
void dual(Rectangle button, float roundness, int a);
void exit(Rectangle button, float roundness, int a);

// Simple text drawing function that works with default font if custom font fails
void text_3d(const string& text, int d, int j, int size, int spacing, Color color) {
    // Safety check
    if (text.empty()) return;
    
    // Shadow position
    float k = (float)(d + 5);
    float l = (float)(j + 5);
    
    // Text position
    float m = (float)d;
    float n = (float)j;
    
    // Draw shadow first
    DrawTextEx(font, text.c_str(), (Vector2){k, l}, (float)size, (float)spacing, DARKGRAY);
    // Draw main text
    DrawTextEx(font, text.c_str(), (Vector2){m, n}, (float)size, (float)spacing, color);
}

void input_hint() {
    SetExitKey(KEY_NULL);  // Disable ESC key for exit
    letterCount = 0;  // Reset letter count
    inputText[0] = '\0';  // Clear input text
    
    while (!WindowShouldClose()) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (letterCount < 39)) {  // Leave room for null terminator
                inputText[letterCount] = (char)key;
                letterCount++;
                inputText[letterCount] = '\0';  // null-terminate
            }
            key = GetCharPressed();
        }
        
        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE)) {
            letterCount--;
            if (letterCount < 0) letterCount = 0;
            inputText[letterCount] = '\0';
        }
        
        if (IsKeyPressed(KEY_ENTER)) {
            EndDrawing();
            input_word();
            return;
        }
        
        BeginDrawing();
        ClearBackground(BLACK);
        text_3d("1st Player Turn", 150, 50, 70, 10, YELLOW);
        text_3d("Enter The Hint: ", 200, 200, 30, 2, WHITE);
        text_3d(inputText, 200, 300, 30, 2, LIGHTGRAY);
        EndDrawing();
    }
}

void input_word() {
    SetExitKey(KEY_NULL);
    WordCount = 0;  // Reset word count
    inputWord[0] = '\0';  // Clear input word
    
    while (!WindowShouldClose()) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (WordCount < 19)) {  // Leave room for null terminator
                inputWord[WordCount] = (char)key;
                WordCount++;
                inputWord[WordCount] = '\0';  // null-terminate
            }
            key = GetCharPressed();
        }
        
        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE)) {
            WordCount--;
            if (WordCount < 0) WordCount = 0;
            inputWord[WordCount] = '\0';
        }
        
        if (IsKeyPressed(KEY_ENTER) && WordCount > 0) {  // Only proceed if word is not empty
            guess_word();
            return;
        }
        
        BeginDrawing();
        ClearBackground(BLACK);
        text_3d("1st Player Turn", 150, 50, 70, 10, YELLOW);
        text_3d("Enter The Word: ", 200, 200, 30, 2, WHITE);
        text_3d(inputWord, 200, 300, 30, 2, LIGHTGRAY);
        EndDrawing();
    }
}

void guess_word() {
    // Create a fixed-size array that's definitely large enough
    char word_x[21];  // Maximum 20 characters + null terminator
    
    // Initialize all positions to 'X'
    for (int i = 0; i < WordCount; i++) {
        word_x[i] = 'X';
    }
    word_x[WordCount] = '\0';  // Null terminate
    
    int lives = WordCount;
    int j = 0;
    int flag = 0;
    string msj = "";
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        text_3d("Player 2 Turn", 100, 100, 50, 5, YELLOW);
        text_3d("Life: " + to_string(lives), SCREEN_WIDTH-200, 100, 30, 4, RED);
        text_3d("HINT:", 100, 200, 30, 4, WHITE);
        text_3d(inputText, 250, 200, 30, 4, WHITE);
        text_3d("GUESS:", 100, 300, 30, 4, WHITE);
        text_3d(string(word_x), 250, 300, 30, 4, WHITE);
        text_3d(msj, 100, 350, 30, 2, SKYBLUE);
        
        char guess = GetCharPressed();
        if (guess > 0) {
            j = 0;
            for (int i = 0; i < WordCount; i++) {
                if (guess == inputWord[i]) {
                    word_x[i] = inputWord[i];
                    inputWord[i] = '\0';
                    j++;
                    flag++;
                    break;
                }
            }
            
            if (j == 0) {
                lives--;
                if (lives <= 0) {
                    EndDrawing();
                    while (!WindowShouldClose()) {
                        BeginDrawing();
                        ClearBackground(BLACK);
                        text_3d("YOU LOSE", 100, 100, 100, 10, WHITE);
                        text_3d("Press R For Return", 200, 200, 30, 10, RED);
                        if (IsKeyPressed(KEY_R)) {
                            return;  // Return to main menu instead of closing window
                        }
                        EndDrawing();
                    }
                }
                msj = "NOT! CORRECT";
            } else if (flag == WordCount) {
                EndDrawing();
                while (!WindowShouldClose()) {
                    BeginDrawing();
                    ClearBackground(BLACK);
                    text_3d("YOU WON", 100, 100, 100, 10, WHITE);
                    text_3d("Press R For Return", 200, 200, 30, 10, RED);
                    if (IsKeyPressed(KEY_R)) {
                        return;  // Return to main menu instead of closing window
                    }
                    EndDrawing();
                }
            } else {
                msj = "WOW";
            }
        }
        EndDrawing();
    }
}

void p1() {
    ifstream read("cpu.txt");
    if (!read.is_open()) {
        cout << "Error: Could not open cpu.txt" << endl;
        return;
    }
    
    // Reset global variables
    WordCount = 0;
    inputWord[0] = '\0';
    inputText[0] = '\0';
    
    // Select a random line
    srand((unsigned int)time(NULL));  // Better seeding
    int target_line = 1;  // Start with a valid line
    
    do {
        target_line = rand() % 50;
    } while (target_line % 2 == 0);  // Ensure odd line
    
    // Read the hint
    for (int i = 0; i < target_line; i++) {
        read.getline(inputText, sizeof(inputText));
        if (read.fail()) {
            // Reset if we hit end of file
            read.clear();
            read.seekg(0, ios::beg);
        }
    }
    
    // Read the word
    string var;
    getline(read, var);
    if (read.fail()) {
        // If we hit end of file, use a default word
        var = "hangman";
    }
    
    // Copy word safely
    WordCount = 0;
    for (size_t i = 0; i < var.length() && i < 19; i++) {
        inputWord[WordCount] = var[i];
        WordCount++;
    }
    inputWord[WordCount] = '\0';
    read.close();
    
    // Create a fixed-size array
    char word_x[21];  // Maximum 20 characters + null terminator
    
    for (int i = 0; i < WordCount; i++) {
        word_x[i] = 'X';
    }
    word_x[WordCount] = '\0';
    
    int lives = WordCount;
    int j = 0;
    int flag = 0;
    string msj = "";
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        text_3d("YOUR Turn", 100, 100, 50, 5, YELLOW);
        text_3d("Lives: " + to_string(lives), SCREEN_WIDTH-200, 100, 30, 4, RED);
        text_3d("HINT:", 100, 200, 30, 4, WHITE);
        text_3d(inputText, 250, 200, 30, 4, WHITE);
        text_3d("GUESS:", 100, 300, 30, 4, WHITE);
        text_3d(string(word_x), 250, 300, 30, 4, WHITE);
        text_3d(msj, 100, 350, 30, 2, SKYBLUE);
        
        char guess = GetCharPressed();
        if (guess > 0) {
            j = 0;
            for (int i = 0; i < WordCount; i++) {
                if (guess == inputWord[i]) {
                    word_x[i] = inputWord[i];
                    inputWord[i] = '\0';
                    j++;
                    flag++;
                    break;
                }
            }
            
            if (j == 0) {
                lives--;
                if (lives <= 0) {
                    EndDrawing();
                    while (!WindowShouldClose()) {
                        BeginDrawing();
                        ClearBackground(BLACK);
                        text_3d("YOU LOSE", 100, 100, 100, 10, WHITE);
                        text_3d("Press R For Return", 200, 200, 30, 10, RED);
                        if (IsKeyPressed(KEY_R)) {
                            return;  // Return to main menu instead of closing window
                        }
                        EndDrawing();
                    }
                }
                msj = "NOT! CORRECT";
            } else if (flag == WordCount) {
                EndDrawing();
                while (!WindowShouldClose()) {
                    BeginDrawing();
                    ClearBackground(BLACK);
                    text_3d("YOU WON", 100, 100, 100, 10, WHITE);
                    text_3d("Press R For Return", 200, 200, 30, 10, RED);
                    if (IsKeyPressed(KEY_R)) {
                        return;  // Return to main menu instead of closing window
                    }
                    EndDrawing();
                }
            } else {
                msj = "WOW";
            }
        }
        EndDrawing();
    }
}

void exit(Rectangle button, float roundness, int a) {
    button.y += a;
    Vector2 mouse = GetMousePosition();
    bool chk = CheckCollisionPointRec(mouse, button);
    DrawRectangleRounded(button, roundness, 8, chk ? LIGHTGRAY : GRAY);
    if (chk && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        EndDrawing();  // End current drawing before exiting
        UnloadFont(font);  // Properly unload resources
        CloseWindow();
        exit(0);  // Force exit the application using global exit function
    }
    text_3d("EXIT", 460, 410, 30, 2, WHITE);
}

void single(Rectangle button, float roundness, int a) {
    button.y += a;
    Vector2 mouse = GetMousePosition();
    bool chk = CheckCollisionPointRec(mouse, button);
    DrawRectangleRounded(button, roundness, 8, chk ? LIGHTGRAY : GRAY);
    if (chk && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        p1();
    }
    text_3d("VS CPU", 440, 260, 30, 2, WHITE);
}

void dual(Rectangle button, float roundness, int a) {
    button.y += a;
    Vector2 mouse = GetMousePosition();
    bool chk = CheckCollisionPointRec(mouse, button);
    DrawRectangleRounded(button, roundness, 8, chk ? LIGHTGRAY : GRAY);
    if (chk && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        input_hint();
    }
    text_3d("DUAL MODE", 415, 335, 30, 2, WHITE);
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hang Man");
    
    // Load font after window initialization
    font = LoadFont("assets/main_font.ttf");
    
    SetTargetFPS(60);
    
    Rectangle button = {400, 250, 200, 50}; // x, y, width, height
    float roundness = 0.5f;
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        text_3d("MAIN MENU", 250, 50, 80, 10, YELLOW);
        single(button, roundness, 0);
        dual(button, roundness, 75);
        exit(button, roundness, 150);
        EndDrawing();
    }   
    UnloadFont(font);
    CloseWindow();
    return 0;
}