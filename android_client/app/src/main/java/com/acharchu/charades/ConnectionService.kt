package com.acharchu.charades

import java.io.BufferedReader
import java.io.InputStreamReader
import java.io.PrintWriter
import java.net.Socket

class ConnectionService {

    private val serverHost = "10.0.2.2" //has to be changed if u want to run on real smartphone
    private val serverPort = 45454
    private var socket: Socket? = null
    private var output: PrintWriter? = null
    private var input: BufferedReader? = null
    var messages = arrayListOf<String>()

    private val HEADER_LENGTH = 12
    private val BYTES_TO_READ_LENGTH = 4

    var status = State.DISCONNECTED

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


    private fun hello() {
        send("HELLO\nAndroidClient$id\n")
//        if (get() != "WELCOME")
//            throw java.lang.Exception()
    }

    fun sendMessage(msg : String) {
        send("SEND_MESSAGE\n${msg.length}\n$msg")
    }

    fun getMessage() : String? {

        val usernameAndMessageLength = read(BYTES_TO_READ_LENGTH)
        val usernameAndMessage = read(usernameAndMessageLength.toInt()+1).split('\n')

        return "${usernameAndMessage[0]}: ${usernameAndMessage[1]}"
    }

    fun getHeader() : HeaderType? {
        val header = read(HEADER_LENGTH)

        if (Headers.headers.containsKey(header))
            return Headers.headers[header]

        println(header)
        return HeaderType.UNDEFINED
    }

    private fun read(bytesToRead : Int) : String {
        var amountOfCharacters = 0
        var consumedCharacters = 0
        val buffer = CharArray(bytesToRead)

        while (amountOfCharacters != bytesToRead && consumedCharacters != -1) {
            consumedCharacters = input?.read(buffer, amountOfCharacters, bytesToRead - amountOfCharacters)!!
            amountOfCharacters += consumedCharacters
        }

        if(consumedCharacters == -1)
            throw Throwable("CONNECTION CLOSED")

        return String(buffer)
    }

    fun closeSocket() {
        output?.close()
        input?.close()
        socket?.close()
    }
}