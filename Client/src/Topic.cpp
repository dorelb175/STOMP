//
// Created by dorelb@wincs.cs.bgu.ac.il on 07/01/2020.
//

#include "../include/Topic.h"
#include <iostream>

Topic::Topic(string topicName): topicName(topicName), books() {}

string Topic::getTopicName() {
    return topicName;
}

vector<Book*> Topic::getBooks() {
    return books;
}

void Topic::addBook(Book* book) {
    books.push_back(book);
}

string Topic::containsBook(string bookName) {
    bool found = false;
    string topic = "";
    for (int i = 0; !found && !books.empty() && i < (int)books.size(); ++i) {
        if (books[i]->getName().compare(bookName) == 0 && books[i]->getBorrowerIsOwner()) {
            found = true;
            topic = getTopicName();
        }
    }
    return topic;
}

void Topic::removeBook(string book) {
    int i;
    bool found = false;
    for (i = 0; !found && i < (int)books.size(); ++i) {
        if (books[i]->getName().compare(book) == 0) {
            delete books[i];
            books[i] = nullptr;
            found = true;
        }
    }
    books.erase(books.begin() + (i-1));
}

string Topic::getBooksOfTopic() {
    string str = "";
    if (books.empty())
        return str;

    for (int i = 0; i < (int) books.size() - 1; ++i) {
        if (books[i] != nullptr  && books[i]->getBorrowerIsOwner())
            str += books[i]->getName() + ", ";
    }
    if (books[(int)books.size() - 1] != nullptr  && books[(int)books.size() - 1]->getBorrowerIsOwner())
        str += books[(int)books.size() - 1]->getName();

    return str;
}

void Topic::cleanTopic() {
    //erase all of the books of the topic in the heap
    for (int i = 0; i < (int)books.size(); ++i) {
        delete books[i];
        books[i] = nullptr;
    }
}

void Topic::setBorrowerIsOwner(string bookName, bool value) {
    for (int i = 0; !books.empty() && i < (int)books.size(); ++i) {
        if (books[i]->getName().compare(bookName) == 0) {
            books[i]->setBorrowerIsOwner(value);
        }
    }
}

string Topic::getLenderOf(string bookName) {
    bool found = false;
    string lender = "";
    for (int i = 0; !found && !books.empty() && i < (int)books.size(); ++i) {
        if (books[i]->getName().compare(bookName) == 0) {
            found = true;
            lender = books[i]->getOwner();
        }
    }
    return lender;
}

