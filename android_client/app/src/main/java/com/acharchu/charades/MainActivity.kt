package com.acharchu.charades

import android.content.Intent
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.os.StrictMode
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
        ConnectionService.setId(nameTextInput.text.toString())
        ConnectionService.performServerHandshake()

        //while if not connected

        val intent = Intent(this, TableSelectionActivity::class.java)
        startActivity(intent)
    }


}
