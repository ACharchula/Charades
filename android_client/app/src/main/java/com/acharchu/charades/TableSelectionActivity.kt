package com.acharchu.charades

import android.content.Intent
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import kotlinx.android.synthetic.main.activity_table_selection.*

class TableSelectionActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportActionBar!!.hide()
        setContentView(R.layout.activity_table_selection)

        joinTableButton.setOnClickListener {
            startGame()
        }
    }

    override fun onBackPressed() {
        ConnectionService.closeSocket()
        super.onBackPressed()
    }

    private fun startGame() {
        ConnectionService.joinToTable()
        val intent = Intent(this, GameActivity::class.java)
        startActivity(intent)
    }
}
