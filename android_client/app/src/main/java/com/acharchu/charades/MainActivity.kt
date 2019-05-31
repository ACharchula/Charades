package com.acharchu.charades

import android.content.Intent
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.os.StrictMode
import android.widget.Toast
import kotlinx.android.synthetic.main.activity_main.*


class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportActionBar!!.hide()
        val policy = StrictMode.ThreadPolicy.Builder().permitAll().build()
        StrictMode.setThreadPolicy(policy)

        setContentView(R.layout.activity_main)

        startButton.setOnClickListener {
            start()
        }
    }

    private fun start() {
        ConnectionService.connectToServer()
        login()
    }

    private fun login() {
        ConnectionService.setId(nameTextInput.text.toString())
        val result = ConnectionService.performServerHandshake()

        if(!result) {
            Toast.makeText(this, "Id ${nameTextInput.text} is already taken!", Toast.LENGTH_SHORT).show()
            ConnectionService.closeSocket()
        }
        else {
            val intent = Intent(this, TableSelectionActivity::class.java)
            startActivity(intent)
        }
    }
}
