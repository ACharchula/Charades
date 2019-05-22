//
// Created by adam on 22.05.19.
//

#ifndef CPP_CLIENT_Consts_H
#define CPP_CLIENT_Consts_H

#include <string>

//client
const std::string TEXT = "SEND_MESSAGE";
const std::string HELLO = "HELLO_SERVER";
const std::string ENTER = "ENTER__TABLE";

//server
const std::string WELCOME = "WELCOME_USER"; //4
const std::string CHAT = "CHAT_MESSAGE"; // 4
const std::string WAIT = "GAME_WAITING"; // 4
const std::string END = "GAME___ENDED"; // 4
const std::string READY = "GAME___READY"; // 4
const std::string DRAW = "YOUAREDRAWER"; // 4
const std::string CORRECT = "CLUE_CORRECT"; // 4
const std::string INCORRECT = "CLUEINCORECT"; // 4
const std::string SET = "SET___CANVAS"; // 8
const std::string UPDATE = "UPDATECANVAS"; // 8

//errors

const std::string ERROROPEN = "Opening stream socket failed.";
const std::string ERRORUKNOWNSERVER = "Uknown server.";
const std::string ERRORCONNECT = "Connection failed.";
const std::string ERRORWRITING = "Error writing to socket.";
const std::string ERRORREADING = "Error reading from socket.";

#endif //CPP_CLIENT_Consts_H
