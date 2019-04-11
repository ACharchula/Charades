package sample;

import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.scene.control.ListView;
import javafx.scene.control.TextField;
import javafx.scene.input.KeyCode;
import sample.Model.Message;

import java.io.*;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Arrays;

public class Controller {

    private Socket socket;
    private PrintWriter out;
    private DataInputStream in;

    private ArrayList<Message> messages = new ArrayList<>();

    @FXML
    private ListView messagesBox;

    @FXML
    private TextField messageField;

    public Controller() {

        startConnection("127.0.0.1", 44444);
        startChatReceiverTask();
    }

    private void startChatReceiverTask(){
        Runnable task = this::runTask;

        Thread backgroundThread = new Thread(task);
        backgroundThread.setDaemon(true);
        backgroundThread.start();
    }

    private void runTask(){
        try{
            while(true){
                byte[] lengthBytes = new byte[4];
                in.readFully(lengthBytes, 0, 4);
                System.out.println(Arrays.toString(lengthBytes));
                System.out.println(new String(lengthBytes));
                int length = Integer.parseInt(new String(lengthBytes));
                System.out.println(length);
                if(length > 0){
                    byte[] message = new byte[length];
                    in.readFully(message, 0, length);
                    System.out.println(new String(message));
                    in.read(); // eat new line char
                    Platform.runLater(() -> messagesBox.getItems().add(messagesBox.getItems().size(), new String(message)));
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @FXML
    public void initialize() {

        System.out.println("initialize");

        messageField.setOnKeyPressed(keyEvent -> {
            if (keyEvent.getCode() == KeyCode.ENTER) {
                addMessage(messageField.getText());
            }
        });
    }

    private void addMessage(String text) {
        if (text.length() > 0) {
            Message message = new Message("Tokarz", text);
            messages.add(message);
            WritingChatMessageThread thread = new WritingChatMessageThread(message.getContent());
            thread.start();
            StringBuilder stringBuilder = new StringBuilder();
            stringBuilder.append(message.getAuthor() + ": " + message.getContent());
            messagesBox.getItems().add(messagesBox.getItems().size(), stringBuilder.toString());
            messageField.clear();
        }
    }

    public void startConnection(String ip, int port) {
        try {
            socket = new Socket(ip, port);
            socket.setTcpNoDelay(true);
            out = new PrintWriter(socket.getOutputStream(), true);
            in = new DataInputStream(socket.getInputStream());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    class WritingChatMessageThread extends Thread {
        private String message;

        public WritingChatMessageThread(String message) {
            this.message = message;
        }

        public void run() {
            try {
                out.println(message.length());
                out.println(message);

            } catch (Exception e) {
                System.out.println("Exception in writer is caught");
            } finally {
                Thread.currentThread().interrupt();//preserve the message
                return;//Stop doing whatever I am doing and terminate
            }
        }
    }


}
