#pragma once

#include <string>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

class Message {
private:
    string encrypted_content_; 
    string sender_;
    string recipient_;

    // Private helper functions for encryption/decryption
    string encrypt(const string& content);
    string decrypt(const string& encrypted_content) const;

public:
    // Constructor
    Message(const string& content, const string& sender, const string& recipient);

    // Getters
    string getDecryptedContent() const; 
    string getSender() const;
    string getRecipient() const;

    // Functions for JSON serialization
    void toJson(json& j) const;
    static Message fromJson(const json& j);
};