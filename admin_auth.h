#include<iostream>
#include<fstream>
#include<string>
#include <termios.h>
#include <unistd.h>
#include <cstdio>

using namespace std;

string admin_db_search(const string& filepath, const string& username);
int admin_authentication();
