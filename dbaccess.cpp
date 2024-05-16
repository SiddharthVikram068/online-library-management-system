#include <iostream>
#include <vector>
#include <string>
#include "user_auth.h"
#include "admin_auth.h"

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>

    void clearScreen() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD Position = {0, 0};
        SetConsoleCursorPosition(hOut, Position);
    }

    int getKeyPress() {
        return _getch();
    }

#else
    #include <termios.h>
    #include <unistd.h>
    #include <cstdio>

    void clearScreen() {
        std::cout << "\033[2J\033[1;1H";
    }

    int getKeyPress() {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;

        // input char by char instead of line by line and input not echoed back to terminal
        newt.c_lflag &= ~(ICANON | ECHO);  
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#endif

using namespace std;

void printMenu(const vector<string>& choices, int highlight) {
    for (size_t i = 0; i < choices.size(); ++i) {
        if (i == highlight) {
            cout << "> " << choices[i] << " <" << endl;
        } else {
            cout << "  " << choices[i] << endl;
        }
    }
}



int main() {
    cout << "Welcome to Online Library Management system\n";
    vector<string> choices = {
        "User",
        "Admin",
        "Exit"
    };
    int highlight = 0;
    int choice = -1;

    while (true) {
        clearScreen();
        printMenu(choices, highlight);

        int c = getKeyPress();
        if (c == '\033') { // handle arrow keys on Unix-like systems
            getKeyPress(); // skip the [
            switch (getKeyPress()) {
                case 'A': // Up arrow
                    if (highlight > 0) {
                        --highlight;
                    } else {
                        highlight = choices.size() - 1;
                    }
                    break;
                case 'B': // Down arrow
                    if (highlight < choices.size() - 1) {
                        ++highlight;
                    } else {
                        highlight = 0;
                    }
                    break;
            }
        } else if (c == 224) { // handle arrow keys on Windows
            switch (getKeyPress()) {
                case 72: // Up arrow
                    if (highlight > 0) {
                        --highlight;
                    } else {
                        highlight = choices.size() - 1;
                    }
                    break;
                case 80: // Down arrow
                    if (highlight < choices.size() - 1) {
                        ++highlight;
                    } else {
                        highlight = 0;
                    }
                    break;
            }
        } else if (c == '\r' || c == '\n') { // Enter key
            choice = highlight;
        }

        if(choice != -1 && choices[choice] == "Exit"){
            return 0;
        }

        else if(choice != -1 && choices[choice] == "User"){
            user_authentication();
            choice = -1;
        }

        else if(choice != -1 && choices[choice] == "Admin"){
            admin_authentication();
            choice = -1;
        }
    }

    // cout << "You chose: " << choices[choice] << endl;



    return 0;
}
