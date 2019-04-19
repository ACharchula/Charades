package sample;

import sample.Model.HeaderType;
import sample.Model.Headers;
import sample.Model.Message;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class ConnectionService {

    private Socket socket;
    private PrintWriter out;
    private BufferedReader in;

    private boolean connected;

    public ConnectionService(String ip, int port){
            connected = startConnection(ip, port);
    }

    private boolean startConnection(String ip, int port) {

       boolean success;
       try {
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
        out.write(welcomePackage.toString());
        out.flush();
    }

    public void sendMessage(String message){
        out.println("SEND_MESSAGE");
        out.println(message.length());
        out.println(message);
    }

    public Message getMessage() throws IOException {
        int length = Integer.parseInt(read(4));
        String userNameAndMessage = read(length+1);

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
