package com.acharchu.charades

import android.content.Intent
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.ArrayAdapter
import android.widget.Toast
import kotlinx.android.synthetic.main.activity_statistics.*

class StatisticsActivity : AppCompatActivity() {

    var IN_STATISTICS_VIEW = true

    private val inputThread = Thread {
        Log.i("info", "thread statistic")
        while(ConnectionService.status == State.CONNECTED && IN_STATISTICS_VIEW) {
            try {
                val header: HeaderType? = ConnectionService.getHeader()

                if (header == HeaderType.SEE_STATISTIC) {
                    setStatistics(ConnectionService.getBestUsersAndScores())
                } else if (header == HeaderType.PING_PING) {
                    ConnectionService.ping_ping()
                    ConnectionService.pong_pong()
                }
            } catch (e: Throwable) {
                if (e.message == "CONNECTION CLOSED") {

                    IN_STATISTICS_VIEW = false
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
        IN_STATISTICS_VIEW = false
        if(ConnectionService.PROCESSING)
            ConnectionService.INTERRUPT = true
        inputThread.join()
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
