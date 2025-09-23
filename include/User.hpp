/*It's a special instruction to the compiler to ensure this file is only included 
once during the build process, which prevents redefinition errors.*/
#pragma once

#include <string>

// Use the standard namespace to avoid writing "std::" repeatedly
using namespace std;

class User {
private:
    // Member variables to store user data
    string username_;
    string password_hash_;

public:
    // Constructor to create a new User object
    User(const string& username, const string& password);

    // Getter methods to access private data safely
    string getUsername() const;
    string getPasswordHash() const;
};