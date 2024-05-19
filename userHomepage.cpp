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
#include<fstream>
#include<arpa/inet.h>
#include<mutex>
#include "userHomepage.h"

using namespace std;

mutex userPages;
/*
info[0] - name
info[1] - DOB
info[2] - number of books borrowed 
*/

vector<string> getInfo(const string& filepath, const string& userPagename){

    ifstream file(filepath);
    string info[3];
    vector<string> out(3, "");

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return out;
    }

    while(file >> info[0] >> info[1] >> info[2]){
        if(info[0] == userPagename){
            out[0] = info[0];
            out[1] = info[1];
            out[2] = info[2];
            file.close();
        }
    }
    
    return out;

}

int userHomepageServer(int clientSocket, string userPagename){
    int state = 5;
    userPages.lock();
    string filepath = "homepage_db/user_pages.txt";
    vector<string> userInfo = getInfo(filepath, userPagename);
    userPages.unlock();

    /*
    since we are sending multiple strings, we have to establish a protocol,
    first we will send the length of a string before sending the string 
    */ 
    uint32_t sendingLength = htonl(userInfo[0].length());
    send(clientSocket, &sendingLength, sizeof(sendingLength), 0);
    send(clientSocket, userInfo[0].c_str(), userInfo[0].length(), 0);


    sendingLength = htonl(userInfo[1].length());
    send(clientSocket, &sendingLength, sizeof(sendingLength), 0);
    send(clientSocket, userInfo[1].c_str(), userInfo[1].length(), 0);

    sendingLength = htonl(userInfo[2].length());
    send(clientSocket, &sendingLength, sizeof(sendingLength), 0);
    send(clientSocket, userInfo[2].c_str(), userInfo[2].length(), 0);


    return 3;
}

