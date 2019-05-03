package com.acharchu.charades

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.Picture
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
            var length = msg.length
            var result = socketChannel!!.write(ByteBuffer.wrap(msg.toByteArray()))
            var a = 0
        }

        private fun sendByteArray(byteArray: ByteArray) {
            var length = byteArray.size
            var result = socketChannel!!.write(ByteBuffer.wrap(byteArray))
            var a = 0
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

            return when {
                length < 10 -> "000$length"
                length < 100 -> "00$length"
                length < 1000 -> "0$length"
                length < 10000 -> length.toString()
                //out of bound error
                else -> ""
            }

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

        fun getGameWaiting(): String {
            readByteArray(BYTES_TO_READ_LENGTH)
            return "The game has not started yet."
        }

        fun getTheWinner(): String {
            return "CORRECT ANSWER! ${getMessage()}"

        }

        fun getGameReady(): String{
            val length = read(BYTES_TO_READ_LENGTH)
            var result = read(length.toInt())

            return "The drawer is - $result"
        }

        fun getThingToDraw() : String {
            var length = read(BYTES_TO_READ_LENGTH)
            var result = read(length.toInt())
            return "Your turn! Draw - $result"
        }

        fun clueCorrect() {
            read(BYTES_TO_READ_LENGTH)
        }

        fun clueIncorrect() {
            read(BYTES_TO_READ_LENGTH)
        }

        fun sendPicture(picture: ByteArray) {
            var header = "SET___CANVAS${preparePictureLength(picture.size)}".toByteArray()
            header += picture
            sendByteArray(header)
        }

        fun preparePictureLength(length: Int): String {
            val builder = StringBuilder()

            builder.append(length)

            return when {
                length < 10 -> "0000000$length"
                length < 100 -> "000000$length"
                length < 1000 -> "00000$length"
                length < 10000 -> "0000$length"
                length < 100000 -> "000$length"
                length < 1000000 -> "00$length"
                length < 10000000 -> "0$length"
                length < 100000000 -> length.toString()
                //out of bound error
                else -> throw Exception("The length of the picture is too big!")
            }

        }
    }
}