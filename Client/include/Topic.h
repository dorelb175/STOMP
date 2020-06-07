//
// Created by dorelb@wincs.cs.bgu.ac.il on 07/01/2020.
//

#ifndef CLIENT_TOPIC_H
#define CLIENT_TOPIC_H

#include <vector>
#include <string>
#include "Book.h"

using namespace std;

class Topic {
private:
    string topicName;
    vector<Book*> books;
public:
    Topic(string topicName);
    string getTopicName();
    vector<Book*> getBooks();
    void addBook(Book* book);
    string containsBook(string bookName);
    void removeBook(string book);
    string getBooksOfTopic();
    void cleanTopic();
    void setBorrowerIsOwner(string bookName, bool value);
    string getLenderOf(string bookName);
};

#endif //CLIENT_TOPIC_H