#include <httplib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include "User.hpp"
#include "Message.hpp"

using namespace std;
using json = nlohmann::json;

// Our simple in-memory database.
vector<User> users;

// --- Persistence Functions ---

// Saves the current list of users to a file
void saveUsersToFile() {
    json user_list_json = json::array();
    for (const auto& user : users) {
        json user_json;
        user.toJson(user_json);
        user_list_json.push_back(user_json);
    }

    ofstream file("database.json");
    file << user_list_json.dump(4);
}

// Loads the list of users from a file when the server starts
void loadUsersFromFile() {
    ifstream file("database.json");
    if (file.is_open()) {
        if (file.peek() != ifstream::traits_type::eof()) { // Check if file is not empty
            json user_list_json;
            file >> user_list_json;
            users.clear();
            for (const auto& user_json : user_list_json) {
                users.push_back(User::fromJson(user_json));
            }
        }
        cout << "Loaded " << users.size() << " users from database.json" << endl;
    }
}


int main() {
    loadUsersFromFile();

    httplib::Server svr;

    // --- Manually handle OPTIONS requests for CORS preflight ---
    svr.Options("/(.*)", [](const httplib::Request&, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Access-Control-Allow-Origin", "*");
        res.status = 204;
    });

    // --- API Endpoints (with manual headers) ---

    svr.Get("/", [](const httplib::Request&, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("<h1>Enigma Server is Running!</h1>", "text/html");
    });
    
    // Endpoint for user registration
    svr.Post("/register", [&](const httplib::Request &req, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        json response_json;
        try {
            json data = json::parse(req.body);
            string username = data.at("username");
            string password = data.at("password");

            if (username.empty() || password.empty()) {
                res.status = 400;
                response_json["status"] = "error";
                response_json["message"] = "Username and password cannot be empty.";
                res.set_content(response_json.dump(), "application/json");
                return;
            }
            
            for (const auto& user : users) {
                if (user.getUsername() == username) {
                    res.status = 409;
                    response_json["status"] = "error";
                    response_json["message"] = "Username already exists.";
                    res.set_content(response_json.dump(), "application/json");
                    return;
                }
            }

            User new_user(username, password);
            users.push_back(new_user);
            saveUsersToFile();

            res.status = 201;
            response_json["status"] = "success";
            response_json["message"] = "User created successfully.";
            res.set_content(response_json.dump(), "application/json");

        } catch (const exception& e) {
            res.status = 400;
            response_json["status"] = "error";
            response_json["message"] = "Invalid request: missing 'username' or 'password', or invalid JSON.";
            res.set_content(response_json.dump(), "application/json");
        }
    });

    // Endpoint for user login
    svr.Post("/login", [&](const httplib::Request &req, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        json response_json;
        try {
            json data = json::parse(req.body);
            string username = data.at("username");
            string password = data.at("password");

            for (const auto& user : users) {
                if (user.getUsername() == username) {
                    if (user.getPasswordHash() == (password + "_hashed")) {
                        res.status = 200;
                        response_json["status"] = "success";
                        response_json["message"] = "Login successful.";
                        res.set_content(response_json.dump(), "application/json");
                        return;
                    } else {
                        res.status = 401;
                        response_json["status"] = "error";
                        response_json["message"] = "Invalid username or password.";
                        res.set_content(response_json.dump(), "application/json");
                        return;
                    }
                }
            }
            
            res.status = 404;
            response_json["status"] = "error";
            response_json["message"] = "Invalid username or password.";
            res.set_content(response_json.dump(), "application/json");

        } catch (const exception& e) {
            res.status = 400;
            response_json["status"] = "error";
            response_json["message"] = "Invalid request format.";
            res.set_content(response_json.dump(), "application/json");
        }
    });
    
    // Endpoint to get a list of all usernames
    svr.Get("/users", [&](const httplib::Request &, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        json user_list = json::array();
        for(const auto& user : users) {
            user_list.push_back(user.getUsername());
        }
        res.set_content(user_list.dump(), "application/json");
    });

    // Endpoint to send a message
    svr.Post("/send", [&](const httplib::Request &req, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        json response_json;
        try {
            json data = json::parse(req.body);
            string sender = data.at("sender");
            string recipient = data.at("recipient");
            string content = data.at("content");

            Message new_message(content, sender, recipient);
            bool recipient_found = false;

            for (auto& user : users) {
                if (user.getUsername() == recipient) {
                    user.receiveMessage(new_message);
                    saveUsersToFile();
                    recipient_found = true;
                    break;
                }
            }

            if (recipient_found) {
                res.status = 200;
                response_json["status"] = "success";
                response_json["message"] = "Message sent.";
                res.set_content(response_json.dump(), "application/json");
            } else {
                res.status = 404;
                response_json["status"] = "error";
                response_json["message"] = "Recipient not found.";
                res.set_content(response_json.dump(), "application/json");
            }

        } catch (const exception& e) {
            res.status = 400;
            response_json["status"] = "error";
            response_json["message"] = "Invalid request format. Requires sender, recipient, and content.";
            res.set_content(response_json.dump(), "application/json");
        }
    });

    // Endpoint to retrieve and clear a user's messages
    svr.Get("/getMessages", [&](const httplib::Request &req, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        json response_json;
        if (!req.has_param("username")) {
            res.status = 400;
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

                for (const auto& msg : inbox) {
                    messages_json.push_back({
                        {"from", msg.getSender()},
                        {"content", msg.getDecryptedContent()}
                    });
                }
                
                res.status = 200;
                res.set_content(messages_json.dump(), "application/json");

                user.clearInbox();
                saveUsersToFile();
                break;
            }
        }

        if (!user_found) {
            res.status = 404;
            response_json["error"] = "User not found.";
            res.set_content(response_json.dump(), "application/json");
        }
    });

    // --- Start Server ---
    cout << "Enigma server starting on http://localhost:18080" << endl;
    svr.listen("0.0.0.0", 18080);

    return 0;
}