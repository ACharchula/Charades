cmake_minimum_required(VERSION 3.5)
project(cpp_client)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)
find_Package(Threads REQUIRED)

add_executable(cpp_client src/main.cpp src/client/client.cpp src/client/client.h src/client/Worker.cpp src/client/Worker.h src/client/MainWindow.cpp src/client/MainWindow.h src/client/Message.cpp src/client/Message.h)
target_link_libraries(cpp_client Qt5::Core Qt5::Widgets Qt5::Gui)
target_link_libraries(cpp_client Threads::Threads)
