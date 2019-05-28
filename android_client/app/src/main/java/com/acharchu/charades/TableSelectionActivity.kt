package com.acharchu.charades

import android.annotation.SuppressLint
import android.content.Intent
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.widget.ArrayAdapter
import android.widget.Button
import android.widget.LinearLayout
import android.widget.Toast
import kotlinx.android.synthetic.main.activity_table_selection.*

class TableSelectionActivity : AppCompatActivity() {

    companion object {
        var IN_TABLE_VIEW = true
    }
    private val inputThread = Thread {

        while(ConnectionService.status == State.CONNECTED && IN_TABLE_VIEW) {
            val header : HeaderType? = ConnectionService.getHeader()

            if(header == HeaderType.SEE_TABLES) {
                val tableList = ConnectionService.getTableList()

                if (tableList != null)
                    setTableList(tableList)
            } else if (header == HeaderType.TABLE_CREATED) {
                val tableId = ConnectionService.getIdOfCreatedTable()
                runOnUiThread {
                    Toast.makeText(this, "Table of id $tableId created!", Toast.LENGTH_SHORT).show()
                }
                ConnectionService.listAvailableTables()
            } else if (header == HeaderType.COMMAND_FAILED) {
                ConnectionService.commandFailed()
                Toast.makeText(this, "Unable to join the table", Toast.LENGTH_SHORT).show()
                ConnectionService.listAvailableTables()
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportActionBar!!.hide()
        setContentView(R.layout.activity_table_selection)

        createTableButton.setOnClickListener {
            ConnectionService.createTable()
        }

        if(!inputThread.isAlive)
            inputThread.start()

        ConnectionService.listAvailableTables()

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
        ConnectionService.closeSocket()
        super.onBackPressed()
    }

    override fun onResume() {
        IN_TABLE_VIEW = true
        ConnectionService.listAvailableTables()
        super.onResume()
    }

}
