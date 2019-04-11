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

    fun connectToServer() {
        try {
            socket = Socket(serverHost, serverPort)
            output = PrintWriter(socket!!.getOutputStream(), true)
            input = BufferedReader(InputStreamReader(socket?.getInputStream()))

            output?.println("Android client connected!")

        } catch (exception: Exception) {
            throw exception
        }
    }

    fun sendMessage(msg : String) {
        output?.println(msg)
    }

    fun getMessage() : String? {
        return input?.readLine()
    }
}