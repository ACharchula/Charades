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
    private var messages = arrayListOf<String>()

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

                when (header) {
                    HeaderType.CHAT_MESSAGE -> updateMessageList(ConnectionService.getMessage())
                    HeaderType.UPDATECANVAS -> updateCanvas(ConnectionService.getCanvas())
                    HeaderType.GAME_WAITING -> updateMessageList(ConnectionService.getGameWaiting())
                    HeaderType.GAME_ENDED -> endGame()
                    HeaderType.GAME_READY -> updateMessageList(ConnectionService.getGameReady())
                    HeaderType.YOU_ARE_DRAWER -> chosenAsDrawer()
                    HeaderType.GAME_ABORTED -> gameAborted()
                    HeaderType.PING_PING -> responseToPing()
                }

            } catch (e : Throwable) {
                if (e.message == "CONNECTION CLOSED") {

                    endBackgroundProcesses()

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

    private fun endBackgroundProcesses() {
        IN_GAME = false
        ConnectionService.status = State.DISCONNECTED
        ConnectionService.INTERRUPT = true
        ConnectionService.closeSocket()
    }

    private fun responseToPing() {
        ConnectionService.readZeros()
        ConnectionService.pong_pong()
    }

    private fun gameAborted() {
        if(drawerView)
            guessingPlayerView()

        updateMessageList(ConnectionService.gameAborted())
    }

    private fun chosenAsDrawer() {
        updateMessageList(ConnectionService.getThingToDraw())
        drawerView()
    }

    private fun endGame() {
        updateMessageList(ConnectionService.getTheWinner())
        guessingPlayerView()
    }

    override fun onBackPressed() {
        draw_view.clearCanvas()
        imageView.setImageResource(android.R.color.white)

        sendPicture = false
        IN_GAME = false
        if(ConnectionService.PROCESSING)
            ConnectionService.INTERRUPT = true

        inputThread.join()
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

        clearDrawViewButton.setOnClickListener {
            draw_view.clearCanvas()
        }

        guessingPlayerView()
        startThreads()
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
            clearDrawViewButton.visibility = VISIBLE
        }

        sendPicture = true


        fixedRateTimer("sendPicture", false, 2000L, 100) {
            if(!sendPicture) {
                cancel()
            } else {
                prepareAndSendPicture()
            }
        }
    }

    private fun guessingPlayerView() {
        sendPicture = false

        runOnUiThread {
            sendButton.visibility = VISIBLE
            messageContent.visibility = VISIBLE
            imageView.setImageResource(android.R.color.white)
            imageView.visibility = VISIBLE
            draw_view.visibility = INVISIBLE
            giveUpButton.visibility = GONE
            clearDrawViewButton.visibility = GONE
        }
    }

    private fun startThreads() {
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
