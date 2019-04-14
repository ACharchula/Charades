package sample;

import javafx.application.Platform;
import javafx.concurrent.Task;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.geometry.Pos;
import javafx.scene.control.ListCell;
import javafx.scene.control.ListView;
import javafx.scene.control.TextField;
import javafx.scene.control.Button;
import javafx.scene.input.KeyCode;
import javafx.scene.layout.VBox;
import sample.Model.Message;

import java.io.*;

public class Controller {

    private ConnectionService connectionService;
    private String IP = "127.0.0.1";
    private int PORT = 44001;
    private Button reconnectButton;
    private Thread readingThread;
    private Thread writingThread;

    @FXML
    private ListView messagesBox;

    @FXML
    private TextField messageField;

    @FXML
    private VBox chatBox;

    @FXML
    public void initialize() {

        messagesBox.setCellFactory(param -> new ListCell<String>() {
            @Override
            protected void updateItem(String item, boolean empty) {
                super.updateItem(item, empty);
                if (empty || item == null) {
                    setGraphic(null);
                    setText(null);
                } else {
                    setPrefWidth(messagesBox.getWidth());
                    setWrapText(true);
                    setText(item);
                }
            }
        });

        reconnectButton = new Button("reconnect");
        reconnectButton.setOnMouseClicked(event -> {
            connectionService = new ConnectionService(IP, PORT);
            if (connectionService.isConnected()) {
                showChatBox();
            }
        });

        if (connectionService.isConnected()) {
            showChatBox();
        } else {
            showReconnectButton();
        }
    }

    public Controller() {
        connectionService = new ConnectionService(IP, PORT);
    }

    private void startChatReceiverTask() {
        System.out.println("starting reading thread");
        Runnable task = this::runChatReceiverTask;

        readingThread = new Thread(task);
        readingThread.setDaemon(true);
        readingThread.start();
    }

    private void runChatReceiverTask() {
        try {
            while (connectionService.isConnected()) {
                String header = connectionService.read("CHAT_MESSAGE".length());
                System.out.println(header);
                if (header.equals("CHAT_MESSAGE")) {
                    Message message = connectionService.getMessage();
                    Platform.runLater(() -> {
                        messagesBox.getItems().add(messagesBox.getItems().size(), formatMessageToShow(message));
                        messageField.clear();
                    });
                }
            }
        } catch (IOException e) {
            Platform.runLater(this::showReconnectButton);
        }
    }

    private String formatMessageToShow(Message message) {
        StringBuilder stringBuilder = new StringBuilder(message.getAuthor() + ": " + message.getContent());
        return stringBuilder.toString();
    }

    private void startChatWriterTask() {
        System.out.println("starting writing thread");
        Runnable task = this::runChatWriterTask;

        writingThread = new Thread(task);
        writingThread.setDaemon(true);
        writingThread.start();

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
            connectionService.sendMessage(message.getContent());
            Platform.runLater(() -> {
                messagesBox.getItems().add(messagesBox.getItems().size(), formatMessageToShow(message));
                messageField.clear();
            });
        }
    }

    private void showChatBox() {
        startChatReceiverTask();
        startChatWriterTask();
        chatBox.getChildren().forEach(child -> child.setVisible(true));
        chatBox.getChildren().remove(reconnectButton);
    }

    private void showReconnectButton() {
        try {
            if(connectionService.isConnected()){
                connectionService.closeSocket();
            }
        } catch (IOException e) {
            connectionService = null;
        }
        chatBox.getChildren().forEach(child -> child.setVisible(false));
        chatBox.setAlignment(Pos.TOP_CENTER);
        chatBox.getChildren().add(reconnectButton);
    }

}
