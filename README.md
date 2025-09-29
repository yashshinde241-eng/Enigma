# Enigma 🛡️

A simple, encrypted C++ chat backend built for an Object-Oriented Programming project. This project is developed following a 6-day plan, focusing on clean code, modern C++, and professional development practices.

## Features
- [x] User registration and login
- [x] Direct, ephemeral messaging with file-based persistence
- [ ] Simple Caesar cipher encryption for all messages

## Tech Stack
- **Language:** C++17
- **Build System:** CMake
- **Package Manager:** vcpkg
- **Core Libraries:**
    - `cpp-httplib`: For handling HTTP requests and creating the web server.
    - `nlohmann-json`: For parsing and creating JSON data structures.

## Project Structure
The core logic of the application is built around two main classes:

- **`User` Class:** Represents a user of the service. It encapsulates the `username`, a hashed `password`, and a message `inbox`.
- **`Message` Class:** Represents a single message sent between users. It contains the message `content`, the `sender`'s username, and the `recipient`'s username.

## API Endpoints

### User Management
#### `POST /register`
Registers a new user.
- **Request Body:** `{"username": "your_username", "password": "your_password"}`
- **Success Response:** `201 Created`
- **Failure Response:** `409 Conflict` if user exists, `400 Bad Request` if data is invalid.

#### `POST /login`
Logs in an existing user.
- **Request Body:** `{"username": "your_username", "password": "your_password"}`
- **Success Response:** `200 OK`
- **Failure Response:** `401 Unauthorized` for wrong password, `404 Not Found` for non-existent user.

### Messaging
#### `POST /send`
Sends a message from one user to another.
- **Request Body:** `{"sender": "sender_username", "recipient": "recipient_username", "content": "your message"}`
- **Success Response:** `200 OK`
- **Failure Response:** `404 Not Found` if recipient does not exist.

#### `GET /getMessages`
Retrieves all unread messages for a user and then clears their inbox.
- **URL Format:** `/getMessages?username=your_username`
- **Success Response:** `200 OK` with a JSON array of messages.
- **Failure Response:** `404 Not Found` if user does not exist.

## Getting Started

### Prerequisites
- Visual Studio Build Tools 2022 (with C++ workload)
- vcpkg
- CMake

### Build Instructions
1. Clone the repository:
   `git clone https://github.com/YourUsername/Enigma.git`
2. Install libraries with vcpkg:
   `C:\vcpkg\vcpkg.exe install cpp-httplib nlohmann-json`
3. Configure the project with CMake:
   `cmake -B build -S . -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake`
4. Build the project:
   `cmake --build build`
5. Run the server:
   `.\build\Debug\enigma_server.exe`

### Running the Full Application
The application requires two servers running simultaneously.

Terminal 1 (Backend): Navigate to the Enigma root folder and run the C++ server.

PowerShell

.\backend\build\Debug\enigma_server.exe
Terminal 2 (Frontend): Navigate to the Enigma/frontend folder and run the Python web server.

PowerShell

cd frontend
python -m http.server 8000
Access the Application: Open your web browser and go to http://localhost:8000.