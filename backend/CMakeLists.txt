cmake_minimum_required(VERSION 3.18)

project(Enigma VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(httplib CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(enigma_server src/main.cpp src/User.cpp src/Message.cpp)

# This new line tells our server where to find its .hpp files
target_include_directories(enigma_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(enigma_server PRIVATE httplib::httplib nlohmann_json::nlohmann_json)