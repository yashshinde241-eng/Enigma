#include <httplib.h>
#include <iostream>

using namespace std;

int main() {
    // Create a server object
    httplib::Server svr;

    // Define the action for the root URL "/"
    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("<h1>Enigma Server is Running!</h1>", "text/html");
    });

    cout << "Enigma server starting on http://localhost:18080" << endl;
    
    // Start the server on port 18080
    svr.listen("0.0.0.0", 18080);

    return 0;
}