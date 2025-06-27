#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <conio.h>
#include <cctype>
#include <string>
using namespace std;

// ANSI Color Codes
const string COLOR_RESET = "\033[0m";
const string COLOR_HIDDEN = "\033[92m";  // Green
const string COLOR_TREASURE = "\033[93m"; // Yellow
const string COLOR_TRAP = "\033[91m";    // Red
const string COLOR_SAFE = "\033[96m";    // Cyan
const string COLOR_TEXT = "\033[97m";    // White
const string COLOR_MENUS = "\033[95m";    // Purple
const string COLORS_HIGHLIGHT = "\033[104m\033[97m"; // Highlight with white text
const string COLOR_BORDER = "\033[94m";  // Blue
const string COLOR_PLAYER = "\033[105m\033[97m"; // Purple background with white text
const string COLOR_CREDIT = "\033[103m\033[30m"; // Yellow background with black text
const string COLOR_TIME = "\033[33m";    // Orange/Yellow for time display

// Game Constants
const int EASY_SIZE = 5;
const int MEDIUM_SIZE = 6;
const int HARD_SIZE = 7;

class TreasureHuntGame {
private:
    char** gameGrid;
    char** revealedGrid;
    int gridSize;
    int difficulty;
    int health;
    int timeLimit;
    int maxHealth;
    int score;
    int treasuresTotal;
    int treasuresFound;
    int playerX, playerY;
    bool gameOver;
    string playerName;

    void clearScreen() { system("cls"); }

    void pause() {
        cout << COLOR_TEXT << "\nPress backspace key to continue..." << COLOR_RESET;
        while (_getch() != '\b');
    }

    void initializeGrid() {
        // Create game grid (hidden from player)
        gameGrid = new char* [gridSize];
        for (int i = 0; i < gridSize; i++) {
            gameGrid[i] = new char[gridSize];
        }

        // Create revealed grid (what player sees)
        revealedGrid = new char* [gridSize];
        for (int i = 0; i < gridSize; i++) {
            revealedGrid[i] = new char[gridSize];
            for (int j = 0; j < gridSize; j++) {
                revealedGrid[i][j] = '?';
            }
        }

        // Fill game grid with treasures, traps and safe spots
        treasuresTotal = 0;
        int trapProb = 15 + difficulty * 5;
        int treasureProb = 15 + difficulty * 2;

        for (int i = 0; i < gridSize; i++) {
            for (int j = 0; j < gridSize; j++) {
                int gen = rand() % 100;
                if (gen < treasureProb) {
                    gameGrid[i][j] = 'T';
                    treasuresTotal++;
                }
                else if (gen < treasureProb + trapProb) {
                    gameGrid[i][j] = 'X';
                }
                else {
                    gameGrid[i][j] = '.';
                }
            }
        }
        gameGrid[0][0] = '.';
    }

    void cleanupGrid() {
        if (gameGrid) {
            for (int i = 0; i < gridSize; i++) {
                delete[] gameGrid[i];
            }
            delete[] gameGrid;
            gameGrid = nullptr;
        }

        if (revealedGrid) {
            for (int i = 0; i < gridSize; i++) {
                delete[] revealedGrid[i];
            }
            delete[] revealedGrid;
            revealedGrid = nullptr;
        }
    }

    void displayGame(int timeLeft) {
        clearScreen();
        cout << COLOR_BORDER << "*-------------" << COLOR_MENUS << "Treasure Hunt" << COLOR_BORDER << "------------*\n" << COLOR_RESET << endl;
        cout << COLOR_TEXT << "Player: " << COLOR_MENUS << playerName << COLOR_TEXT << "  | Difficulty: ";
        switch (difficulty) {
        case 1: cout << COLOR_SAFE << "Easy"; break;
        case 2: cout << COLOR_TREASURE << "Medium"; break;
        case 3: cout << COLOR_TRAP << "Hard"; break;
        }
        cout << COLOR_TEXT << endl << "Health: " << COLOR_SAFE << health << COLOR_TEXT << "/" << maxHealth
            << "  | Score: " << COLOR_TREASURE << score << COLOR_RESET << endl;
        cout << COLOR_TEXT << "Time Left: " << COLOR_TIME << timeLeft << " seconds" << COLOR_RESET << "\n";

        cout << COLOR_TREASURE << "\nPlayer Position: " << COLOR_TRAP << "(" << playerX << "," << playerY << ")" << COLOR_RESET << endl;

        // Display grid with coordinates
        cout << COLOR_BORDER << "   ";
        for (int j = 0; j < gridSize; j++) cout << " " << j << " ";
        cout << COLOR_RESET << endl;

        for (int i = 0; i < gridSize; i++) {
            cout << COLOR_BORDER << " " << i << " " << COLOR_RESET;
            for (int j = 0; j < gridSize; j++) {
                if (i == playerX && j == playerY) {
                    cout << COLOR_PLAYER << " P " << COLOR_RESET;
                }
                else {
                    switch (revealedGrid[i][j]) {
                    case '?': cout << COLOR_HIDDEN; break;
                    case 'T': cout << COLOR_TREASURE; break;
                    case 'X': cout << COLOR_TRAP; break;
                    case '.': cout << COLOR_SAFE; break;
                    }
                    cout << " " << revealedGrid[i][j] << " " << COLOR_RESET;
                }
            }
            cout << endl;
        }

        cout << COLOR_TEXT << "\nTreasures: " << COLOR_TREASURE << treasuresFound << COLOR_TEXT << "/" << treasuresTotal << endl;
        cout << COLOR_TEXT << "a : Left | d : Right | w : Up | s : Down\n";
        cout << COLOR_TRAP << "To Exit the game press ESC key...." << COLOR_RESET << endl;
    }

    void movePlayer(char direction) {
        int newX = playerX, newY = playerY;

        switch (tolower(direction)) {
        case 'w': newX--; break;
        case 'a': newY--; break;
        case 's': newX++; break;
        case 'd': newY++; break;
        default: return;
        }

        // Check boundaries
        if (newX >= 0 && newX < gridSize && newY >= 0 && newY < gridSize) {
            playerX = newX;
            playerY = newY;
            revealCell();
        }
        else {
            cout << "\a";
            Sleep(900);
            cout << COLOR_TRAP << "Out of bound..." << COLOR_RESET << endl;
        }
    }

    void revealCell() {
        // Prevent revisiting the same cell for score/health
        if (revealedGrid[playerX][playerY] != '?') return;

        revealedGrid[playerX][playerY] = gameGrid[playerX][playerY];

        switch (gameGrid[playerX][playerY]) {
        case 'T':
            if (difficulty == 1) {
                score = score + (rand() % 20 + 1);
            }
            else if (difficulty == 2) {
                score = score + (rand() % 20 + 1);
            }
            else {
                score = score + (rand() % 20 + 1);
            }
            treasuresFound++;
            break;
        case 'X':
            if (difficulty == 1) {
                health = health - (rand() % 15 + 1);
            }
            else if (difficulty == 2) {
                health = health - (rand() % 20 + 1);
            }
            else {
                health = health - (rand() % 25 + 1);
            }

            if (health <= 0) gameOver = true;
            break;
        }

        if (treasuresFound == treasuresTotal) {
            gameOver = true;
        }
    }

    void showInstructions() {
        clearScreen();
        cout << COLOR_BORDER << "----------------------------------------------------\n";
        cout <<  "|"<< COLOR_MENUS <<"     INSTRUCTIONS                                 "<< COLOR_MENUS <<"|\n" << COLOR_RESET;
        cout << COLOR_BORDER << "|" << COLOR_TREASURE << "Find all treasures (" << COLOR_TREASURE << "T" << COLOR_TEXT << ") while avoiding traps (" << COLOR_TRAP << "X" << COLOR_TEXT << ")   " << COLOR_BORDER << "|\n";
        cout << "|" << COLOR_SAFE << "Controls: " << COLOR_TREASURE << "W=Up" << COLOR_SAFE << ", " << COLOR_TREASURE << "A=Left" << COLOR_SAFE << ", " << COLOR_TREASURE << "S=Down" << COLOR_SAFE << ", " << COLOR_TREASURE << "D=Right" << COLOR_SAFE << "           " << COLOR_BORDER << "|\n";
        cout << "|" << COLOR_SAFE << "Press " << COLOR_TREASURE << "ESC" << COLOR_SAFE << " during game to return to menu           " << COLOR_BORDER << "|\n";
        cout << "|" << COLOR_MENUS << "  Difficulty Levels:                              " << COLOR_BORDER << "|\n";
        cout << "|" << COLOR_SAFE << "1. " << COLOR_SAFE << "Easy: 5x5 grid, 100 health" << COLOR_SAFE << "                     " << COLOR_BORDER << "|\n";
        cout << "|" << COLOR_SAFE << "2. " << COLOR_TREASURE << "Medium: 6x6 grid, 80 health" << COLOR_SAFE << "                    " << COLOR_BORDER << "|\n";
        cout << "|" << COLOR_SAFE << "3. " << COLOR_TRAP << "Hard: 7x7 grid, 60 health" << COLOR_SAFE << "                      " << COLOR_BORDER << "|\n";
        cout << "----------------------------------------------------\n" << COLOR_RESET;
        pause();
    }
    






    void playGame() {
        playerX = playerY = 0;
        revealedGrid[0][0] = gameGrid[0][0];
        gameOver = false;

        auto startTime = std::chrono::steady_clock::now();

        // Set console to non-blocking input mode
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode = 0;
        GetConsoleMode(hStdin, &mode);
        SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT));
        int timeLeft=0;
        while (!gameOver) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
            timeLeft = timeLimit - elapsed;

            if (timeLeft <= 0) {
                displayGame(timeLeft);
                cout << COLOR_TRAP << "\nTime's up! You ran out of time!\n" << COLOR_TEXT;
                cout << COLOR_TEXT << "Final Score: " << COLOR_TREASURE << score << COLOR_RESET << "\n";
                cout << COLOR_TEXT << "Time Limit: " << COLOR_TIME << timeLimit << " seconds" << COLOR_RESET << "\n";
                cleanupGrid();

                // Restore console mode
                SetConsoleMode(hStdin, mode);
                pause();
                return;
            }

            displayGame(timeLeft);

            // Non-blocking input check
            if (_kbhit()) {
                char input = tolower(_getch());
                if (input == 27) {
                    // Restore console mode before returning
                    SetConsoleMode(hStdin, mode);
                    return;
                }
                movePlayer(input);
            }

            // Small delay to prevent CPU overuse
            Sleep(100);
        }

        auto endTime = std::chrono::steady_clock::now();
        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

        // Restore console mode
        SetConsoleMode(hStdin, mode);

        displayGame(timeLeft);

        if (health <= 0) {
            cout << COLOR_TRAP << "\nGAME OVER! You ran out of health!\n" << COLOR_TEXT;
        }
        else {
            cout << COLOR_TREASURE << "\nCONGRATULATIONS! You found all treasures!\n" << COLOR_TEXT;
        }

        cout << COLOR_TEXT << "Final Score: " << COLOR_TREASURE << score << COLOR_RESET << " \n";
        cout << COLOR_TEXT << "Time Taken: " << COLOR_TIME << elapsedSeconds << " seconds" << COLOR_RESET << "\n";
        cleanupGrid();
        pause();
    }




    void credit() {
        clearScreen();
        cout << COLOR_BORDER << "--------------------------\n";
        for (int i = 0; i < 6; i++) {
            if (i == 3) {
                cout << "|    "<< COLOR_TREASURE <<"Malik Kamran Ali"<< COLOR_BORDER<<"    |\n";
            }
            else {
                cout << "|                        |\n";
            }
        }
        cout << "--------------------------\n" << COLOR_RESET;
        pause();
    }

public:
    TreasureHuntGame() : gameGrid(nullptr), revealedGrid(nullptr), playerName("") {
        srand(time(0));
    }

    ~TreasureHuntGame() {
        cleanupGrid();
    }

    void showMainMenu() {
        char choice;
        do {
            clearScreen();
            cout << COLOR_BORDER << "*------------------------*\n" << COLOR_RESET;
            cout << COLOR_MENUS << "|        MAIN MENU       |\n" << COLOR_RESET;
            cout << COLOR_BORDER << "|                        |\n" << COLOR_RESET;
            cout << COLOR_TEXT << "|1. " << COLOR_SAFE << "Play Game" << COLOR_TEXT << "            |\n";
            cout << "|2. " << COLOR_TREASURE << "Instructions" << COLOR_TEXT << "         |\n";
            cout << "|3. " << COLOR_MENUS << "Credit" << COLOR_TEXT << "               |\n";
            cout << "|4. " << COLOR_TRAP << "Exit" << COLOR_TEXT << "                 |\n";
            cout << COLOR_BORDER << "|                        |\n";
            cout << "|Enter choice:           |\n";
            cout << "|                        |\n";
            cout << "*------------------------*\n" << COLOR_RESET;
            choice = _getch();
            switch (choice) {
            case '1': showLevelMenu(); break;
            case '2': showInstructions(); break;
            case '3': credit(); break;
            case '4': return;
            }
        } while (true);
    }

    void showLevelMenu() {
        char choice;
        do {
            clearScreen();
            cout << COLOR_BORDER << "*----------------------*\n" << COLOR_RESET;
            cout << COLOR_MENUS << "|     SELECT LEVEL     |\n" << COLOR_RESET;
            cout << COLOR_BORDER << "|                      |\n" << COLOR_RESET;
            cout << COLOR_TEXT << "|1. " << COLOR_SAFE << "Easy (5x5)" << COLOR_TEXT << "         |\n";
            cout << "|2. " << COLOR_TREASURE << "Medium (6x6)" << COLOR_TEXT << "       |\n";
            cout << "|3. " << COLOR_TRAP << "Hard (7x7)" << COLOR_TEXT << "         |\n";
            cout << "|4. " << COLOR_TEXT << "Back" << "               |\n";
            cout << COLOR_BORDER << "|                      |\n";
            cout << "|Enter choice :        |\n";
            cout << "|                      |\n";
            cout << "*----------------------*\n" << COLOR_RESET;
            choice = _getch();

            if (choice >= '1' && choice <= '3') {
                difficulty = choice - '0';
                switch (difficulty) {
                case 1: gridSize = EASY_SIZE; maxHealth = health = 100; timeLimit = 70; break;
                case 2: gridSize = MEDIUM_SIZE; maxHealth = health = 80; timeLimit = 60; break;
                case 3: gridSize = HARD_SIZE; maxHealth = health = 70; timeLimit = 50; break;
                }

                cleanupGrid();
                initializeGrid();
                score = 0;
                treasuresFound = 0;
                clearScreen();
                cout << COLOR_BORDER << "*-------------" << COLOR_MENUS << "Treasure Hunt" << COLOR_BORDER << "------------*\n" << COLOR_RESET << endl;
                cout << COLOR_TEXT << "Enter player name: " << COLOR_MENUS;
                getline(cin, playerName);
                playGame();
            }
            else if (choice == '4') {
                return;
            }
        } while (true);
    }
};

int main() {
    TreasureHuntGame game;
    game.showMainMenu();
    return 0;
}