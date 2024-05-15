//! Interactive menu with up, down and enter options 

// #include <iostream>
// #include <vector>
// #include <string>


// #ifdef _WIN32
//     #include <conio.h>
//     #include <windows.h>

//     void clearScreen() {
//         HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
//         COORD Position = {0, 0};
//         SetConsoleCursorPosition(hOut, Position);
//     }

//     int getKeyPress() {
//         return _getch();
//     }

// #else
//     #include <termios.h>
//     #include <unistd.h>
//     #include <cstdio>

//     void clearScreen() {
//         std::cout << "\033[2J\033[1;1H";
//     }

//     int getKeyPress() {
//         struct termios oldt, newt;
//         int ch;
//         tcgetattr(STDIN_FILENO, &oldt);
//         newt = oldt;
//         newt.c_lflag &= ~(ICANON | ECHO);
//         tcsetattr(STDIN_FILENO, TCSANOW, &newt);
//         ch = getchar();
//         tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
//         return ch;
//     }
// #endif

// void printMenu(const std::vector<std::string>& choices, int highlight) {
//     for (size_t i = 0; i < choices.size(); ++i) {
//         if (i == highlight) {
//             std::cout << "> " << choices[i] << " <" << std::endl;
//         } else {
//             std::cout << "  " << choices[i] << std::endl;
//         }
//     }
// }

// int main() {
//     std::vector<std::string> choices = {
//         "Option 1",
//         "Option 2",
//         "Option 3",
//         "Option 4",
//         "Exit"
//     };
//     int highlight = 0;
//     int choice = -1;

//     while (true) {
//         clearScreen();
//         printMenu(choices, highlight);

//         int c = getKeyPress();
//         if (c == '\033') { // handle arrow keys on Unix-like systems
//             getKeyPress(); // skip the [
//             switch (getKeyPress()) {
//                 case 'A': // Up arrow
//                     if (highlight > 0) {
//                         --highlight;
//                     } else {
//                         highlight = choices.size() - 1;
//                     }
//                     break;
//                 case 'B': // Down arrow
//                     if (highlight < choices.size() - 1) {
//                         ++highlight;
//                     } else {
//                         highlight = 0;
//                     }
//                     break;
//             }
//         } else if (c == 224) { // handle arrow keys on Windows
//             switch (getKeyPress()) {
//                 case 72: // Up arrow
//                     if (highlight > 0) {
//                         --highlight;
//                     } else {
//                         highlight = choices.size() - 1;
//                     }
//                     break;
//                 case 80: // Down arrow
//                     if (highlight < choices.size() - 1) {
//                         ++highlight;
//                     } else {
//                         highlight = 0;
//                     }
//                     break;
//             }
//         } else if (c == '\r' || c == '\n') { // Enter key
//             choice = highlight;
//             break;
//         }
//     }

//     std::cout << "You chose: " << choices[choice] << std::endl;
//     return 0;
// }


//! returning second word of line in file if first word matches 

// #include <iostream>
// #include <fstream>
// #include <string>

// using namespace std;

// string searchSecondWord(const string& filepath, const string& inputWord) {
//     ifstream file(filepath);
//     string firstWord, secondWord;

//     if (!file.is_open()) {
//         cerr << "Error opening file!" << endl;
//         return "";
//     }

//     while (file >> firstWord >> secondWord) {
//         if (firstWord == inputWord) {
//             file.close();
//             return secondWord;
//         }
//     }

//     file.close();
//     return ""; // Return empty string if no match is found
// }

// int main() {
//     string folder = "auth_db";
//     string filename = "user_db.txt";
//     string filepath = folder + "/" + filename;
//     string inputWord;

//     cout << "Enter the first word to search for: ";
//     cin >> inputWord;

//     string result = searchSecondWord(filepath, inputWord);
    
//     if (!result.empty()) {
//         cout << "The second word is: " << result << endl;
//     } else {
//         cout << "No match found." << endl;
//     }

//     return 0;
// }


