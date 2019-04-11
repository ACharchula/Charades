package com.acharchu.charades

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import kotlinx.android.synthetic.main.activity_main.*
import android.os.StrictMode
import android.view.View.GONE
import android.view.View.VISIBLE
import android.widget.ArrayAdapter


class MainActivity : AppCompatActivity() {

    private val app : App = App()

    private val outputThread = Thread {
        println("Output thread has started")

        sendButton.setOnClickListener {

            val msg : String = messageContent.text.toString()

            if(msg != "") {
                app.sendMessage(msg)
                updateMessageList("Me: $msg")
                clearTextInput()
            }
        }
    }

    private val inputThread = Thread {
        println("Input thread has started")

        while(true) {
            val msg : String? = app.getMessage()

            if (msg != null && msg != "") {
                updateMessageList(msg)
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val policy = StrictMode.ThreadPolicy.Builder().permitAll().build()
        StrictMode.setThreadPolicy(policy)

        setContentView(R.layout.activity_main)

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
            app.connectToServer()
            inputThread.start()
            outputThread.start()
        } catch (exception : java.lang.Exception) {
            messagesListView.visibility = GONE
            sendButton.visibility = GONE
            messageContent.visibility = GONE
            reconnectButton.visibility = VISIBLE
        }
    }

    private fun printMessages() {
        val adapter = ArrayAdapter(this, R.layout.message, app.messages)
        messagesListView.adapter = adapter
        messagesListView.setSelection(adapter.count - 1)
    }

    private fun updateMessageList(msg : String) {
        runOnUiThread {
            app.messages.add(msg)
            printMessages()
        }
    }

    private fun clearTextInput() {
        runOnUiThread {
            messageContent.setText("")
        }
    }


}
