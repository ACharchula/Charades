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
    GAME_READY
}