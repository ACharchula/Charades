package com.acharchu.charades

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import java.lang.StringBuilder
import java.net.InetAddress
import java.net.InetSocketAddress
import java.nio.ByteBuffer
import java.nio.channels.SocketChannel

class ConnectionService {

    companion object {
        private val serverHost = "10.0.2.2" //has to be changed if u want to run on real smartphone
        private val serverPort = 44444
        private var socketChannel: SocketChannel? = null
        var messages = arrayListOf<String>() // needs to be moved

        private val HEADER_LENGTH = 12
        private val BYTES_TO_READ_LENGTH = 4

        var status = State.DISCONNECTED

        private val id: Int = (10..99).random()

        fun connectToServer() {
            try {
                val address = InetSocketAddress(InetAddress.getByName(serverHost), serverPort)

                socketChannel = SocketChannel.open(address)
                socketChannel!!.configureBlocking(false)
                performServerHandshake()

            } catch (exception: Exception) {
                throw exception
            }
        }

        private fun send(msg: String) {
            socketChannel!!.write(ByteBuffer.wrap(msg.toByteArray()))
        }


        private fun performServerHandshake() {
            send("HELLO_SERVER0015AndroidClient$id")
            val handshakeResult = read(16) //check if result is ok

            if (handshakeResult == "WELCOME_USER0000")
                status = State.CONNECTED
            else
                throw Throwable("CANNOT CONNECT TO SERVER")
        }

        fun joinToTable() {
            send("ENTER__TABLE0000")
        }

        fun sendMessage(msg: String) {
            send("SEND_MESSAGE${prepareMessageLength(msg.length)}$msg")
        }

        fun prepareMessageLength(length: Int): String {
            val builder = StringBuilder()

            builder.append(length)

            if (length < 10)
                return "000$length"
            else if (length < 100)
                return "00$length"
            else if (length < 1000)
                return "0$length"
            else if (length < 10000)
                return length.toString()

            return ""
            //out of bound error

        }

        fun getMessage(): String? {

            val usernameAndMessageLength = read(BYTES_TO_READ_LENGTH)
            val usernameAndMessage = read(usernameAndMessageLength.toInt()).split('\n')

            return "${usernameAndMessage[0]}: ${usernameAndMessage[1]}"
        }

        fun getHeader(): HeaderType? {
            val header = read(HEADER_LENGTH)

            if (Headers.headers.containsKey(header))
                return Headers.headers[header]

            return HeaderType.UNDEFINED
        }

        private fun read(bytesToRead: Int): String {
            return String(readByteArray(bytesToRead))
        }

        private fun readByteArray(bytesToRead: Int): ByteArray {
            var amountOfCharacters = 0
            var consumedCharacters = 0
            var result: ByteArray = ByteArray(0)
            val buffer = ByteBuffer.allocate(bytesToRead)

            while (amountOfCharacters != bytesToRead && consumedCharacters != -1) {
                buffer.clear()
                consumedCharacters = socketChannel?.read(buffer)!!

                if (consumedCharacters != 0) {
                    result += buffer.array()
                    amountOfCharacters += consumedCharacters
                }
            }

            if (consumedCharacters == -1)
                throw Throwable("CONNECTION CLOSED")

            return result
        }

        fun closeSocket() {
            socketChannel!!.close()
        }

        fun getCanvas() : Bitmap {
            var length = read(BYTES_TO_READ_LENGTH*2)
            var bitmapByteArray = readByteArray(length.toInt())
            return BitmapFactory.decodeByteArray(bitmapByteArray, 0, bitmapByteArray.size)
        }

        fun getGameWaiting() {
            readByteArray(BYTES_TO_READ_LENGTH)
        }

        fun getTheWinner() {
            getMessage()
        }

        fun getGameReady() {
            val length = read(BYTES_TO_READ_LENGTH)
            var result = read(length.toInt())
        }
    }
}