package sample;

import com.sun.org.apache.xml.internal.security.exceptions.Base64DecodingException;
import com.sun.org.apache.xml.internal.security.utils.Base64;
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
import java.util.Random;

class ConnectionService {

    private SocketChannel socketChannel;


    private boolean connected;
    private int HEADER_LENGTH = 12;
    private int BYTES_TO_READ_LENGTH = 4;

    ConnectionService(String ip, int port){
            connected = startConnection(ip, port);
    }

    private boolean startConnection(String ip, int port) {

       boolean success;
       try {
           InetSocketAddress address = new InetSocketAddress(InetAddress.getByName(ip), port);

           socketChannel = SocketChannel.open(address);
           socketChannel.configureBlocking(false);

           success = handShakeServer();
           joinTable();
        } catch (Exception e) {
           return false;
        }

       return success;
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

        do{
            buff.clear();
            consumed = socketChannel.read(buff);

            if(consumed == -1) {
                throw new IOException();
            }
            if(consumed > 0){
                stringBuilder.append(new String(buff.array()));
            }
            read +=consumed;

        } while (read != expectedLength);

        return stringBuilder.toString();
    }

    private byte[] readByteArray(int expectedLength) throws IOException {
        ByteBuffer buff = ByteBuffer.allocate(expectedLength);
        int read = 0;
        int consumed;
        byte[] result = new byte[0];

        do{
            buff.clear();
            consumed = socketChannel.read(buff);
            if(consumed == -1) {
                throw new IOException();
            }

            result = concatByteArrays(result, convertByteBufferToByteArray(buff));
            read +=consumed;

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
        welcomePackage.append(String.format("%04d","Ja".length()));
        welcomePackage.append("Ja");
        socketChannel.write(ByteBuffer.wrap(welcomePackage.toString().getBytes()));
    }

    private void joinTable() throws IOException {
        send("ENTER__TABLE0000");
    }

    private void send(String message) throws IOException {
        socketChannel.write(ByteBuffer.wrap(message.getBytes()));
    }


    public void sendMessage(String message) throws IOException {
        StringBuilder stringBuilder = new StringBuilder("SEND_MESSAGE");
        stringBuilder.append(String.format("%04d",message.length()));
        stringBuilder.append(message);

        send(stringBuilder.toString());
    }

    public Message getMessage() throws IOException {
        int length = Integer.parseInt(read(BYTES_TO_READ_LENGTH));
        String userNameAndMessage = read(length);
        Message message = new Message(userNameAndMessage.split("\n")[0],userNameAndMessage.split("\n")[1]);
        System.out.println(message);
        return message;
    }

    public boolean isConnected() {
        return connected;
    }

    public void closeSocket() throws IOException {
        socketChannel.close();
    }

    public HeaderType readHeader() throws IOException {
        String header = read(HEADER_LENGTH);
        System.out.println(header);

        if(Headers.HEADERS.containsKey(header)){
            return Headers.HEADERS.get(header);
        }

        return HeaderType.UNDEFINED;
    }

    public void sendByteArray(byte[] byteArray) throws IOException {
        int length = byteArray.length;
        socketChannel.write(ByteBuffer.wrap(byteArray));
    }

    public String getGameWaiting() throws IOException {
        read(BYTES_TO_READ_LENGTH);
        return "The game has not started yet";
    }

    public String getWinner() throws IOException {
        return "Correct Answer! " + getMessage();
    }

    public String getGameReady() throws IOException {
        int length = Integer.parseInt(read(BYTES_TO_READ_LENGTH));
        String result = read(length);
        System.out.println("The drawer is - " + result);
        return "The drawer is - " + result;
    }

    public BufferedImage getCanvas() throws IOException {
        int length = Integer.parseInt(read(BYTES_TO_READ_LENGTH*2));
        byte[] bitmapByteArray = readByteArray(length);
        ByteArrayInputStream bais = new ByteArrayInputStream(bitmapByteArray);
        File outputFile = new File("saved.png");
        BufferedImage image = ImageIO.read(bais);
        ImageIO.write(image, "png", outputFile);
        return image;
    }

    public String getThingToDraw() throws IOException {
        int length = Integer.parseInt(read(BYTES_TO_READ_LENGTH));
        String result = read(length);

        return "Your turn! Draw - " + result;
    }

    public void clueCorrect() throws IOException {
        read(BYTES_TO_READ_LENGTH);
    }

    public void clueIncorrect() throws IOException {
        read(BYTES_TO_READ_LENGTH);
    }

    public void sendPicture(byte[] picture  ) throws Exception {
        String header = "SET___CANVAS" + preparePictureLength(picture.length);
        byte[] byteHeader = concatByteArrays(header.getBytes(), picture);
        sendByteArray(byteHeader);
    }

    public String preparePictureLength(int length) throws Exception {
        StringBuilder stringBuilder = new StringBuilder(length);
        if(length > 100000000) throw new Exception("The length of picture is too big!");
        String message = String.format("%08d", length);
//        System.out.println(message);
        return message;
    }

    private byte[] concatByteArrays(byte[] array1, byte[] array2) {
        byte[] afterConcat = new byte[array1.length + array2.length];
        System.arraycopy(array1, 0 , afterConcat, 0, array1.length);
        System.arraycopy(array2, 0 , afterConcat, array1.length, array2.length);

        return afterConcat;
    }

}
