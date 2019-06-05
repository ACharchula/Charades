//
// Created by adam on 22.05.19.
//

#ifndef CPP_CLIENT_Consts_H
#define CPP_CLIENT_Consts_H

#include <string>

//client
const std::string TEXT = "SEND_MESSAGE";
const std::string HELLO = "HELLO_SERVER";
const std::string ENTER = "ENTER__TABLE"; // TODO change it -> 4 + body
const std::string LEAVE = "COMEOUTTABLE"; // 4 //new
const std::string PONG = "PONG____PONG"; // 4 //new
const std::string GIVEUP = "GIVE_UP_GAME"; // 4 //new
const std::string GETSTATS = "GETSTATISTIC"; // 4 //new
const std::string CREATETABLE = "CREATE_TABLE"; // 4 //new
const std::string TABLEREQUEST = "LIST__TABLES"; // 4 // new

//server
const std::string WELCOME = "WELCOME_USER"; //4
const std::string TABLELIST = "SEE___TABLES"; // 4 // new
const std::string CHAT = "CHAT_MESSAGE"; // 4
const std::string WAIT = "GAME_WAITING"; // 4
const std::string END = "GAME___ENDED"; // 4
const std::string READY = "GAME___READY"; // 4
const std::string DRAW = "YOUAREDRAWER"; // 4
const std::string PING = "PING____PING"; // 4 // new
const std::string TABLECREATED = "TABLECREATED"; // 4 // new -> 4 + body
const std::string FAIL = "COMMANDFAILD"; // 4 // new
const std::string ABORT = "GAME_ABORTED"; // 4 // new
const std::string STATS = "SEESTATISTIC"; // 4 // new
const std::string SET = "SET___CANVAS"; // 8
const std::string UPDATE = "UPDATECANVAS"; // 8

//errors
const std::string ERROROPEN = "Opening stream socket failed.";
const std::string ERRORUKNOWNSERVER = "Uknown server.";
const std::string ERRORCONNECT = "Connection failed.";
const std::string ERRORWRITING = "Error writing to socket.";
const std::string ERRORREADING = "Error reading from socket.";
const std::string ERRORRECEAVING = "Error in receiving data.";

#endif //CPP_CLIENT_Consts_H
