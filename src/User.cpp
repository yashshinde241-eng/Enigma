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