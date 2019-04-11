package sample;

import javafx.fxml.FXML;
import javafx.scene.control.ListView;
import javafx.scene.control.TextField;
import javafx.scene.input.KeyCode;
import sample.Model.Message;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.ArrayList;

public class Controller {

    private Socket socket;
    private PrintWriter out;
    private BufferedReader in;

    private ArrayList<Message> messages = new ArrayList<>();

    @FXML
    private ListView messagesBox;

    @FXML
    private TextField messageField;

    public Controller(){
        System.out.println("Controller");
    }

    @FXML
    public void initialize(){

        System.out.println("initialize");

        messageField.setOnKeyPressed(keyEvent -> {
            if (keyEvent.getCode() == KeyCode.ENTER)  {
                String text = messageField.getText();
                if(text.length() > 0){
                    Message message = new Message("Tokarz", text);
                    messages.add(message);
                    StringBuilder stringBuilder = new StringBuilder();
                    stringBuilder.append(message.getAuthor() + ": " + message.getContent());
                    messagesBox.getItems().add(messagesBox.getItems().size(), stringBuilder.toString());
                    messageField.clear();
                }
            }
        });
    }

    public void startConnection(String ip, int port) {
        try {
            socket = new Socket(ip, port);
            out = new PrintWriter(socket.getOutputStream(), true);
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public String sendMessage(String msg){
        out.println(msg);
        String resp = null;
        try {
            resp = in.readLine();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return resp;
    }


}
