#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include<termios.h>
#include<vector>
#include<ios>
#include<limits>
#include<stdint.h>
#include<mutex>

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
    cout << "\n\n";
    for (size_t i = 0; i < choices.size(); ++i) {
        if (i == highlight) {
            cout << "> " << choices[i] << " <" << endl;
        } else {
            cout << "  " << choices[i] << endl;
        }
    }
}

void printMenuHor(const vector<string>& choices, int highlight) {
    cout << "\n\n";
    for (size_t i = 0; i < choices.size(); ++i) {
        cout << "\t\t\t";
        if (i == highlight) {
            cout << "> " << choices[i] << " <";
        } else {
            cout << "  " << choices[i];
        }
    }
}

void printMenuHor2(const vector<string>& choices, int highlight) {
    cout << "\n\n";
    for (size_t i = 0; i < choices.size(); ++i) {
        cout << "\t";
        if (i == highlight) {
            cout << "> " << choices[i] << " <";
        } else {
            cout << "  " << choices[i];
        }
    }
}

string convertToString(char* a, int size)
{
    string s(a, size);
    return s;
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

int adminAuthClient(int clientSocket){
    cout << "\n\n";
    cout << "Welcome to administrator login portal\n\n";
    string username = "", password="";
    cout << "username: ";
    cin >> username;

    send(clientSocket, username.c_str(), username.length(), 0);

    cout << "password: ";
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    cin >> password;
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    send(clientSocket, password.c_str(), password.length(), 0);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    uint32_t buffer; 
    int bytesRead = recv(clientSocket, &buffer, sizeof(buffer), 0);
    cout << "\n\n";
    if(bytesRead <= 0){
        cout << "Server and client communication broken";
        return 3;
    }
    int state = ntohl(buffer);
    if(state == 3){
        cout << "Wrong password or no match found, going back to the access menu\n\n";
    }

    else if(state == 4){
        cout << "Authentication Successful\n\n";
    }
    return state;
}

int userAuthClient(int clientSocket){
    cout << "\n\n";
    cout << "Welcome to User login portal\n\n";
    string username = "", password="";
    cout << "username: ";
    cin >> username;

    send(clientSocket, username.c_str(), username.length(), 0);

    cout << "password: ";
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    cin >> password;
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    send(clientSocket, password.c_str(), password.length(), 0);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    uint32_t buffer; 
    int bytesRead = recv(clientSocket, &buffer, sizeof(buffer), 0);
    cout << "\n\n";
    if(bytesRead <= 0){
        cout << "Server and client communication broken";
        return 3;
    }
    int state = ntohl(buffer);
    if(state == 3){
        cout << "Wrong password or no match found, going back to the access menu\n\n";
    }

    else if(state == 5){
        cout << "Authentication Successful\n\n";
    }
    return state;
}

int adminHomepageClient(int clientSocket){


    // recieving admin info from server
    vector<string> info(2, "");
    char nameBuf[1024];
    char dobBuf[1024];

    int length = 0;
    uint32_t recieveLength;

    recv(clientSocket, &recieveLength, sizeof(recieveLength), 0);
    length = ntohl(recieveLength);
    int bytesRead = recv(clientSocket, nameBuf, length, 0);
    info[0] = convertToString(nameBuf, length);

    cout << info[0] << endl;
    
    recv(clientSocket, &recieveLength, sizeof(recieveLength), 0);
    length = ntohl(recieveLength);
    bytesRead = recv(clientSocket, dobBuf, length, 0);
    info[1] = convertToString(dobBuf, length);


    // printing the menu and info 
    vector<string> choices = {
        "Logout",
        "Add Book",
        "Delete Book",
        "Search book",
        "Add user"
    };

    int highlight = 0;
    int choice = -1;

    while(true){
        clearScreen();

        // printing admin info
        for(int i = 0; i<2; i++){
            if(i == 0) cout << "Name: ";
            else if(i == 1) cout << "DOB: ";

            cout << info[i] << endl;
        }
        
        cout << "\n\n";

        printMenuHor2(choices, highlight);

        int c = getKeyPress();

        if (c == '\033') { // handle arrow keys on Unix-like systems
            getKeyPress(); // skip the [
            switch (getKeyPress()) {
                case 'D': // left arrow
                    if (highlight > 0) {
                        --highlight;
                    } else {
                        highlight = choices.size() - 1;
                    }
                    break;
                case 'C': // right arrow
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

    int actionClient = 9 + choice;
    uint32_t sendingAction = htonl(actionClient);

    // actions based upon the option selected
    if(choice == 0){
        send(clientSocket, &sendingAction, sizeof(sendingAction), 0);
        return 3;
    }
    // add book
    else if(choice == 1){
        cout << "\n\n";
        cout << "Name the book to add: ";
        string bookToAdd; cin >> bookToAdd;
        send(clientSocket, &sendingAction, sizeof(sendingAction), 0);
        send(clientSocket, bookToAdd.c_str(), bookToAdd.length(), 0);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    }

    else if(choice == 2){
        cout << "\n\n";
        cout << "Name the book to delete: ";
        string bookToDelete; cin >> bookToDelete;
        send(clientSocket, &sendingAction, sizeof(sendingAction), 0);
        send(clientSocket, bookToDelete.c_str(), bookToDelete.length(), 0);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    else if(choice == 3){
        cout << "\n\n";
        cout << "Name the book to search: ";
        string bookToSearch; cin >> bookToSearch;
        send(clientSocket, &sendingAction, sizeof(sendingAction), 0);
        send(clientSocket, bookToSearch.c_str(), bookToSearch.length(), 0);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        char bookInfo[1024];
        uint32_t some_length;
        recv(clientSocket, &some_length, sizeof(some_length), 0);
        int infoLength = ntohl(some_length);
        recv(clientSocket, bookInfo, infoLength, 0);
        string book_info(bookInfo, infoLength);

        cout << book_info << endl;
        sleep(5);


    }

    // modify user
    else if(choice == 4){
        cout << "\n\n";

        send(clientSocket, &sendingAction, sizeof(sendingAction), 0);

        vector<string> modifyInfo(4, "");
        cout << "Enter the name of the user you want to change: "; cin >> modifyInfo[0];
        cout << "Enter the new username: "; cin >> modifyInfo[1];
        cout << "Enter the new DOB: "; cin >> modifyInfo[2];
        cout << "Enter new number of books borrowed: "; cin >> modifyInfo[3];

        uint32_t sendingLength = htonl(modifyInfo[0].length());
        send(clientSocket, &sendingLength, sizeof(sendingLength), 0);
        send(clientSocket, modifyInfo[0].c_str(), modifyInfo[0].length(), 0);


        sendingLength = htonl(modifyInfo[1].length());
        send(clientSocket, &sendingLength, sizeof(sendingLength), 0);
        send(clientSocket, modifyInfo[1].c_str(), modifyInfo[1].length(), 0);
        
        
        sendingLength = htonl(modifyInfo[2].length());
        send(clientSocket, &sendingLength, sizeof(sendingLength), 0);
        send(clientSocket, modifyInfo[2].c_str(), modifyInfo[2].length(), 0);


        sendingLength = htonl(modifyInfo[3].length());
        send(clientSocket, &sendingLength, sizeof(sendingLength), 0);
        send(clientSocket, modifyInfo[3].c_str(), modifyInfo[3].length(), 0);

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return 4;

}

int userHomepageClient(int clientSocket){

    // recieving user info from server and displaying it 
    vector<string> info(3, "");
    char nameBuf[1024];
    char dobBuf[1024];
    char bbBuf[1024];

    int length = 0;
    uint32_t recieveLength;

    recv(clientSocket, &recieveLength, sizeof(recieveLength), 0);
    length = ntohl(recieveLength);
    int bytesRead = recv(clientSocket, nameBuf, length, 0);
    info[0] = convertToString(nameBuf, length);
    
    recv(clientSocket, &recieveLength, sizeof(recieveLength), 0);
    length = ntohl(recieveLength);
    bytesRead = recv(clientSocket, dobBuf, length, 0);
    info[1] = convertToString(dobBuf, length);

    recv(clientSocket, &recieveLength, sizeof(recieveLength), 0);
    length = ntohl(recieveLength);
    bytesRead = recv(clientSocket, bbBuf, length, 0);
    info[2] = convertToString(bbBuf, length);
    

    // logout and search menu
    vector<string> choices = {
        "Logout",
        "Search for a book"
    };

    int highlight = 0;
    int choice = -1;


    while(true){
        clearScreen();

        cout << "\n\n";

        // user info printing 
        for(int i = 0; i<3; i++){
            if(i == 0) cout << "Name: ";
            else if(i == 1) cout << "DOB: ";
            else cout << "Number of Books Borrowed: ";

            cout << info[i] << endl;
        }
        
        cout << "\n\n";


        // printing menu for choices 
        printMenuHor(choices, highlight);

        int c = getKeyPress();

        if (c == '\033') { // handle arrow keys on Unix-like systems
            getKeyPress(); // skip the [
            switch (getKeyPress()) {
                case 'D': // left arrow
                    if (highlight > 0) {
                        --highlight;
                    } else {
                        highlight = choices.size() - 1;
                    }
                    break;
                case 'C': // right arrow
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
        // clearScreen();
    }

    // actions based upon logout or search
    if(choice == 0){
        return 3;
    }
    else{
        // book database search
        return 3;
    }
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
            cout << "clientState is " << clientState << endl;
            break;
        }

        if(clientState == 3){
            clientState = accessMenuClient(clientSocket);
            cout << "clientState is " << clientState << endl;
        }

        if(clientState == 1){
            clientState = adminAuthClient(clientSocket);
            cout << "clientState is " << clientState << endl;
        }

        if(clientState == 0){
            clientState = userAuthClient(clientSocket);
            cout << "clientState is " << clientState << endl;
        }

        if(clientState ==5){
            cout << "user Home page\n\n";
            clientState = userHomepageClient(clientSocket);
            cout << "clientState is " << clientState << endl;
        }
        

        // temporary code before implementing user and admin pages 
        if(clientState == 4){
            cout << "Admin Home page\n\n";
            clientState = adminHomepageClient(clientSocket);
            cout << "clientState is " << clientState << endl;
        }

    }

    close(clientSocket);
    return 0;
}
