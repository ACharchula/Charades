package com.acharchu.charades

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.os.StrictMode
import android.view.View.GONE
import android.view.View.VISIBLE
import android.widget.ArrayAdapter
import kotlinx.android.synthetic.main.activity_game.*

class GameActivity : AppCompatActivity() {

    private val connectionService : ConnectionService = ConnectionService()

    private val outputThread = Thread {
        sendButton.setOnClickListener {

            val msg : String = messageContent.text.toString()

            if(msg != "") {
                connectionService.sendMessage(msg)
                updateMessageList("Me: $msg")
                clearTextInput()
            }
        }
    }

    private val inputThread = Thread {

        while(connectionService.status == State.CONNECTED) {
            try{
                val header : HeaderType? = connectionService.getHeader()

                if (header == HeaderType.CHAT_MESSAGE)
                    updateMessageList(connectionService.getMessage())

            } catch (e : Throwable) {
                if (e.message == "CONNECTION CLOSED") {

                    connectionService.status = State.DISCONNECTED

                    runOnUiThread {
                        connectionService.closeSocket()
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
            connectionService.connectToServer()
            if(!inputThread.isAlive)
                inputThread.start()

            if(!outputThread.isAlive)
                outputThread.start()
        } catch (e: Throwable) {
            messagesListView.visibility = GONE
            sendButton.visibility = GONE
            messageContent.visibility = GONE
            reconnectButton.visibility = VISIBLE
        }
    }

    private fun printMessages() {
        val adapter = ArrayAdapter(this, R.layout.message, connectionService.messages)
        messagesListView.adapter = adapter
        messagesListView.setSelection(adapter.count - 1)
    }

    private fun updateMessageList(msg : String?) {
        runOnUiThread {
            connectionService.messages.add(msg!!)
            printMessages()
        }
    }

    private fun clearTextInput() {
        runOnUiThread {
            messageContent.setText("")
        }
    }
}
