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

class GameActivity : AppCompatActivity() {

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
                else if (header == HeaderType.GAME_ENDED)
                    updateMessageList(ConnectionService.getTheWinner())
                else if (header == HeaderType.GAME_READY)
                    updateMessageList(ConnectionService.getGameReady())
                else if (header == HeaderType.YOU_ARE_DRAWER)
                    updateMessageList(ConnectionService.getThingToDraw())
                else if (header == HeaderType.CLUE_CORRECT)
                    updateMessageList(ConnectionService.clueCorrect())
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

//        val policy = StrictMode.ThreadPolicy.Builder().permitAll().build()
//        StrictMode.setThreadPolicy(policy)

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
        imageView.setImageBitmap(bitmap)
    }

    private fun clearTextInput() {
        runOnUiThread {
            messageContent.setText("")
        }
    }
}
