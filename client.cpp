#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include<termios.h>
#include<vector>
#include<stdint.h>

const char* SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 8080;
const int BUFFER_SIZE = 1024;

using namespace std;

void clearScreen() {
    cout << "\033[2J\033[1;1H";
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

void printMenu(const vector<string>& choices, int highlight) {
    for (size_t i = 0; i < choices.size(); ++i) {
        if (i == highlight) {
            cout << "> " << choices[i] << " <" << endl;
        } else {
            cout << "  " << choices[i] << endl;
        }
    }
}

int accessMenuClient(int clientSocket){

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
            break;
        }

    }

    uint32_t menuChoice = htonl(choice);
    send(clientSocket, &menuChoice, sizeof(menuChoice), 0);
    clearScreen();

    return choice;
    
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];


    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        cerr << "Failed to create socket." << endl;
        return 1;
    }

    // Define server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        cerr << "Invalid address or address not supported." << endl;
        close(clientSocket);
        return 1;
    }

    // Connect to server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        cerr << "Connection to server failed." << endl;
        close(clientSocket);
        return 1;
    }

    cout << "Connected to server at " << SERVER_IP << ":" << SERVER_PORT << endl;

    // let's make a state based protocol, a number will represent our state 
    /*
    0 - User authentication
    1 - Admin authentication
    2 - Client exit 
    3 - Access menu
    */

    int clientState = 3;

    while(true){
        
        if(clientState == 2){
            break;
        }

        if(clientState != 2){
            clientState = accessMenuClient(clientSocket);
        }

        cout << "clientState is " << clientState << endl;

    }

    close(clientSocket);
    return 0;
}
