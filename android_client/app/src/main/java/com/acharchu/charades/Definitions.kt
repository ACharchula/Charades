package com.acharchu.charades

enum class State {
    CONNECTED,
    DISCONNECTED
}

enum class HeaderType {
    CHAT_MESSAGE,
    CONNECTION_CLOSED,
    UNDEFINED,
    UPDATECANVAS,
    GAME_WAITING,
    GAME_ENDED,
    GAME_READY,
    YOU_ARE_DRAWER,
    TABLE_CREATED,
    SEE_TABLES,
    GAME_ABORTED,
    COMMAND_FAILED,
    PING_PING,
    SEE_STATISTIC,
}