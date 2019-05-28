package com.acharchu.charades

import android.content.Context
import android.content.Intent
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import android.widget.Button
import android.widget.ListAdapter

class ButtonAdapter(val list: ArrayList<Button>, val context : Context) : BaseAdapter(),  ListAdapter{

    override fun getItem(position: Int): Any {
        return list[position]
    }

    override fun getItemId(position: Int): Long {
        return list[position].id.toLong()
    }

    override fun getCount(): Int {
        return list.size
    }

    override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
        var view = convertView
        if (view == null) {
            val inflater = context.getSystemService(Context.LAYOUT_INFLATER_SERVICE) as LayoutInflater
            view = inflater.inflate(R.layout.table_button, null)
        }

        val button = view!!.findViewById<Button>(R.id.table_button)
        button.text = "Table ${list[position].id}"

        button.setOnClickListener {
            ConnectionService.INTERRUPT = true
            TableSelectionActivity.IN_TABLE_VIEW = false

            ConnectionService.joinToTable(list[position].id)
            val intent = Intent(context, GameActivity::class.java)
            context.startActivity(intent) }

        return view

    }

}