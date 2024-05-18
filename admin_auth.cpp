#include<iostream>
#include<fstream>
#include<string>
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include<ios>
#include<limits>
#include "admin_auth.h"

using namespace std;

// linear search find
string admin_db_search(const string& filepath, const string& username) {
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

int admin_authentication(){

    string username = "", password= "";
    string printString = ""; // this string takes care if the server wants to print anything to the client

    cout << "Please enter your username and password for admin login\n\n";

    cout << "Username: ";
    cin >> username;

    cout << "Password: ";

    // used terminal structures so that password can be invisible while input
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    cin >> password;
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    cout << "\n\n";

    string filepath = "auth_db/admin_db.txt";

    string result = admin_db_search(filepath, username);

    if(result.empty()){
        cout << "no match is found or an error occured\n";
    }
    else{
        if(password == result){
            cout << "Authentication successful\n";

        }

        else{
            cout << "Wrong password\n";
        }
    }

    sleep(3);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return 0;

}