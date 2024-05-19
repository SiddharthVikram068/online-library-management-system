#include<string>

using namespace std;
void addBook(const string& filepath, string searchBookName);
void deleteBook(const string& filepath, string searchBookName);
string searchBook(const string& filepath, string searchBookName);
void modifyUser(string user, string newUser, string newDob, string newNumberOfBooksBorrowed);