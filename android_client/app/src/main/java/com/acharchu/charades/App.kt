package com.acharchu.charades

import java.io.BufferedReader
import java.io.InputStreamReader
import java.io.PrintWriter
import java.net.Socket

class App {

    private val serverHost = "10.0.2.2" //has to be changed if u want to run on real smartphone
    private val serverPort = 45454
    private var socket: Socket? = null
    private var output: PrintWriter? = null
    private var input: BufferedReader? = null
    var messages = arrayListOf<String>()

    private val id : Int = (0..100).random()

    fun connectToServer() {
        try {
            socket = Socket(serverHost, serverPort)

            output = PrintWriter(socket!!.getOutputStream(), true)
            input = BufferedReader(InputStreamReader(socket?.getInputStream()))

            hello()

        } catch (exception: Exception) {
            throw exception
        }
    }

    private fun send(msg : String) {
        output?.println(msg)
    }

    private fun get() : String? {
        return input?.readLine()
    }

    private fun hello() {
        send("HELLO\nAndroidClient$id\n")

        if (get() != "WELCOME")
            throw java.lang.Exception()
    }

    fun sendMessage(msg : String) {
        send("SEND_MESSAGE\n${msg.length}\n$msg\n")
    }

    fun getMessage() : String? {

        if (get() == "CHAT_MESSAGE") {
            val userId : String? = get()
            val bytes : String? = get()
            val msg : String? = get()

            return "$userId: $msg"
        }

        return null
    }
}