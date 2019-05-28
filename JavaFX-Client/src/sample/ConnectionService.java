package sample;

import sample.Model.HeaderType;
import sample.Model.Headers;
import sample.Model.Message;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.*;

class ConnectionService {

    private SocketChannel socketChannel;


    private boolean connected;
    private boolean isOnTable = false;
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

    public String read(int expectedLength) throws IOException {
        ByteBuffer buff = ByteBuffer.allocate(expectedLength);
        int read = 0;
        int consumed;
        StringBuilder stringBuilder = new StringBuilder();

        do {
            buff.clear();
            consumed = socketChannel.read(buff);

            if (consumed == -1) {
                throw new IOException();
            }
            if (consumed > 0) {
                stringBuilder.append(new String(buff.array()));
            }
            read += consumed;

        } while (read != expectedLength);

        return stringBuilder.toString();
    }

    public boolean isOnTable() {
        return isOnTable;
    }

    private String read1(int bytesToRead) throws IOException {
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

    private byte[] convertByteBufferToByteArray(ByteBuffer buffer) {

        byte[] bytes = new byte[buffer.remaining()];
        buffer.get(bytes, 0, bytes.length);

        buffer.clear();
        bytes = new byte[buffer.capacity()];
        buffer.get(bytes, 0, bytes.length);

        return bytes;
    }

    private void sendWelcomePackage() throws IOException {
        StringBuilder welcomePackage = new StringBuilder("HELLO_SERVER");
        welcomePackage.append(String.format("%04d", this.loggedUser.length()));
        welcomePackage.append(this.loggedUser);
        socketChannel.write(ByteBuffer.wrap(welcomePackage.toString().getBytes()));
    }

    private void send(String message) throws IOException {
        socketChannel.write(ByteBuffer.wrap(message.getBytes()));
    }


    public void sendMessage(String message) throws IOException {
        StringBuilder stringBuilder = new StringBuilder("SEND_MESSAGE");
        stringBuilder.append(String.format("%04d", message.length()));
        stringBuilder.append(message);

        send(stringBuilder.toString());
    }

    public Message getMessage() throws IOException {
        int length = Integer.parseInt(read(BYTES_TO_READ_LENGTH));
        String userNameAndMessage = read(length);
        Message message = new Message(userNameAndMessage.split("\n")[0], userNameAndMessage.split("\n")[1]);
        return message;
    }

    public boolean isConnected() {
        return connected;
    }

    public void closeSocket() throws IOException {
        socketChannel.close();
    }

    public HeaderType readHeader() throws IOException {
        String header = read1(HEADER_LENGTH);

        if (Headers.HEADERS.containsKey(header)) {
            return Headers.HEADERS.get(header);
        }

        return HeaderType.UNDEFINED;
    }

    public void sendByteArray(byte[] byteArray) throws IOException {
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

    public String getGameWaiting() throws IOException {
        read(BYTES_TO_READ_LENGTH);
        return "The game has not started yet";
    }

    public String getWinner() throws IOException {
        return "Correct Answer! " + getMessage().getContent();
    }

    public String getGameReady() throws IOException {
        int length = Integer.parseInt(read(BYTES_TO_READ_LENGTH));
        String result = read(length);
        System.out.println("The drawer is - " + result);
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
        int amountOfTables = Integer.parseInt(read1(BYTES_TO_READ_LENGTH));
        String result = read1(amountOfTables);
        return Arrays.asList(result.split("\n"));
    }

    public void clueCorrect() throws IOException {
        read(BYTES_TO_READ_LENGTH);
    }

    public void clueIncorrect() throws IOException {
        read(BYTES_TO_READ_LENGTH);
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
        int length = Integer.parseInt(read1(BYTES_TO_READ_LENGTH));
        int id = Integer.parseInt(read1(length));
        System.out.println("Id is = " + id);
    }

    public void enterTable(String table) throws IOException {
        System.out.println("Entered table " + table);
        String message = "ENTER__TABLE" + String.format("%04d", table.length()) +
                table;
        send(message);
    }
}
