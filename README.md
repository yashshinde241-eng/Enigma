# Enigma 🛡️

A simple, encrypted C++ chat backend built for an Object-Oriented Programming project. This project is developed following a 6-day plan, focusing on clean code, modern C++, and professional development practices.

## Features
- [ ] User registration and login
- [ ] Direct, ephemeral messaging
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

- **`User` Class:** Represents a user of the service. It encapsulates the `username` and a hashed `password`, providing the basic structure for user authentication.
- **`Message` Class:** Represents a single message sent between users. It contains the message `content`, the `sender`'s username, and the `recipient`'s username.

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