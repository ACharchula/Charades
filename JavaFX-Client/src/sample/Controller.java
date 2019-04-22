package sample;

import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.geometry.Pos;
import javafx.scene.canvas.Canvas;
import javafx.scene.control.*;
import javafx.scene.input.KeyCode;
import javafx.scene.layout.VBox;
import sample.Model.HeaderType;
import sample.Model.Message;

import java.io.*;

public class Controller {

    private ConnectionService connectionService;
    private String IP = "127.0.0.1";
    private int PORT = 44444;
    private Button reconnectButton;
    private Thread readingThread;
    private Thread writingThread;
    private DrawingController drawingController;

    @FXML
    private ListView messagesBox;

    @FXML
    private TextField messageField;

    @FXML
    private VBox chatBox;

    @FXML
    private Canvas canvas;

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

        drawingController = new DrawingController(canvas);
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
                HeaderType header = connectionService.readHeader();
                System.out.println(header);
                if (header == HeaderType.CHAT_MESSAGE) {
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

            try {
                connectionService.sendMessage(message.getContent());

                Platform.runLater(() -> {
                    messagesBox.getItems().add(messagesBox.getItems().size(), formatMessageToShow(message));
                    messageField.clear();
                });

            } catch (IOException e) {
                showAlert("SEND_ERROR");
            }

        }
    }

    private void showAlert(String type){
        Alert alert = new Alert(Alert.AlertType.ERROR);

        if(type.equals("SEND_ERROR")){
            alert.setTitle("Sending message error");
            alert.setHeaderText("Oops, something went wrong");
            alert.setContentText("Please try to send that message again");
        }

        alert.showAndWait();
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