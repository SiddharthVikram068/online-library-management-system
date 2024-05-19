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
#include "adminHomepage.h"
#include "actions.h"

using namespace std;

// locks for files 
mutex adminPages;

string convertToString(char* a, int size)
{
    string s(a, size);
    return s;
}

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
    adminPages.lock();
    string filepath = "homepage_db/admin_pages.txt";
    vector<string> adminInfo = getAdminInfo(filepath, adminPagename);
    adminPages.unlock();

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

    // code to add actions based upon the input by client

    /*
    Action protocol
    
    9 - logout
    10 - add Book
    11 - delete Book
    12 - Search Book
    13 - add User

    */
    uint32_t recievedAction;
    recv(clientSocket, &recievedAction, sizeof(recievedAction), 0);
    int actionNumber = ntohl(recievedAction);

    cout << actionNumber << endl;

    if(actionNumber == 9){
        return 3; // go back to login page 
    }

    else if(actionNumber == 10){
        char bookBuf[1024];
        int bytesRecieved = recv(clientSocket, bookBuf, 1024, 0);
        string searchBookName(bookBuf, bytesRecieved);
        addBook("book_db/books.txt", searchBookName);
    }

    // delete book
    else if(actionNumber == 11){
        char bookBuf[1024];
        int bytesRecieved = recv(clientSocket, bookBuf, 1024, 0);
        string searchBookName(bookBuf, bytesRecieved);
        deleteBook("book_db/books.txt", searchBookName);
    }

    // search book
    else if(actionNumber == 12){
        char bookBuf[1024];
        int bytesRecieved = recv(clientSocket, bookBuf, 1024, 0);
        string searchBookName(bookBuf, bytesRecieved);
        string bookInfo = searchBook("book_db/books.txt", searchBookName);

        // send bookInfo to the client
        uint32_t someLength = htonl(bookInfo.length());
        send(clientSocket, &someLength, sizeof(someLength), 0);
        send(clientSocket, bookInfo.c_str(), bookInfo.length(), 0);

    }

    // modify user
    // name dob number_of_books_borrowed
    else if(actionNumber == 13){
        vector<string> info(4, "");
        char nameBuf[1024];
        char newName[1024];
        char newDob[1024];
        char newbb[1024];

        int length = 0;
        uint32_t recieveLength;

        recv(clientSocket, &recieveLength, sizeof(recieveLength), 0);
        length = ntohl(recieveLength);
        int bytesRead = recv(clientSocket, nameBuf, length, 0);
        info[0] = convertToString(nameBuf, length);
        
        recv(clientSocket, &recieveLength, sizeof(recieveLength), 0);
        length = ntohl(recieveLength);
        bytesRead = recv(clientSocket, newName, length, 0);
        info[1] = convertToString(newName, length);

        recv(clientSocket, &recieveLength, sizeof(recieveLength), 0);
        length = ntohl(recieveLength);
        bytesRead = recv(clientSocket, newDob, length, 0);
        info[2] = convertToString(newDob, length);

        recv(clientSocket, &recieveLength, sizeof(recieveLength), 0);
        length = ntohl(recieveLength);
        bytesRead = recv(clientSocket, newbb, length, 0);
        info[3] = convertToString(newbb, length);

        modifyUser(info[0], info[1], info[2], info[3]);

        
    }

    else{
        cout << "some error occured";
        return 3;
    }

    return 4;
}