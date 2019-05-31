package com.acharchu.charades

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.widget.ArrayAdapter
import android.widget.Toast
import kotlinx.android.synthetic.main.activity_statistics.*

class StatisticsActivity : AppCompatActivity() {

    var IN_STATISTICS_VIEW = true

    private val inputThread = Thread {

        while(ConnectionService.status == State.CONNECTED && IN_STATISTICS_VIEW) {
            val header : HeaderType? = ConnectionService.getHeader()

            if(header == HeaderType.SEE_STATISTIC) {
                setStatistics(ConnectionService.getBestUsersAndScores())
            }  else if (header == HeaderType.PING_PING) {
                ConnectionService.ping_ping()
                ConnectionService.pong_pong()
            }
        }
    }

    override fun onBackPressed() {
        IN_STATISTICS_VIEW = false
        ConnectionService.INTERRUPT = true
        ConnectionService.skipLeftovers()
        super.onBackPressed()
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportActionBar!!.hide()
        setContentView(R.layout.activity_statistics)

        if(!inputThread.isAlive)
            inputThread.start()

        ConnectionService.getStatistics()
    }

    private fun setStatistics(usersAndScores : List<String>?) {
        if(usersAndScores != null) {
            val adapter = ArrayAdapter(this, R.layout.message, usersAndScores)
            scoresListView.adapter = adapter
        }
    }

}
