package com.acharchu.charades

import android.content.Intent
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import android.widget.Toast
import kotlinx.android.synthetic.main.activity_table_selection.*

class TableSelectionActivity : AppCompatActivity() {

    var IN_TABLE_VIEW = true

    private val inputThread = Thread {

        while(ConnectionService.status == State.CONNECTED && IN_TABLE_VIEW) {
            try {
                val header: HeaderType? = ConnectionService.getHeader()

                when (header) {
                    HeaderType.SEE_TABLES -> showTables()
                    HeaderType.TABLE_CREATED -> tableCreated()
                    HeaderType.COMMAND_FAILED -> errorDuringJoiningTable()
                    HeaderType.PING_PING -> responseToPing()
                }
            } catch (e: Throwable) {
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
        IN_TABLE_VIEW = false
        ConnectionService.status = State.DISCONNECTED

        if(ConnectionService.PROCESSING)
            ConnectionService.INTERRUPT = true

        ConnectionService.closeSocket()
    }

    private fun responseToPing() {
        ConnectionService.readZeros()
        ConnectionService.pong_pong()
    }

    private fun errorDuringJoiningTable() {
        ConnectionService.readZeros()
        Toast.makeText(this, "Unable to join the table", Toast.LENGTH_SHORT).show()
        ConnectionService.listAvailableTables()
    }

    private fun tableCreated() {
        val tableId = ConnectionService.getIdOfCreatedTable()
        runOnUiThread {
            Toast.makeText(this, "Table of id $tableId created!", Toast.LENGTH_SHORT).show()
        }
        ConnectionService.listAvailableTables()
    }

    private fun showTables() {
        val tableList = ConnectionService.getTableList()

        if (tableList != null)
            setTableList(tableList)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportActionBar!!.hide()
        setContentView(R.layout.activity_table_selection)

        createTableButton.setOnClickListener {
            ConnectionService.createTable()
        }

        scoresButton.setOnClickListener {
            goToStatistics()
        }

        reloadButton.setOnClickListener {
            ConnectionService.listAvailableTables()
        }

        if(!inputThread.isAlive)
            inputThread.start()

        ConnectionService.listAvailableTables()

    }

    fun waitForThreadToFinish() {
        inputThread.join()
    }

    private fun goToStatistics() {
        IN_TABLE_VIEW = false

        if(ConnectionService.PROCESSING)
            ConnectionService.INTERRUPT = true

        inputThread.join()
        ConnectionService.INTERRUPT = false
        ConnectionService.skipLeftovers()
        val intent = Intent(this, StatisticsActivity::class.java)
        startActivity(intent)
    }

    private fun setTableList(listOfIDs: List<Int>) {
        val listOfButtons = arrayListOf<Button>()

        for(id in listOfIDs) {
            val button = Button(this)
            button.id = id
            button.text = "Table $id"
            listOfButtons.add(button)
        }
        val adapter = ButtonAdapter(listOfButtons, this)

        runOnUiThread {
            tableListView.adapter = adapter
        }
    }

    override fun onBackPressed() {
        IN_TABLE_VIEW = false
        ConnectionService.INTERRUPT = true
        ConnectionService.skipLeftovers()

        ConnectionService.closeSocket()
        super.onBackPressed()
    }

    override fun onResume() {
        IN_TABLE_VIEW = true

        if(!inputThread.isAlive)
            inputThread.start()

        ConnectionService.listAvailableTables()
        super.onResume()
    }
}
