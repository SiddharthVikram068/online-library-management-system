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
#include<sstream>
#include<mutex>
#include "actions.h"

using namespace std;

// locks for files 
mutex auth_lock;
mutex page_lock;
mutex book_lock;

/*
in the books database, the data is structured as

book_name number_of_copies

*/

void addBook(const string& filepath, string searchBookName){

    book_lock.lock();
    ifstream inputFile(filepath);
    vector<string> lines;
    string line;
    bool matchFound = false;

    // Read the file and store lines
    while (getline(inputFile, line)) {
        istringstream iss(line);
        string bookname, copies;
        iss >> bookname >> copies;

        if (bookname == searchBookName) {
            // Update the third word
            string newcopies = to_string(stoi(copies) + 1);
            lines.push_back(bookname + " " + newcopies);
            matchFound = true;
        } else {
            // Keep the line as is
            lines.push_back(line);
        }
    }
    inputFile.close();

    // If no match was found, add a new entry
    if (!matchFound) {
        lines.push_back(searchBookName + " " + "1");
    }

    // Write the lines back to the file
    ofstream outputFile(filepath);
    for (const auto& updatedLine : lines) {
        outputFile << updatedLine << endl;
    }
    outputFile.close();

    book_lock.unlock();

}

void deleteBook(const string& filepath, string searchBookName){

    book_lock.lock();

    ifstream inputFile(filepath);
    vector<string> lines;
    string line;
    bool matchFound = false;

    // Read the file and store lines
    while (getline(inputFile, line)) {
        istringstream iss(line);
        string bookname, copies;
        iss >> bookname >> copies;

        if (bookname == searchBookName) {
            // Update the third word
            string newcopies = to_string(stoi(copies) - 1);
            if(stoi(newcopies) > 0) lines.push_back(bookname + " " + newcopies);
            matchFound = true;
        } else {
            // Keep the line as is
            lines.push_back(line);
        }
    }
    inputFile.close();

    // If no match was found, add a new entry
    if (!matchFound) {
        cout << "No such book exists";
    }

    // Write the lines back to the file
    ofstream outputFile(filepath);
    for (const auto& updatedLine : lines) {
        outputFile << updatedLine << endl;
    }
    outputFile.close();

    book_lock.unlock();

}

string searchBook(const string& filepath, string searchBookName){

    book_lock.lock();

    ifstream file(filepath);
    string bookName, numberOfCopies;

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return "";
    }

    while(file >> bookName >> numberOfCopies){
        if(bookName == searchBookName){
            file.close();
            return bookName + " " + numberOfCopies;
        }
    }

    file.close();
    return "Book not found";

    book_lock.unlock();
}

void modifyUser(string user, string newUser, string newDob, string newNumberOfBooksBorrowed){


    string filepath = "homepage_db/user_pages.txt";
    string auth_file = "auth_db/user_db.txt";

    page_lock.lock();
    // updating the homepage file
    ifstream homepage(filepath);
    vector<string> lines;
    string line;
    bool matchFound = false;

    while (getline(homepage, line)) {
        istringstream iss(line);
        string userName, dob, num;
        iss >> userName >> dob >> num;

        if (userName == user) {
            lines.push_back(newUser + " " + newDob + " " + newNumberOfBooksBorrowed);
            matchFound = true;
        } else {
            lines.push_back(line);
        }
    }
    homepage.close();

    if (!matchFound) {
        cout << "No such user exists";
    }

    ofstream outputPageFile(filepath);
    for (const auto& updatedLine : lines) {
        outputPageFile << updatedLine << endl;
    }
    outputPageFile.close();

    page_lock.unlock();

    // updating the authentication file 

    auth_lock.lock();
    vector<string> auth_lines;
    string auth_line;
    ifstream authFile(auth_file);
    matchFound = false;

        while (getline(homepage, auth_line)) {
        istringstream iss(auth_line);
        string userName, password;
        iss >> userName >> password;

        if (userName == user) {
            auth_lines.push_back(newUser + " " + password);
            matchFound = true;
        } else {
            // Keep the line as is
            auth_lines.push_back(auth_line);
        }
    }
    authFile.close();

    if (!matchFound) {
        cout << "No such user exists";
    }

    // Write the auth_lines back to the file
    ofstream outputAuthFile(auth_file);
    for (const auto& updatedLine : auth_lines) {
        outputAuthFile << updatedLine << endl;
    }
    outputAuthFile.close();

    auth_lock.unlock();


}
