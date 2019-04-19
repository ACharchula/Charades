package sample;

import sample.Model.HeaderType;
import sample.Model.Headers;
import sample.Model.Message;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

import static java.lang.System.out;

public class ConnectionService {

    private SocketChannel socketChannel;


    private boolean connected;
    private int HEADER_LENGTH = 12;
    private int BYTES_TO_READ_LENGTH = 4;

    public ConnectionService(String ip, int port){
            connected = startConnection(ip, port);
    }

    private boolean startConnection(String ip, int port) {

       boolean success;
       try {
           InetSocketAddress address = new InetSocketAddress(InetAddress.getByName(ip), port);

           socketChannel = SocketChannel.open(address);

           success = handShakeServer();
        } catch (Exception e) {
           return false;
        }

       return success;
    }

    private boolean handShakeServer() throws IOException {
        sendWelcomePackage();
        HeaderType header = readHeader();
        read(BYTES_TO_READ_LENGTH);
        return  header.equals(HeaderType.WELCOME_USER);
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

            stringBuilder.append(new String(buff.array()));
            read +=consumed;

        } while (read != expectedLength);

        return stringBuilder.toString();
    }

    private void sendWelcomePackage() throws IOException {
        StringBuilder welcomePackage = new StringBuilder("HELLO_SERVER");
        welcomePackage.append(String.format("%04d","Ja".length()));
        welcomePackage.append("Ja");
        socketChannel.write(ByteBuffer.wrap(welcomePackage.toString().getBytes()));
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

        return new Message(userNameAndMessage.split("\n")[0],userNameAndMessage.split("\n")[1]);
    }

    public boolean isConnected() {
        return connected;
    }

    public void closeSocket() throws IOException {
        socketChannel.close();
    }

    public HeaderType readHeader() throws IOException {
        String header = read(HEADER_LENGTH);

        if(Headers.HEADERS.containsKey(header)){
            return Headers.HEADERS.get(header);
        }

        return HeaderType.UNDEFINED;
    }
}
