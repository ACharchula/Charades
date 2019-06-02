package sample;

import sample.Model.HeaderType;
import sample.Model.Headers;
import sample.Model.Message;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.*;

class ConnectionService {

    private SocketChannel socketChannel;


    private boolean connected;
    private int HEADER_LENGTH = 12;
    private int BYTES_TO_READ_LENGTH = 4;
    private String loggedUser;

    ConnectionService(String ip, int port, String name) {
        this.loggedUser = name;
        connected = startConnection(ip, port);
    }

    private boolean startConnection(String ip, int port) {

        boolean success;
        try {
            InetSocketAddress address = new InetSocketAddress(InetAddress.getByName(ip), port);

            socketChannel = SocketChannel.open(address);
            socketChannel.configureBlocking(false);

            success = handShakeServer();
            requestTablesInfo();
        } catch (Exception e) {
            return false;
        }

        return success;
    }

    public void requestTablesInfo() throws IOException {
        send("LIST__TABLES0000");
    }

    private boolean handShakeServer() throws IOException {
        sendWelcomePackage();
        HeaderType header = readHeader();
        read(BYTES_TO_READ_LENGTH);
        return header.equals(HeaderType.WELCOME_USER);
    }


    private String read(int bytesToRead) throws IOException {
        return new String(readByteArray(bytesToRead));
    }

    private byte[] readByteArray(int expectedLength) throws IOException {
        int read = 0;
        int consumed;
        byte[] result = new byte[expectedLength];

        do {
            ByteBuffer buff = ByteBuffer.allocate(expectedLength - read);
            buff.clear();
            consumed = socketChannel.read(buff);
            if(consumed > 0) {
                System.arraycopy(buff.array(), 0, result, read, consumed);
                read += consumed;
            }
            else if (consumed == -1) {
                throw new IOException("Error when reading byte array");
            }

        } while (read != expectedLength);

        return result;
    }

    private void sendWelcomePackage() throws IOException {
        String welcomePackage = "HELLO_SERVER" + String.format("%04d", this.loggedUser.length()) +
                this.loggedUser;
        socketChannel.write(ByteBuffer.wrap(welcomePackage.getBytes()));
    }

    private void send(String message) throws IOException {
        socketChannel.write(ByteBuffer.wrap(message.getBytes()));
    }


    void sendMessage(String message) throws IOException {
        String stringBuilder = "SEND_MESSAGE" + String.format("%04d", message.length()) +
                message;
        send(stringBuilder);
    }

    Message getMessage() throws IOException {
        int length = Integer.parseInt(read(BYTES_TO_READ_LENGTH));
        String userNameAndMessage = read(length);
        return new Message(userNameAndMessage.split("\n")[0], userNameAndMessage.split("\n")[1]);
    }

    boolean isConnected() {
        return connected;
    }

    void closeSocket() throws IOException {
        socketChannel.close();
    }

    HeaderType readHeader() throws IOException {
        String header = read(HEADER_LENGTH);

        if (Headers.HEADERS.containsKey(header)) {
            return Headers.HEADERS.get(header);
        }

        return HeaderType.UNDEFINED;
    }

    private void sendByteArray(byte[] byteArray) throws IOException {
        int length = byteArray.length;
        int sent = 0;
        ByteBuffer buffer = ByteBuffer.wrap(byteArray);

        while(sent != length) {
            sent += socketChannel.write(buffer);

            if(sent != length) {
                byte[] rest = new byte[length - sent];
                buffer.get(rest, sent, length - sent);
                buffer = ByteBuffer.wrap(rest);
            }
        }
    }

    String getGameWaiting() throws IOException {
        read(BYTES_TO_READ_LENGTH);
        return "The game has not started yet";
    }

    String getWinner() throws IOException {
        return "Correct Answer! " + getMessage().getContent();
    }

    String getGameReady() throws IOException {
        int length = Integer.parseInt(read(BYTES_TO_READ_LENGTH));
        String result = read(length);
        return "The drawer is - " + result;
    }

    public BufferedImage getCanvas() throws IOException {
        int length = Integer.parseInt(read(BYTES_TO_READ_LENGTH * 2));
        byte[] bitmapByteArray = readByteArray(length);
        ByteArrayInputStream bais = new ByteArrayInputStream(bitmapByteArray);
        return ImageIO.read(bais);
    }

    public String getThingToDraw() throws IOException {
        int length = Integer.parseInt(read(BYTES_TO_READ_LENGTH));
        String result = read(length);
        return "Your turn! Draw - " + result;
    }

    public List<String> getActiveTables() throws IOException {
        int amountOfTables = Integer.parseInt(read(BYTES_TO_READ_LENGTH));
        String result = read(amountOfTables);
        return Arrays.asList(result.split("\n"));
    }

    public void sendPicture(byte[] picture) throws Exception {
        if(isConnected()){
            String header = "SET___CANVAS" + preparePictureLength(picture.length);
            byte[] byteHeader = concatByteArrays(header.getBytes(), picture);
            sendByteArray(byteHeader);
        }
    }

    public String preparePictureLength(int length) throws Exception {
        if (length > 100000000) throw new Exception("The length of picture is too big!");
        String message = String.format("%08d", length);
        return message;
    }

    private byte[] concatByteArrays(byte[] array1, byte[] array2) {
        byte[] afterConcat = new byte[array1.length + array2.length];
        System.arraycopy(array1, 0, afterConcat, 0, array1.length);
        System.arraycopy(array2, 0, afterConcat, array1.length, array2.length);

        return afterConcat;
    }


    public void createNewTable() throws IOException {
        send("CREATE_TABLE0000");
    }

    public void getTableID() throws IOException {
        int length = Integer.parseInt(read(BYTES_TO_READ_LENGTH));
        int id = Integer.parseInt(read(length));
    }

    public void enterTable(String table) throws IOException {
        String message = "ENTER__TABLE" + String.format("%04d", table.length()) +
                table;
        send(message);
    }

    public void leaveTable() throws IOException {
        send("COMEOUTTABLE0000");
    }

    public void surrender() throws IOException {
        send("GIVE_UP_GAME0000");
    }

    public void handleAbortion() throws IOException {
        int length = Integer.parseInt(read(BYTES_TO_READ_LENGTH));
        String pass = read(length);
    }

    public void handleFail() throws IOException {
        read(BYTES_TO_READ_LENGTH);
    }

    public void pingPong() throws IOException {
        read(BYTES_TO_READ_LENGTH);
        send("PONG____PONG0000");
    }

    public void getStatistics() throws IOException {
        send("GETSTATISTIC0000");
    }


    public Map<String,String> seeStatistics() throws IOException {
        int amountOfStats = Integer.parseInt(read(BYTES_TO_READ_LENGTH));
        String result = read(amountOfStats);
        List<String> array = Arrays.asList(result.split("\n"));
        HashMap<String,String> map = new HashMap<>();
        for(int i = 0; i<array.size(); ++i){
            map.put(array.get(i),array.get(++i));
        }
        return map;
    }
}
