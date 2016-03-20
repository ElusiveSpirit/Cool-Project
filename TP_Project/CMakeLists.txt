cmake_minimum_required(VERSION 3.3)
project(Chat)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -Wextra")

set(SERVER_FILES server.cpp sockets.cpp NetException.cpp)
add_executable(server ${SERVER_FILES})

set(CLIENT_FILES client.cpp sockets.cpp NetException.cpp)
add_executable(client ${CLIENT_FILES})