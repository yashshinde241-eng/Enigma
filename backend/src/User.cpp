#include "User.hpp"
#include "Message.hpp" // Make sure Message.hpp is included

// Constructor implementation
User::User(const string& username, const string& password) 
    : username_(username), is_online_(false) { // <-- Initialize is_online_ to false
    this->password_hash_ = password + "_hashed";
}

// Getter for username
string User::getUsername() const {
    return this->username_;
}

// Getter for the password hash
string User::getPasswordHash() const {
    return this->password_hash_;
}

// Getter for online status
bool User::isOnline() const {
    return this->is_online_;
}

// Returns a reference to the user's inbox
vector<Message>& User::getInbox() {
    return this->inbox_;
}

// Adds a message to the user's private inbox
void User::receiveMessage(const Message& msg) {
    this->inbox_.push_back(msg);
}

// Clears all messages from the inbox
void User::clearInbox() {
    this->inbox_.clear();
}

// Setter for online status
void User::setOnline(bool status) {
    this->is_online_ = status;
}

// Convert User object to JSON
void User::toJson(json& j) const {
    json inbox_json = json::array();
    for (const auto& msg : inbox_) {
        json msg_json;
        msg.toJson(msg_json);
        inbox_json.push_back(msg_json);
    }

    j = json{
        {"username", username_}, 
        {"password_hash", password_hash_},
        {"inbox", inbox_json},
        {"is_online", is_online_} 
    };
}

// Create User object from JSON
User User::fromJson(const json& j) {
    string username = j.at("username").get<string>();
    string password_hash = j.at("password_hash").get<string>();
    
    User user(username, ""); 
    user.password_hash_ = password_hash; 
    
    // Load online status, default to false if not present
    user.is_online_ = j.value("is_online", false); 

    if (j.contains("inbox")) {
        for (const auto& msg_json : j.at("inbox")) {
            user.inbox_.push_back(Message::fromJson(msg_json));
        }
    }
    return user;
}