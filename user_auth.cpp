#include<iostream>
#include<fstream>
#include<string>
#include "user_auth.h"

using namespace std;

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

int user_authentication(){

    string username = "", password= "";

    cout << "Username: ";
    cin >> username;

    cout << "Password: ";
    cin >> password;

    string filepath = "auth_db/user_db.txt";

    string result = user_db_search(filepath, username);

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

    return 0;

}