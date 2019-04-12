cmake_minimum_required(VERSION 3.5)
project(cpp_client)

set(CMAKE_CXX_STANDARD 11)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)
find_Package(Threads REQUIRED)

add_executable(cpp_client src/main.cpp src/client/client.cpp src/client/client.h)
target_link_libraries(cpp_client Qt5::Core Qt5::Widgets Qt5::Gui)
target_link_libraries(cpp_client Threads::Threads)
