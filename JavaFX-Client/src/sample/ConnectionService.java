package sample;

import sample.Model.HeaderType;
import sample.Model.Headers;
import sample.Model.Message;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.nio.channels.SocketChannel;

public class ConnectionService {

    private Socket socket;
    private PrintWriter out;
    private BufferedReader in;

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

            socket = new Socket(ip, port);
            socket.setTcpNoDelay(true);
            out = new PrintWriter(socket.getOutputStream(), true);
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            success = handShakeServer();
        } catch (Exception e) {
           return false;
        }

       return success;
    }

    private boolean handShakeServer() throws IOException {
        sendWelcomePackage();
        HeaderType header = readHeader();
        read(4);
        return  header.equals(HeaderType.WELCOME_USER);
    }

    public String read(int expectedLength) throws IOException {
        char[] buff = new char[expectedLength];
        int read = 0;
        int consumed = 0;
        do{
            consumed = in.read(buff, read, expectedLength - read);
            if(consumed == -1) {
                throw new IOException();
            }
            read += consumed;
        } while (read != expectedLength);

        return new String(buff);
    }

    private void sendWelcomePackage() {
        StringBuilder welcomePackage = new StringBuilder("HELLO_SERVER");
        welcomePackage.append(String.format("%04d","Ja".length()));
        welcomePackage.append("Ja");
        out.write(welcomePackage.toString());
        out.flush();
    }

    public void sendMessage(String message){
        StringBuilder stringBuilder = new StringBuilder("SEND_MESSAGE");
        stringBuilder.append(String.format("%04d",message.length()));
        stringBuilder.append(message);

        out.write(stringBuilder.toString());
        out.flush();
    }

    public Message getMessage() throws IOException {
        int length = Integer.parseInt(read(4));
        String userNameAndMessage = read(length);

        return new Message(userNameAndMessage.split("\n")[0],userNameAndMessage.split("\n")[1]);
    }

    public boolean isConnected() {
        return connected;
    }

    public void closeSocket() throws IOException {
        in.close();
        out.close();
        socket.close();
    }

    public HeaderType readHeader() throws IOException {
        String header = read(12);
        System.out.println(header);
        if(Headers.HEADERS.containsKey(header)){
            return Headers.HEADERS.get(header);
        }

        return HeaderType.UNDEFINED;
    }
}
