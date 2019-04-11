package sample;

import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.scene.control.ListCell;
import javafx.scene.control.ListView;
import javafx.scene.control.TextField;
import javafx.scene.input.KeyCode;
import sample.Model.Message;

import java.io.*;
import java.net.Socket;
import java.util.ArrayList;

public class Controller {

    private PrintWriter out;
    private BufferedReader in;

    @FXML
    private ListView messagesBox;

    @FXML
    private TextField messageField;

    @FXML
    public void initialize() {
        startChatReceiverTask();
        startChatWriterTask();
        messagesBox.setCellFactory(param -> new ListCell<String>() {
            @Override
            protected void updateItem(String item, boolean empty) {
                super.updateItem(item, empty);
                if (empty || item == null) {
                    setGraphic(null);
                    setText(null);
                    // other stuff to do...
                } else {
                    setPrefWidth(messagesBox.getWidth());
                    setWrapText(true);
                    setText(item);
                }
            }
        });
    }

    public Controller() {
        startConnection("127.0.0.1", 45655);
    }

    private void startConnection(String ip, int port) {
        try {
            Socket socket = new Socket(ip, port);
            socket.setTcpNoDelay(true);
            out = new PrintWriter(socket.getOutputStream(), true);
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            sendWelcomePackage();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void sendWelcomePackage() {
        String welcomePackage = "HELLO\ntokarz";
        out.println(welcomePackage);
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
                String messageType = in.readLine();
                System.out.println(messageType);
                if (!messageType.equals("WELCOME")) {
                    String user = in.readLine();
                    System.out.println(user);
                    int length = Integer.parseInt(in.readLine());
                    System.out.println(length);
                    if (length > 0) {
                        char[] message = new char[length];
                        in.read(message, 0, length);
                        System.out.println(new String(message));
                        Message msg = new Message(user, new String(message));
                        Platform.runLater(() -> {
                            messagesBox.getItems().add(messagesBox.getItems().size(), formatMessageToShow(msg));
                            messagesBox.scrollTo(messagesBox.getItems().size() - 1);
                        });
                    }
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private String formatMessageToShow(Message message) {
        StringBuilder stringBuilder = new StringBuilder(message.getAuthor() + ": " + message.getContent());
        return stringBuilder.toString();
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


    private void addMessage() {
        String text = messageField.getText();
        if (text.length() > 0) {
            Message message = new Message("Ja", text);
            sendMessage(message.getContent());
            Platform.runLater( () -> {
                messagesBox.getItems().add(messagesBox.getItems().size(), formatMessageToShow(message));
                messageField.clear();
            } );
        }
    }

    private void sendMessage(String message) {
        out.println("SEND_MESSAGE");
        out.println(message.length());
        out.println(message);
    }


}
