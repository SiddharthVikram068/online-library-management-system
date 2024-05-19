#include<iostream>
#include<fstream>
#include<string>
#include <termios.h>
#include <unistd.h>
#include <cstdio>

using namespace std;

extern string adminPagename;

string admin_db_search(const string& filepath, const string& username);
int admin_authentication(int clientSocket);
