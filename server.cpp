#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "user_auth.h"
#include "admin_auth.h"
#include "userHomepage.h"
#include "adminHomepage.h"
#include<stdint.h>

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

using namespace std;

int accessMenu(int clientSocket){
    uint32_t buffer;
    int choice=0;

    choice = recv(clientSocket, &buffer, sizeof(buffer), 0);

    if (choice <= 0) {
        cout << "Client disconnected or error occurred." << endl;
        return 2;
    }

    int number = ntohl(buffer);

    return number;
}


void handleClient(int clientSocket) {

    // let's make a state based protocol, a number will represent our state 
    /*
    0 - User authentication
    1 - Admin authentication
    2 - Client exit 
    3 - Access menu
    */


   int state = 3;

    while (true) {

        if(state == 2){
            cout << "state of server is " << state << endl;
            break;
        }

        if(state == 3){
            // cout << "entered access menu\n\n";
            state = accessMenu(clientSocket);
            cout << "state of server is " << state << endl;
        }

                

        if(state == 1){
            state = admin_authentication(clientSocket);
            cout << adminPagename << "\n\n";
            cout << "state of server is " << state << endl;
        }

        

        if(state == 0){
            state = user_authentication(clientSocket);
            cout << userPagename << "\n\n";
            cout << "state of server is " << state << endl;
        }

        

        if(state == 5){
            cout << "User Home page\n\n";
            state = userHomepageServer(clientSocket, userPagename);
            cout << "state of server is " << state << endl;
        }

        

        if(state == 4){
            cout << "Admin Home page\n\n";
            state = adminHomepageServer(clientSocket, adminPagename);
            cout << "state of server is " << state << endl;
        }

        

    }

    close(clientSocket);
}


int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cerr << "Failed to create socket." << endl;
        return 1;
    }

    // Bind socket to port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        cerr << "Bind failed." << endl;
        close(serverSocket);
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 10) == -1) {
        cerr << "Listen failed." << endl;
        close(serverSocket);
        return 1;
    }

    cout << "Server listening on port " << PORT << endl;

    while (true) {
        // Accept a new connection
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            cerr << "Failed to accept connection." << endl;
            continue;
        }

        cout << "Client connected." << endl;

        // Spawn a new thread to handle the client
        thread clientThread(handleClient, clientSocket);
        clientThread.detach();  // Detach the thread to allow it to run independently
    }

    close(serverSocket);
    return 0;
}
