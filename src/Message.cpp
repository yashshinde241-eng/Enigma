#include "Message.hpp"

// Private helper function to encrypt a string using Caesar cipher
string Message::encrypt(const string& content) {
    string encrypted = "";
    int shift = 3; // The key for our cipher

    for (char c : content) {
        if (isalpha(c)) { // Only encrypt alphabetic characters
            char base = islower(c) ? 'a' : 'A';
            encrypted += (c - base + shift) % 26 + base;
        } else {
            encrypted += c; // Keep non-alphabetic characters (spaces, numbers, etc.) the same
        }
    }
    return encrypted;
}

// Private helper function to decrypt a string from Caesar cipher
string Message::decrypt(const string& encrypted_content) const {
    string decrypted = "";
    int shift = 3; // Must use the same key

    for (char c : encrypted_content) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            decrypted += (c - base - shift + 26) % 26 + base;
        } else {
            decrypted += c;
        }
    }
    return decrypted;
}

// Constructor implementation now calls encrypt()
Message::Message(const string& content, const string& sender, const string& recipient)
    : sender_(sender),
      recipient_(recipient) {
    
    this->encrypted_content_ = encrypt(content);
}

// Getter for content now calls decrypt()
string Message::getDecryptedContent() const {
    return decrypt(this->encrypted_content_);
}

// Getter for sender
string Message::getSender() const {
    return this->sender_;
}

// Getter for recipient
string Message::getRecipient() const {
    return this->recipient_;
}

// Convert Message object to JSON
void Message::toJson(json& j) const {
    j = json{
        {"sender", sender_},
        {"recipient", recipient_},
        {"encrypted_content", encrypted_content_}
    };
}

// Create Message object from JSON
Message Message::fromJson(const json& j) {
    string sender = j.at("sender");
    string recipient = j.at("recipient");
    string encrypted_content = j.at("encrypted_content");

    // Create a message with dummy content, then overwrite with loaded data
    Message msg("", sender, recipient);
    msg.encrypted_content_ = encrypted_content;
    return msg;
}