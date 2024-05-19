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
#include<mutex>

using namespace std;

mutex user_auth;
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


    string filepath = "auth_db/user_db.txt";

    user_auth.lock();
    string result = user_db_search(filepath, username);
    user_auth.unlock();

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

    uint32_t sendingState = htonl(state);

    send(clientSocket, &sendingState, sizeof(sendingState), 0);

    return state;

}