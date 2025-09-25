#include <httplib.h>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include "User.hpp"
#include <fstream>

using namespace std;
using json = nlohmann::json;

// Our simple in-memory database. A vector that holds all registered User objects.
vector<User> users;

// --- Persistence Functions ---

// Saves the current list of users to a file
void saveUsersToFile() {
    json user_list_json;
    for (const auto& user : users) {
        json user_json;
        user.toJson(user_json);
        user_list_json.push_back(user_json);
    }

    ofstream file("database.json");
    file << user_list_json.dump(4); // .dump(4) formats the JSON nicely
}

// Loads the list of users from a file when the server starts
void loadUsersFromFile() {
    ifstream file("database.json");
    if (file.is_open()) {
        json user_list_json;
        file >> user_list_json;

        for (const auto& user_json : user_list_json) {
            users.push_back(User::fromJson(user_json));
        }
        cout << "Loaded " << users.size() << " users from database.json" << endl;
    }
}

int main() {
    loadUsersFromFile();

    httplib::Server svr;

    // The old "Hello World" route. We can keep it for now.
    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("<h1>Enigma Server is Running!</h1>", "text/html");
    });
    
    // Endpoint for user registration
    svr.Post("/register", [&](const httplib::Request &req, httplib::Response &res) {
        json response_json;
        
        try {
            // Parse the incoming JSON body
            json data = json::parse(req.body);

            // Extract username and password
            string username = data.at("username");
            string password = data.at("password");

            // --- Business Logic ---
            // Check if username already exists
            for (const auto& user : users) {
                if (user.getUsername() == username) {
                    // User exists, send an error response
                    res.status = 409; // 409 Conflict
                    response_json["status"] = "error";
                    response_json["message"] = "Username already exists.";
                    res.set_content(response_json.dump(), "application/json");
                    return;
                }
            }

            // If username is new, create user and add to our "database"
            User new_user(username, password);
            users.push_back(new_user);
            saveUsersToFile();

            // Send a success response
            res.status = 201; // 201 Created
            response_json["status"] = "success";
            response_json["message"] = "User created successfully.";
            res.set_content(response_json.dump(), "application/json");

        } catch (const json::parse_error& e) {
            // If JSON is invalid
            res.status = 400; // 400 Bad Request
            response_json["status"] = "error";
            response_json["message"] = "Invalid JSON format.";
            res.set_content(response_json.dump(), "application/json");
        } catch (const json::out_of_range& e) {
            // If "username" or "password" keys are missing
            res.status = 400; // 400 Bad Request
            response_json["status"] = "error";
            response_json["message"] = "Missing 'username' or 'password' in request.";
            res.set_content(response_json.dump(), "application/json");
        }
    });

    // Endpoint for user login
    svr.Post("/login", [&](const httplib::Request &req, httplib::Response &res) {
        json response_json;
        
        try {
            json data = json::parse(req.body);
            string username = data.at("username");
            string password = data.at("password");

            // --- Business Logic ---
            // Find the user
            for (const auto& user : users) {
                if (user.getUsername() == username) {
                    // User found, now check password
                    // We re-create the "dummy hash" to see if it matches the stored one
                    if (user.getPasswordHash() == (password + "_hashed")) {
                        // Password matches
                        res.status = 200; // 200 OK
                        response_json["status"] = "success";
                        response_json["message"] = "Login successful.";
                        res.set_content(response_json.dump(), "application/json");
                        return;
                    } else {
                        // Password does not match
                        res.status = 401; // 401 Unauthorized
                        response_json["status"] = "error";
                        response_json["message"] = "Invalid username or password.";
                        res.set_content(response_json.dump(), "application/json");
                        return;
                    }
                }
            }

            // If the loop finishes, the user was not found
            res.status = 404; // 404 Not Found
            response_json["status"] = "error";
            response_json["message"] = "Invalid username or password.";
            res.set_content(response_json.dump(), "application/json");

        } catch (const exception& e) {
            // Catches errors from JSON parsing or missing keys
            res.status = 400; // 400 Bad Request
            response_json["status"] = "error";
            response_json["message"] = "Invalid request format.";
            res.set_content(response_json.dump(), "application/json");
        }
    });

    // Endpoint to send a message
    svr.Post("/send", [&](const httplib::Request &req, httplib::Response &res) {
        json response_json;

        try {
            json data = json::parse(req.body);
            string sender = data.at("sender");
            string recipient = data.at("recipient");
            string content = data.at("content");

            // TODO: In a real app, you'd verify the sender is authenticated (e.g., with a token).
            // We are skipping that for simplicity.

            // --- Business Logic ---
            Message new_message(content, sender, recipient);
            bool recipient_found = false;

            // Find the recipient user and deliver the message.
            // Note the '&' in 'auto& user': this lets us modify the actual user object.
            for (auto& user : users) {
                if (user.getUsername() == recipient) {
                    user.receiveMessage(new_message);
                    recipient_found = true;
                    break; // Exit loop once user is found
                }
            }

            if (recipient_found) {
                res.status = 200; // 200 OK
                response_json["status"] = "success";
                response_json["message"] = "Message sent.";
                res.set_content(response_json.dump(), "application/json");
            } else {
                res.status = 404; // 404 Not Found
                response_json["status"] = "error";
                response_json["message"] = "Recipient not found.";
                res.set_content(response_json.dump(), "application/json");
            }

        } catch (const exception& e) {
            res.status = 400; // 400 Bad Request
            response_json["status"] = "error";
            response_json["message"] = "Invalid request format. Requires sender, recipient, and content.";
            res.set_content(response_json.dump(), "application/json");
        }
    });

    // Endpoint to retrieve and clear a user's messages
    svr.Get("/getMessages", [&](const httplib::Request &req, httplib::Response &res) {
        json response_json;

        // We identify the user by a query parameter, e.g., /getMessages?username=yash
        if (!req.has_param("username")) {
            res.status = 400; // Bad Request
            response_json["error"] = "Missing username query parameter.";
            res.set_content(response_json.dump(), "application/json");
            return;
        }

        string username = req.get_param_value("username");
        bool user_found = false;

        for (auto& user : users) {
            if (user.getUsername() == username) {
                user_found = true;
                
                vector<Message>& inbox = user.getInbox();
                json messages_json = json::array();

                // Convert all messages in the inbox to JSON
                for (const auto& msg : inbox) {
                    messages_json.push_back({
                        {"from", msg.getSender()},
                        {"content", msg.getContent()}
                    });
                }
                
                res.status = 200; // OK
                res.set_content(messages_json.dump(), "application/json");

                // Clear the inbox after retrieving the messages
                user.clearInbox();
                break;
            }
        }

        if (!user_found) {
            res.status = 404; // Not Found
            response_json["error"] = "User not found.";
            res.set_content(response_json.dump(), "application/json");
        }
    });

    cout << "Enigma server starting on http://localhost:18080" << endl;
    
    svr.listen("0.0.0.0", 18080);

    return 0;
}