package com.acharchu.charades

import android.graphics.Bitmap
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.os.StrictMode
import android.view.View.GONE
import android.view.View.VISIBLE
import android.widget.ArrayAdapter
import kotlinx.android.synthetic.main.activity_game.*
import kotlinx.android.synthetic.main.activity_table_selection.*
import kotlin.concurrent.fixedRateTimer

class GameActivity : AppCompatActivity() {

    private var sendPicture = false
    private var pictureByteArray = ByteArray(0)

    private val outputThread = Thread {
        sendButton.setOnClickListener {

            val msg : String = messageContent.text.toString()

            if(msg != "") {
                ConnectionService.sendMessage(msg)
                updateMessageList("Me: $msg")
                clearTextInput()
            }
        }
    }

    private val inputThread = Thread {

        while(ConnectionService.status == State.CONNECTED) {
            try{
                val header : HeaderType? = ConnectionService.getHeader()

                if (header == HeaderType.CHAT_MESSAGE)
                    updateMessageList(ConnectionService.getMessage())
                else if (header == HeaderType.UPDATECANVAS)
                    updateCanvas(ConnectionService.getCanvas())
                else if (header == HeaderType.GAME_WAITING)
                    updateMessageList(ConnectionService.getGameWaiting())
                else if (header == HeaderType.GAME_ENDED) {
                    updateMessageList(ConnectionService.getTheWinner())
                    guessingPlayerView()
                }
                else if (header == HeaderType.GAME_READY)
                    updateMessageList(ConnectionService.getGameReady())
                else if (header == HeaderType.YOU_ARE_DRAWER) {
                    updateMessageList(ConnectionService.getThingToDraw())
                    drawerView()
                }
                else if (header == HeaderType.CLUE_CORRECT)
                    ConnectionService.clueCorrect()
                else if (header == HeaderType.CLUE_INCORRECT)
                    ConnectionService.clueIncorrect()

            } catch (e : Throwable) {
                if (e.message == "CONNECTION CLOSED") {

                    ConnectionService.status = State.DISCONNECTED

                    runOnUiThread {
                        ConnectionService.closeSocket()
                        messagesListView.visibility = GONE
                        sendButton.visibility = GONE
                        messageContent.visibility = GONE
                        reconnectButton.visibility = VISIBLE
                    }
                }

            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContentView(R.layout.activity_game)

        reconnectButton.setOnClickListener {
            reconnectButton.visibility = GONE
            messagesListView.visibility = VISIBLE
            sendButton.visibility = VISIBLE
            messageContent.visibility = VISIBLE
            connectToServer()
        }

        connectToServer()
    }

    private fun drawerView() {
        runOnUiThread {
            sendButton.visibility = GONE
            messageContent.visibility = GONE
            imageView.visibility = GONE
            draw_view.clearCanvas()
            draw_view.visibility = VISIBLE
        }

        sendPicture = true


        fixedRateTimer("sendPicture", false, 2000L, 100) {
            prepareAndSendPicture()
            if(!sendPicture)
                cancel()
        }
    }

    private fun guessingPlayerView() {
        runOnUiThread {
            sendButton.visibility = VISIBLE
            messageContent.visibility = VISIBLE
            imageView.visibility = VISIBLE
            draw_view.visibility = GONE
        }

        sendPicture = false
    }

    private fun connectToServer() {
        try {
            ConnectionService.connectToServer()
            if(!inputThread.isAlive)
                inputThread.start()

            if(!outputThread.isAlive)
                outputThread.start()
            ConnectionService.joinToTable()
        } catch (e: Throwable) {
            messagesListView.visibility = GONE
            sendButton.visibility = GONE
            messageContent.visibility = GONE
            reconnectButton.visibility = VISIBLE
        }
    }

    private fun printMessages() {
        val adapter = ArrayAdapter(this, R.layout.message, ConnectionService.messages)
        messagesListView.adapter = adapter
        messagesListView.setSelection(adapter.count - 1)
    }

    private fun updateMessageList(msg : String?) {
        runOnUiThread {
            ConnectionService.messages.add(msg!!)
            printMessages()
        }
    }

    private fun updateCanvas(bitmap: Bitmap) {
        runOnUiThread {
            imageView.setImageBitmap(bitmap)
        }
    }

    private fun clearTextInput() {
        runOnUiThread {
            messageContent.setText("")
        }
    }

    private fun prepareAndSendPicture() {
            if(!pictureByteArray.contentEquals(draw_view.getByteArray())) {
                pictureByteArray = draw_view.getByteArray()
                ConnectionService.sendPicture(draw_view.getByteArray())
            }

    }
}
