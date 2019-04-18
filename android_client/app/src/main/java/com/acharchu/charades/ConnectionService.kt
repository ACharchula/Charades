package com.acharchu.charades

import java.io.BufferedReader
import java.io.InputStreamReader
import java.io.PrintWriter
import java.lang.StringBuilder
import java.net.Socket

class ConnectionService {

    private val serverHost = "10.0.2.2" //has to be changed if u want to run on real smartphone
    private val serverPort = 44444
    private var socket: Socket? = null
    private var output: PrintWriter? = null
    private var input: BufferedReader? = null
    var messages = arrayListOf<String>()

    private val HEADER_LENGTH = 12
    private val BYTES_TO_READ_LENGTH = 4

    var status = State.DISCONNECTED

    private val id : Int = (10..99).random()

    fun connectToServer() {
        try {
            socket = Socket(serverHost, serverPort)

            output = PrintWriter(socket!!.getOutputStream(), false)
            input = BufferedReader(InputStreamReader(socket?.getInputStream()))

            hello()

        } catch (exception: Exception) {
            throw exception
        }
    }

    private fun send(msg : String) {
        //output?.write(msg)
        output?.print(msg)
        output?.flush()
        //output?.write(msg)
    }


    private fun hello() {
        send("HELLO_SERVER0015AndroidClient$id")
        read(16)
    }

    fun sendMessage(msg : String) {
        send("SEND_MESSAGE${prepareMessageLength(msg.length)}$msg")
    }

    fun prepareMessageLength(length:Int) : String {
        val builder = StringBuilder()

        builder.append(length)

        if(length < 10)
            return "000$length"
        else if(length < 100)
            return "00$length"
        else if(length < 1000)
            return "0$length"
        else if (length < 10000)
            return length.toString()

        return ""
        //out of bound error

    }

    fun getMessage() : String? {

        val usernameAndMessageLength = read(BYTES_TO_READ_LENGTH)
        val usernameAndMessage = read(usernameAndMessageLength.toInt()).split('\n')

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