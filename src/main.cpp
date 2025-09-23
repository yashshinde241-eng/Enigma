#include <httplib.h>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include "User.hpp"

using namespace std;
using json = nlohmann::json;

// Our simple in-memory database. A vector that holds all registered User objects.
vector<User> users;

int main() {
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

    cout << "Enigma server starting on http://localhost:18080" << endl;
    
    svr.listen("0.0.0.0", 18080);

    return 0;
}