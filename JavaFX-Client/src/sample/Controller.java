package sample;

import javafx.application.Platform;
import javafx.embed.swing.SwingFXUtils;
import javafx.fxml.FXML;
import javafx.scene.canvas.Canvas;
import javafx.scene.control.*;
import javafx.scene.image.ImageView;
import javafx.scene.image.WritableImage;
import javafx.scene.input.KeyCode;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import sample.Model.HeaderType;
import sample.Model.Message;

import java.io.*;
import java.util.*;

public class Controller {

    private ConnectionService connectionService;
    private String IP = "127.0.0.1";
    private int PORT = 44444;
    private Button reconnectButton;
    private Button addTableButton;
    private Button refreshButton;
    private Button seeStatisticsButton;
    private ScrollPane tablesPane;
    private VBox tablesVBox;
    private Thread readingThread;
    private Thread writingThread;
    private List<String> tables = new ArrayList<>();
    private Map<String,String> stats = new HashMap<>();
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
    private StackPane stackPane;

    @FXML
    public Button surrenderButton;

    @FXML
    private Button leaveTableButton;

    @FXML
    public void initialize() {
        drawingController = new DrawingController(canvas, imageView);

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

        reconnectButton = new Button("Reconnect");
        reconnectButton.setOnMouseClicked(event -> {
            reconnectButton.setDisable(true);
            initConnection();
//            connectionService = new ConnectionService(IP, PORT);
            if (connectionIsActive()) {
                startChatReceiverTask();
                startChatWriterTask();
                showMainMenu();
            } else {
                showAlert("CONNECT_ERROR");
            }
            reconnectButton.setDisable(false);
        });

        tablesPane = new ScrollPane();
        tablesVBox = new VBox();
        tablesPane.setContent(tablesVBox);
        stackPane.getChildren().add(tablesPane);

        addTableButton = new Button("Add Table");
        addTableButton.setOnMouseClicked(event -> {
            addTableButton.setDisable(true);
            if (connectionIsActive()) {
                try {
                    connectionService.createNewTable();
                } catch (IOException e) {
                    showAlert("TABLE_ERROR");
                }
            }
            addTableButton.setDisable(false);
        });

        leaveTableButton.setOnMouseClicked(event -> {
            leaveTableButton.setDisable(true);
            if (connectionIsActive()) {
                try {
                    connectionService.leaveTable();
                    showMainMenu();
                } catch (IOException e) {
                    showAlert("TABLE_ERROR");
                }
            }
            leaveTableButton.setDisable(false);
        });

        surrenderButton.setOnMouseClicked(event -> {
            surrenderButton.setDisable(true);
            if (connectionIsActive()) {
                try {
                    showGuesserPerspective();
                    connectionService.surrender();
                    drawingController.allowDrawing(false);
                    drawingController.clearImage();
                } catch (IOException e) {
                    showAlert("TABLE_ERROR");
                }
            }
            surrenderButton.setDisable(false);
        });

        refreshButton = new Button("Refresh");
        refreshButton.setOnMouseClicked(event -> {
            refreshButton.setDisable(true);
            if (connectionIsActive()) {
                try {
                    connectionService.requestTablesInfo();
                } catch (IOException e) {
                    showAlert("TABLE_ERROR");
                }
            }
            refreshButton.setDisable(false);
        });

        seeStatisticsButton = new Button("See statsistics");
        seeStatisticsButton.setOnMouseClicked(event -> {
            seeStatisticsButton.setDisable(true);
            if(connectionIsActive()){
                try{
                    connectionService.getStatistics();
                } catch (IOException e) {
                    showAlert("TABLE_ERROR");
                }
            }
            seeStatisticsButton.setDisable(false);
        });

        Platform.runLater(this::prepareNotOnTableView);

        if (!connectionIsActive()) {
            showLoginView();
        } else {
            showMainMenu();
        }
    }

    private void showMainMenu() {
        timer.cancel();//stops timer if was running
        Platform.runLater(() -> {
            prepareNotOnTableView(); // hides everything on stackPane
            chatBox.getChildren().remove(reconnectButton);
            chatBox.getChildren().remove(leaveTableButton);
            chatBox.getChildren().remove(surrenderButton);
            showTablesButton();
            showActiveTables();
        });
    }

    private void showLoginView() {
        Platform.runLater(() -> {
            prepareNotOnTableView();// hides everything on stackPane
            chatBox.getChildren().forEach(child -> child.setVisible(false));
            chatBox.getChildren().remove(leaveTableButton);
            chatBox.getChildren().remove(surrenderButton);
            chatBox.getChildren().remove(refreshButton);
            chatBox.getChildren().remove(addTableButton);
            chatBox.getChildren().remove(seeStatisticsButton);
            showReconnectButton();
        });
    }


    private void showDrawerPerspective() {
        Platform.runLater(() -> {
            removeMainMenuButtons();
            addGameButtons();
            hideMessageFieldAndStartSendingImage();
            showCanvas();
            showChat();
        });
    }


    private void showGuesserPerspective() {
        Platform.runLater(() -> {
            removeMainMenuButtons();
            addGameButtons();
            showMessageField();
            showCanvas();
            showChat();
        });
    }

    private void addGameButtons() {
        if(!chatBox.getChildren().contains(surrenderButton))
            chatBox.getChildren().add(surrenderButton);
        if(!chatBox.getChildren().contains(leaveTableButton))
            chatBox.getChildren().add(leaveTableButton);
    }

    private void removeMainMenuButtons() {
        chatBox.getChildren().remove(addTableButton);
        chatBox.getChildren().remove(refreshButton);
    }

    private void showChat() {
        messagesBox.setVisible(true);
        leaveTableButton.setVisible(true);
    }


    private void showTablesButton() {
        //chatBox.getChildren().forEach(child -> child.setVisible(false));
        messageField.setVisible(false);
        if(!chatBox.getChildren().contains(addTableButton))
            chatBox.getChildren().add(addTableButton);
        if(!chatBox.getChildren().contains(refreshButton))
            chatBox.getChildren().add(refreshButton);
        if(!tablesVBox.getChildren().contains(seeStatisticsButton))
            tablesVBox.getChildren().add(seeStatisticsButton);
    }

    private void initConnection() {
        TextInputDialog dialog = createLoginDialog();
        Optional<String> result = dialog.showAndWait();
        result.ifPresent(name -> {
            connectionService = new ConnectionService(IP, PORT, name);
        });
    }

    private TextInputDialog createLoginDialog() {
        TextInputDialog dialog = new TextInputDialog();
        dialog.setTitle("Login Dialog");
        dialog.setHeaderText("Please give us your login");
        dialog.setContentText("Please enter your login:");
        return dialog;
    }

    private void startChatReceiverTask() {
        System.out.println("starting reading thread");
        Runnable task = this::runChatReceiverTask;

        readingThread = new Thread(task);
        readingThread.setDaemon(true);
        readingThread.start();
    }

    private void runChatReceiverTask() {
        WritableImage writableImage = new WritableImage((int) canvas.getWidth(), (int) canvas.getHeight());

        try {
            while (connectionIsActive()) {
                HeaderType header = connectionService.readHeader();
                System.out.println(header);
                switch (header) {
                    case CHAT_MESSAGE:
                        updateChatBox(connectionService.getMessage());
                        break;
                    case UPDATECANVAS:
                        SwingFXUtils.toFXImage(connectionService.getCanvas(), writableImage);
                        drawingController.updateImage(writableImage);
                        break;
                    case GAME_WAITING:
                        showGuesserPerspective();
                        updateChatBox(connectionService.getGameWaiting());
                        break;
                    case GAME_ENDED:
                        timer.cancel();
                        updateChatBox(connectionService.getWinner());
                        drawingController.clearImage();
                        drawingController.allowDrawing(false);
                        showMessageField();
                        //showGuesserPerspective();
                        break;
                    case GAME_READY:
                        updateChatBox(connectionService.getGameReady());
                        break;
                    case YOU_ARE_DRAWER:
                        timer.cancel();
                        updateChatBox(connectionService.getThingToDraw());
                        drawingController.clearImage();
                        drawingController.allowDrawing(true);
                        hideMessageFieldAndStartSendingImage();
                        System.out.println(drawingController.getCanvas());
//                        showDrawerPerspective();
                        break;
                    case SEE___TABLES:
                        tables = connectionService.getActiveTables();
                        showActiveTables();
                        break;
                    case TABLECREATED:
                        connectionService.getTableID();
                        connectionService.requestTablesInfo();
                        break;
                    case GAME_ABORTED:
                        connectionService.handleAbortion();
                        timer.cancel();
                        drawingController.clearImage();
                        drawingController.allowDrawing(false);
                        showMessageField();
                        break;
                    case COMMANDFAILD:
                        connectionService.handleFail();
                        showAlert("ERROR");
                        break;
                    case PING____PING:
                        connectionService.pingPong();
                        break;
                    case SEESTATISTIC:
                        stats = connectionService.seeStatistics();
                        showStatistics();
                        System.out.println(stats);
                        break;
                }
            }
        } catch (IOException e) {
            timer.cancel();
            Platform.runLater(this::showLoginView);
        } catch (NullPointerException e) {
            timer.cancel();
            try {
                connectionService.closeSocket();
            } catch (IOException ex) {
                ex.printStackTrace();
            }
            Platform.runLater(() -> {
                showAlert("IMAGE_ERROR");
                showLoginView();
            });
        }
    }

    private void showStatistics() {
        messagesBox.setVisible(true);
        int size = messagesBox.getItems().size();
        if(size > 0){
            Platform.runLater(()->{
                messagesBox.getItems().remove(0,size);
            });
        }

        stats.forEach((player, score) -> {
            try {
                Controller.this.updateChatBox(player +": " +score);
            } catch (IOException e) {
                showAlert("STATS_ERROR");
            }
        });
    }

    private void showCanvas() {
        Platform.runLater(() -> {
            tablesPane.setVisible(false);
            canvas.setVisible(true);
            imageView.setVisible(true);
        });
    }

    private void showActiveTables() {
        Platform.runLater(()->{
            int size = tablesVBox.getChildren().size();
            tablesVBox.getChildren().remove(1, size);
            tables.forEach(table -> {
                Button button = createEnterTableButton(table);
                if (!tablesVBox.getChildren().contains(button)) {
                    tablesVBox.getChildren().add(button);
                }
            });
            tablesPane.setVisible(true);
        });
    }

    private Button createEnterTableButton(String table) {
        tablesVBox.getChildren().add(new Label("Table nr. " + table));
        Button button = new Button("Enter table " + table);
        button.setOnMouseClicked(event -> {
            enterTable(table);
        });
        return button;
    }

    private void enterTable(String table) {
        try {
            connectionService.enterTable(table);
        } catch (IOException e) {
            showAlert("ENTER_TABLE_ERROR");
        }
    }

    private void hideMessageFieldAndStartSendingImage() {
        Platform.runLater(() -> {
            messageField.setVisible(false);
            surrenderButton.setVisible(true);
        });

        class SendImage extends TimerTask {
            @Override
            public void run() {
                try {
                    if (connectionService.isConnected()) {
                        if(drawingController.imageHasChanged()){
                            connectionService.sendPicture(drawingController.getByteArrayFromCanvas());
                            drawingController.setHasChanged(false);
                        }
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
            messageField.setVisible(true);
            surrenderButton.setVisible(false);
        });
        timer.cancel();
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
        } else if (type.equals("CONNECT_ERROR")) {
            alert.setTitle("Connecting error");
            alert.setHeaderText("Oops, something went wrong");
            alert.setContentText("Seems like server is not responding");
        } else if (type.equals("IMAGE_ERROR")) {
            alert.setTitle("Image error");
            alert.setHeaderText("Oops, something went wrong");
            alert.setContentText("There is a problem loading your image");
        } else if (type.equals("TABLE_ERROR")) {
            alert.setTitle("Creating table error");
            alert.setHeaderText("Oops, creating table finished unsuccesfully");
            alert.setContentText("There was a problem when creating your table,\nPlease try again");
        } else if (type.equals("ENTER_TABLE_ERROR")) {
            alert.setTitle("Entering table error");
            alert.setHeaderText("Oops, entering table finished unsuccesfully");
            alert.setContentText("There was a problem while entering table,\nPlease try again");
        } else if (type.equals("ERROR")) {
            alert.setTitle("ERROR");
            alert.setHeaderText("Oops, something went wrong");
            alert.setContentText("Something went wrong :( Please try again");
        } else if(type.equals("STATS_ERROR")){
            alert.setTitle("Getting statistics error");
            alert.setHeaderText("Oops, something went wrong");
            alert.setContentText("There was a problem while fetching statistics");
        }

        alert.showAndWait();
    }

    private void showReconnectButton() {
        try {
            if (connectionService != null && connectionService.isConnected()) {
                connectionService.closeSocket();
            }
        } catch (IOException e) {
            connectionService = null;
        }
        chatBox.getChildren().add(reconnectButton);
    }

    private void prepareNotOnTableView() {
        stackPane.getChildren().forEach(child -> child.setVisible(false));
        messagesBox.setVisible(false);
    }

    private boolean connectionIsActive() {
        return this.connectionService != null && this.connectionService.isConnected();
    }

}
