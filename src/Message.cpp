#include "Message.hpp"

// Constructor implementation using a member initializer list
Message::Message(const string& content, const string& sender, const string& recipient)
    : content_(content),
      sender_(sender),
      recipient_(recipient) {
    // The constructor's body is empty because the initializer list handles everything.
}

// Getter for content
string Message::getContent() const {
    return this->content_;
}

// Getter for sender
string Message::getSender() const {
    return this->sender_;
}

// Getter for recipient
string Message::getRecipient() const {
    return this->recipient_;
}