#include<iostream>
#include<fstream>
#include<string>
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include<ios>
#include<limits>
#include "user_auth.h"
#include<cstring>
#include<arpa/inet.h>

using namespace std;

string userPagename;

// linear search find
string user_db_search(const string& filepath, const string& username) {
    ifstream file(filepath);
    string inputUser, userPassword;

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return "";
    }

    while (file >> inputUser >> userPassword) {
        if (inputUser == username) {
            file.close();
            return userPassword;
        }
    }

    file.close();
    return ""; // Return empty string if no match is found
}

int user_authentication(int clientSocket){

    char usernameBuf[1024];
    char passwordBuf[1024];
    memset(usernameBuf, 0, 1024);
    memset(passwordBuf, 0, 1024);
    int state = 0;

    string printString = ""; // if server wants to print anything to the client, this string can be used

    // string username = "", password= "";
        // taking username and password from the client from sockets 
    int bytesRead = recv(clientSocket, usernameBuf, 1024, 0);
    string username(usernameBuf, bytesRead);
    userPagename = username;
    cout << "Username recieved is " << username;
    if(bytesRead <= 0){
        cout << "error in authentication\n";
        state = 3;
        
    }

    bytesRead = recv(clientSocket, passwordBuf, 1024, 0);
    string password(passwordBuf, bytesRead);
    cout << "password recieved is " << password;
    if(bytesRead <= 0){
        cout << "error in authentication\n";
        state = 3;
    }

    // cout << "Please enter your username and password for member login\n\n";

    // cout << "Username: ";
    // cin >> username;

    // cout << "Password: ";

    // // used terminal structures so that password can be invisible while input 
    // struct termios oldt, newt;
    // tcgetattr(STDIN_FILENO, &oldt);
    // newt = oldt;
    // newt.c_lflag &= ~(ECHO);
    // tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    // cin >> password;
    // tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    // cout << "\n\n";

    string filepath = "auth_db/user_db.txt";

    string result = user_db_search(filepath, username);

    if(result.empty()){
        cout << "no match is found or an error occured\n";
    }
    else{
        if(password == result){
            cout << "Authentication successful\n";
            state = 5;
        }

        else{
            cout << "Wrong password\n";
            state = 3;
        }
    }

    // sleep(3);

    // cin.ignore(numeric_limits<streamsize>::max(), '\n');

    uint32_t sendingState = htonl(state);

    send(clientSocket, &sendingState, sizeof(sendingState), 0);

    return state;

}