#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "Message.hpp"

using namespace std;
using json = nlohmann::json;

class User {
private:
    string username_;
    string password_hash_;
    vector<Message> inbox_;
    bool is_online_; 

public:
    // Constructor
    User(const string& username, const string& password);

    // Getters
    string getUsername() const;
    string getPasswordHash() const;
    vector<Message>& getInbox();
    bool isOnline() const; 

    // Setters
    void receiveMessage(const Message& msg);
    void clearInbox();
    void setOnline(bool status); 

    // JSON serialization
    void toJson(json& j) const;
    static User fromJson(const json& j);
};