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
#include "book_db_utils.h"

using namespace std;

/*
in the books database, the data is structured as

book_name number_of_copies

*/

void addBook(const string& filepath, string searchBookName){

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

}

int main(){
    string filepath = "book_db/books.txt";
    for(int i = 0; i<5; i++){
        string bookname;
        cin >> bookname;
        addBook(filepath, bookname);
    }
    return 0;
}