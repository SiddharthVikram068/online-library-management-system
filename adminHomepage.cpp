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
#include "adminHomepage.h"

using namespace std;

/*
info[0] - name
info[1] - DOB
*/

vector<string> getAdminInfo(const string& filepath, const string& adminPagename){

    ifstream file(filepath);
    string info[2];
    vector<string> out(2, "");

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return out;
    }

    while(file >> info[0] >> info[1]){
        if(info[0] == adminPagename){
            out[0] = info[0];
            out[1] = info[1];
            file.close();

        }
    }
    
    return out;

}

int adminHomepageServer(int clientSocket, string adminPagename){
    int state = 4;
    string filepath = "homepage_db/admin_pages.txt";
    vector<string> adminInfo = getAdminInfo(filepath, adminPagename);

    for(auto i : adminInfo){
        cout << i << " ";
    }
    cout << "\n\n";

    uint32_t sendingLength = htonl(adminInfo[0].length());
    send(clientSocket, &sendingLength, sizeof(sendingLength), 0);
    send(clientSocket, adminInfo[0].c_str(), adminInfo[0].length(), 0);


    sendingLength = htonl(adminInfo[1].length());
    send(clientSocket, &sendingLength, sizeof(sendingLength), 0);
    send(clientSocket, adminInfo[1].c_str(), adminInfo[1].length(), 0);

    return 3;
}