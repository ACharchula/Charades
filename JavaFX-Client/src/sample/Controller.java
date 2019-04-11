package sample;

import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.scene.control.ListView;
import javafx.scene.control.TextField;
import javafx.scene.input.KeyCode;
import sample.Model.Message;

import java.io.*;
import java.net.Socket;
import java.nio.Buffer;
import java.util.ArrayList;
import java.util.Arrays;

public class Controller {

    private PrintWriter out;
    private BufferedReader in;

    @FXML
    private ListView messagesBox;

    @FXML
    private TextField messageField;

    public Controller() {
        startConnection("127.0.0.1", 44444);
    }

    private void startConnection(String ip, int port) {
        try {
            Socket socket = new Socket(ip, port);
            socket.setTcpNoDelay(true);
            out = new PrintWriter(socket.getOutputStream(), true);
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void startChatReceiverTask() {
        Runnable task = this::runChatReceiverTask;

        Thread backgroundThread = new Thread(task);
        backgroundThread.setDaemon(true);
        backgroundThread.start();
    }

    private void runChatReceiverTask() {
        try {
            while (true) {
                int length = Integer.parseInt(in.readLine());
                System.out.println(length);
                if (length > 0) {
                    char[] message = new char[length];
                    in.read(message, 0, length);
                    System.out.println(new String(message));
                    in.read(); // eat new line char
                    Platform.runLater(() -> messagesBox.getItems().add(messagesBox.getItems().size(), new String(message)));
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void startChatWriterTask() {
        Runnable task = this::runChatWriterTask;

        Thread backgroundThread = new Thread(task);
        backgroundThread.setDaemon(true);
        backgroundThread.start();
    }

    private void runChatWriterTask() {

        messageField.setOnKeyPressed(keyEvent -> {
            if (keyEvent.getCode() == KeyCode.ENTER) {
                addMessage();
            }
        });

    }

    @FXML
    public void initialize() {
        startChatReceiverTask();
        startChatWriterTask();
    }

    private void addMessage() {
        String text = messageField.getText();
        if (text.length() > 0) {
            Message message = new Message("Tokarz", text);
            sendMessage(message.getContent());
            StringBuilder stringBuilder = new StringBuilder();
            stringBuilder.append(message.getAuthor() + ": " + message.getContent());
            messagesBox.getItems().add(messagesBox.getItems().size(), stringBuilder.toString());
            messageField.clear();
        }
    }

    private void sendMessage(String message) {
        out.println(message.length());
        out.println(message);
    }



}
