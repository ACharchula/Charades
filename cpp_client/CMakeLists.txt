cmake_minimum_required(VERSION 3.5)
project(cpp_client)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)
find_Package(Threads REQUIRED)

add_executable(cpp_client src/main.cpp src/client/Connection/client.cpp src/client/Connection/client.h src/client/Connection/Worker.cpp src/client/Connection/Worker.h src/client/UI/MainWindow.cpp src/client/UI/MainWindow.h src/client/Connection/Message.cpp src/client/Connection/Message.h src/client/UI/DrawView.cpp src/client/UI/DrawView.h src/client/UI/DrawScene.cpp src/client/UI/DrawScene.h src/client/UI/LoginDialog.cpp src/client/UI/LoginDialog.h src/client/UI/ChangeTableDialog.cpp src/client/UI/ChangeTableDialog.h src/client/Consts.h src/client/Controller/Controller.cpp src/client/Controller/Controller.h src/client/UI/MessageBox.cpp src/client/UI/MessageBox.h)
target_link_libraries(cpp_client Qt5::Core Qt5::Widgets Qt5::Gui)
target_link_libraries(cpp_client Threads::Threads)