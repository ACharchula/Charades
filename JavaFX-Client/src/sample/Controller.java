package sample;

import com.sun.xml.internal.ws.api.message.Header;
import javafx.application.Platform;
import javafx.embed.swing.SwingFXUtils;
import javafx.fxml.FXML;
import javafx.geometry.Pos;
import javafx.scene.canvas.Canvas;
import javafx.scene.control.*;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.image.WritableImage;
import javafx.scene.input.KeyCode;
import javafx.scene.layout.VBox;
import sample.Model.HeaderType;
import sample.Model.Message;

import java.io.*;
import java.util.Optional;
import java.util.Timer;
import java.util.TimerTask;

public class Controller {

    private ConnectionService connectionService;
    private String IP = "127.0.0.1";
    private int PORT = 44444;
    private Button reconnectButton;
    private Thread readingThread;
    private Thread writingThread;
    private DrawingController drawingController;
    private Timer timer = new Timer();

    @FXML
    private ListView messagesBox;

    @FXML
    private TextField messageField;

    @FXML
    private VBox chatBox;

    @FXML
    private ImageView imageView;

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
            reconnectButton.setDisable(true);
            initConnection();
//            connectionService = new ConnectionService(IP, PORT);
            if (connectionService.isConnected()) {
                showChatBox();
            } else {
                showAlert("CONNECT_ERROR");
            }
            reconnectButton.setDisable(false);
        });

//        showReconnectButton();

        drawingController = new DrawingController(canvas, imageView);
        //initConnection();
        if(connectionService == null || !connectionService.isConnected()){
            System.out.println("Disconnected");
            showReconnectButton();
        }else {
            showChatBox();
        }
    }

    public Controller() {}

    private void initConnection() {
        TextInputDialog dialog = new TextInputDialog();
        dialog.setTitle("Login Dialog");
        dialog.setHeaderText("Please give us your login");
        dialog.setContentText("Please enter your login:");
        Optional<String> result = dialog.showAndWait();
        result.ifPresent(name -> {
            connectionService = new ConnectionService(IP, PORT, name);
        });

        System.out.println(connectionService.isConnected());

    }

    private void startChatReceiverTask() {
        System.out.println("starting reading thread");
        Runnable task = this::runChatReceiverTask;

        readingThread = new Thread(task);
        readingThread.setDaemon(true);
        readingThread.start();
    }

    private void runChatReceiverTask() {
        WritableImage writableImage = new WritableImage((int)canvas.getWidth(),(int)canvas.getHeight());


        try {
            while (connectionService != null && connectionService.isConnected()) {
                HeaderType header = connectionService.readHeader();
                System.out.println(header);
                if (header == HeaderType.CHAT_MESSAGE) {
                    updateChatBox(connectionService.getMessage());
                } else if (header == HeaderType.UPDATECANVAS) {
                    SwingFXUtils.toFXImage(connectionService.getCanvas(), writableImage);
                    drawingController.updateImage(writableImage);
                } else if (header == HeaderType.GAME_WAITING) {
                    updateChatBox(connectionService.getGameWaiting());
                } else if (header == HeaderType.GAME_ENDED) {
                    updateChatBox(connectionService.getWinner());
                    drawingController.allowDrawing(false);
                    drawingController.clearImage();
                    showMessageField();
                } else if (header == HeaderType.GAME_READY) {
                    updateChatBox(connectionService.getGameReady());
                } else if (header == HeaderType.YOU_ARE_DRAWER) {
                    updateChatBox(connectionService.getThingToDraw());
                    drawingController.allowDrawing(true);
                    hideMessageField();
                } else if (header == HeaderType.CLUE_CORRECT) {
                    connectionService.clueCorrect();
                } else if (header == HeaderType.CLUE_INCORRECT) {
                    connectionService.clueIncorrect();
                }
            }
        } catch (IOException e) {
            timer.cancel();
            Platform.runLater(this::showReconnectButton);
        } catch(NullPointerException e){
            timer.cancel();
            try {
                connectionService.closeSocket();
            } catch (IOException ex) {
                ex.printStackTrace();
            }
            Platform.runLater(()->{
                showAlert("IMAGE_ERROR");
                showReconnectButton();
            });
        }
    }

    private void hideMessageField() {
        Platform.runLater(() -> {
            chatBox.getChildren().remove(messageField);
        });

        class SendImage extends TimerTask {
            @Override
            public void run() {
                try {
                    if(connectionService.isConnected()){
                        connectionService.sendPicture(drawingController.getByteArrayFromCanvas());
                    }
                } catch (Exception e) {
                    this.cancel();
                }
            }
        }

        timer = new Timer();
        timer.schedule(new SendImage(), 0, 100);

    }

    private void showMessageField() {
        Platform.runLater(() -> {
            if (!chatBox.getChildren().contains(messageField)) {
                chatBox.getChildren().add(messageField);
            }
        });
    }

    private void updateChatBox(Message message) throws IOException {
        Platform.runLater(() -> {
            messagesBox.getItems().add(messagesBox.getItems().size(), formatMessageToShow(message));
            messageField.clear();
        });
    }

    private void updateChatBox(String message) throws IOException {
        Message msg = new Message("INFO", message);
        Platform.runLater(() -> {
            messagesBox.getItems().add(messagesBox.getItems().size(), formatMessageToShow(msg));
            messageField.clear();
        });
    }

    private String formatMessageToShow(Message message) {
        return message.getAuthor() + ": " + message.getContent();
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

    private void showAlert(String type) {
        Alert alert = new Alert(Alert.AlertType.ERROR);

        if (type.equals("SEND_ERROR")) {
            alert.setTitle("Sending message error");
            alert.setHeaderText("Oops, something went wrong");
            alert.setContentText("Please try to send that message again");
        } else if (type.equals("CONNECT_ERROR")){
            alert.setTitle("Connecting error");
            alert.setHeaderText("Oops, something went wrong");
            alert.setContentText("Seems like server is not responding");
        } else if (type.equals("IMAGE_ERROR")){
            alert.setTitle("Image error");
            alert.setHeaderText("Oops, something went wrong");
            alert.setContentText("There is a problem loading your image");
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
            if (connectionService != null && connectionService.isConnected()) {
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
