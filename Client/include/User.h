#ifndef CLIENT_USER_H
#define CLIENT_USER_H

#include <unordered_map>
#include <string>
#include <vector>
#include "Inventory.h"
#include "Frame.h"
#include "Book.h"

using namespace std;

class User{
private:
    string username;
    string passcode;
    vector<string> id_topics;
    unordered_map<int, string> receiptMap;
    Inventory inventory;
    vector<string> booksWishList;
    bool isConnected;

public:
    User(string username, string passcode);
    void addTopic(string topic);
    int getTopicID(string topic);
    string getName();
    vector<string> getIdTopics();
    void addReceiptFrame(int receiptID, string s);
    unordered_map<int, string> getReceiptMap();
    void addBook(string bookName, string topic, string owner);
    bool containsBook(string topic, string bookName);
    void addToWishList(string bookName);
    void removeFromWishList(string bookName);
    bool inWishList(string bookName);
    void removeBook(string bookName);
    string getBooksOfTopic(string topic);
    void cleanInventory();
    void setBorrowerIsOwner(string topic, string bookName, bool value);
    string getLenderOf(string topic, string bookName);
};

#endif //CLIENT_USER_H