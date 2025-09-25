#include "User.hpp"

// Constructor implementation
User::User(const string& username, const string& password) 
    : username_(username) {
    // TODO: In a real application, use a strong hashing library like bcrypt or Argon2.
    // For this project, we'll use a simple "dummy" hash for demonstration.
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

// Convert User object to JSON
void User::toJson(json& j) const {
    j = json{{"username", username_}, {"password_hash", password_hash_}};
}

// Create User object from JSON
User User::fromJson(const json& j) {
    string username = j.at("username").get<string>();
    string password_hash = j.at("password_hash").get<string>();

    // We need a way to create a User without re-hashing the password
    // Let's create a temporary User and then set the hash directly
    User user(username, ""); // Create with a dummy password
    user.password_hash_ = password_hash; // Directly set the stored hash
    return user;
}

// Adds a message to the user's private inbox
void User::receiveMessage(const Message& msg) {
    this->inbox_.push_back(msg);
}

// Returns a reference to the user's inbox
vector<Message>& User::getInbox() {
    return this->inbox_;
}

// Clears all messages from the inbox
void User::clearInbox() {
    this->inbox_.clear();
}