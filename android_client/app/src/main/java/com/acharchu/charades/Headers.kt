package com.acharchu.charades

class Headers {
    companion object {
        val headers: HashMap<String, HeaderType> = hashMapOf(
            "CHAT_MESSAGE" to HeaderType.CHAT_MESSAGE,
            "UPDATECANVAS" to HeaderType.UPDATECANVAS,
            "GAME___ENDED" to HeaderType.GAME_ENDED,
            "GAME_WAITING" to HeaderType.GAME_WAITING,
            "GAME___READY" to HeaderType.GAME_READY,
            "YOUAREDRAWER" to HeaderType.YOU_ARE_DRAWER,
            "CLUEINCORECT" to HeaderType.CLUE_INCORRECT,
            "CLUE_CORRECT" to HeaderType.CLUE_CORRECT
        )
    }
}