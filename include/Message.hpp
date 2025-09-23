#pragma once

#include <string>

using namespace std;

class Message {
private:
    string content_;
    string sender_;
    string recipient_;

public:
    // Constructor
    Message(const string& content, const string& sender, const string& recipient);

    // Getters
    string getContent() const;
    string getSender() const;
    string getRecipient() const;
};