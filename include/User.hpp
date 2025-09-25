#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp> // <-- ADD THIS LINE
#include "Message.hpp"

using namespace std;
using json = nlohmann::json; // <-- AND ADD THIS LINE

class User {
private:
    string username_;
    string password_hash_;
    vector<Message> inbox_;

public:
    // Constructor
    User(const string& username, const string& password);

    // Getters
    string getUsername() const;
    string getPasswordHash() const;

    void receiveMessage(const Message& msg);

    // Functions for JSON serialization
    void toJson(json& j) const;
    static User fromJson(const json& j);

    // ... inside the User class public section ...
    vector<Message>& getInbox();
    void clearInbox();
};