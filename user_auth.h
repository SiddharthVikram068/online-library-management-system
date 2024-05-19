#include<iostream>
#include<fstream>
#include<string>

using namespace std;

extern string userPagename;

string user_db_search(const string& filepath, const string& username);
int user_authentication(int clientSocket);