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




//! client side of socket programming (string transfer)
// #include <iostream>
// #include <string>
// #include <cstring>
// #include <unistd.h>
// #include <arpa/inet.h>

// const char* SERVER_IP = "127.0.0.1";
// const int SERVER_PORT = 8080;
// const int BUFFER_SIZE = 1024;

// int main() {
//     int clientSocket;
//     struct sockaddr_in serverAddr;
//     char buffer[BUFFER_SIZE];

//     // Create socket
//     clientSocket = socket(AF_INET, SOCK_STREAM, 0);
//     if (clientSocket == -1) {
//         std::cerr << "Failed to create socket." << std::endl;
//         return 1;
//     }

//     // Define server address
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_port = htons(SERVER_PORT);
//     if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
//         std::cerr << "Invalid address or address not supported." << std::endl;
//         close(clientSocket);
//         return 1;
//     }

//     // Connect to server
//     if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
//         std::cerr << "Connection to server failed." << std::endl;
//         close(clientSocket);
//         return 1;
//     }

//     std::cout << "Connected to server at " << SERVER_IP << ":" << SERVER_PORT << std::endl;

//     while (true) {
//         std::cout << "Enter message: ";
//         std::string message;
//         std::getline(std::cin, message);

//         if (message == "exit") {
//             break;
//         }

//         send(clientSocket, message.c_str(), message.size(), 0);

//         memset(buffer, 0, BUFFER_SIZE);
//         int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
//         if (bytesRead <= 0) {
//             std::cerr << "Server disconnected or error occurred." << std::endl;
//             break;
//         }

//         std::cout << "Server response: " << buffer << std::endl;
//     }

//     close(clientSocket);
//     return 0;
// }



//! server side of socket programming(string transfer)
// #include <iostream>
// #include <string>
// #include <thread>
// #include <vector>
// #include <cstring>
// #include <unistd.h>
// #include <arpa/inet.h>

// const int PORT = 8080;
// const int BUFFER_SIZE = 1024;


// void handleClient(int clientSocket) {
//     char buffer[BUFFER_SIZE];
//     int bytesRead;

//     while (true) {
//         memset(buffer, 0, BUFFER_SIZE);
//         bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);

//         if (bytesRead <= 0) {
//             std::cout << "Client disconnected or error occurred." << std::endl;
//             break;
//         }

//         std::cout << "Received message: " << buffer << std::endl;

//         std::string response = "Server received: " + std::string(buffer);
//         send(clientSocket, response.c_str(), response.size(), 0);
//     }

//     close(clientSocket);
// }


// int main() {
//     int serverSocket, clientSocket;
//     struct sockaddr_in serverAddr, clientAddr;
//     socklen_t clientAddrLen = sizeof(clientAddr);

//     // Create socket
//     serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//     if (serverSocket == -1) {
//         std::cerr << "Failed to create socket." << std::endl;
//         return 1;
//     }

//     // Bind socket to port
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_addr.s_addr = INADDR_ANY;
//     serverAddr.sin_port = htons(PORT);

//     if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
//         std::cerr << "Bind failed." << std::endl;
//         close(serverSocket);
//         return 1;
//     }

//     // Listen for incoming connections
//     if (listen(serverSocket, 10) == -1) {
//         std::cerr << "Listen failed." << std::endl;
//         close(serverSocket);
//         return 1;
//     }

//     std::cout << "Server listening on port " << PORT << std::endl;

//     while (true) {
//         // Accept a new connection
//         clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
//         if (clientSocket == -1) {
//             std::cerr << "Failed to accept connection." << std::endl;
//             continue;
//         }

//         std::cout << "Client connected." << std::endl;

//         // Spawn a new thread to handle the client
//         std::thread clientThread(handleClient, clientSocket);
//         clientThread.detach();  // Detach the thread to allow it to run independently
//     }

//     close(serverSocket);
//     return 0;
// }
