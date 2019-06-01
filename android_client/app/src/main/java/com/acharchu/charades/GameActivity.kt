package com.acharchu.charades

import android.content.Intent
import android.graphics.Bitmap
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.view.View.*
import android.widget.ArrayAdapter
import android.widget.Toast
import kotlinx.android.synthetic.main.activity_game.*
import kotlin.concurrent.fixedRateTimer

class GameActivity : AppCompatActivity() {

    private var sendPicture = false
    private var pictureByteArray = ByteArray(0)
    private var IN_GAME = true
    private var drawerView = false
    var messages = arrayListOf<String>()

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

        while(ConnectionService.status == State.CONNECTED && IN_GAME) {
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
                } else if (header == HeaderType.GAME_ABORTED) {
                    if(drawerView)
                        guessingPlayerView()

                    updateMessageList(ConnectionService.gameAborted())
                } else if (header == HeaderType.PING_PING) {
                    ConnectionService.ping_ping()
                    ConnectionService.pong_pong()
                }

            } catch (e : Throwable) {
                if (e.message == "CONNECTION CLOSED") {

                    IN_GAME = false
                    ConnectionService.status = State.DISCONNECTED
                    ConnectionService.INTERRUPT = true
                    ConnectionService.closeSocket()

                    runOnUiThread {
                        val intent = Intent(this, MainActivity::class.java)
                        intent.flags = Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_CLEAR_TASK
                        startActivity(intent)
                        ConnectionService.INTERRUPT = false
                        Toast.makeText(this, "Disconnected from server!", Toast.LENGTH_SHORT).show()

                    }
                }

            }
        }
    }

    override fun onBackPressed() {
        IN_GAME = false
        ConnectionService.INTERRUPT = true
        ConnectionService.skipLeftovers()
        ConnectionService.comeOutFromTable()
        super.onBackPressed()
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportActionBar!!.hide()
        setContentView(R.layout.activity_game)

        giveUpButton.setOnClickListener {
            ConnectionService.giveUpAGame()
            guessingPlayerView()
        }
        guessingPlayerView()
        connectToServer()
    }

    private fun drawerView() {
        drawerView = true

        runOnUiThread {
            sendButton.visibility = GONE
            messageContent.visibility = GONE
            imageView.visibility = INVISIBLE
            draw_view.clearCanvas()
            draw_view.visibility = VISIBLE
            giveUpButton.visibility = VISIBLE
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
            imageView.setImageResource(android.R.color.white)
            imageView.visibility = VISIBLE
            draw_view.visibility = INVISIBLE
            giveUpButton.visibility = GONE
        }

        sendPicture = false
    }

    private fun connectToServer() {
        if(!inputThread.isAlive)
            inputThread.start()

        if(!outputThread.isAlive)
            outputThread.start()
    }

    private fun printMessages() {
        val adapter = ArrayAdapter(this, R.layout.message, messages)
        messagesListView.adapter = adapter
        messagesListView.setSelection(adapter.count - 1)
    }



    private fun updateMessageList(msg : String?) {
        runOnUiThread {
            messages.add(msg!!)
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
