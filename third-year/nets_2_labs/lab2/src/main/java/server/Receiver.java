package server;

import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

public class Receiver implements Runnable {

    ServerSocket socket;

    public Receiver(int port) throws IOException {
        socket = new ServerSocket(port, 0, InetAddress.getLocalHost());
        System.out.println("Server started. Address: " +
                socket.getLocalSocketAddress());
    }

    @Override
    public void run() {
        try {
            while (!Thread.currentThread().isInterrupted()) {
                Socket newSenderSocket = socket.accept();
                Thread newSenderThread = new Thread(new SenderThread(newSenderSocket));
                newSenderThread.start();
                System.out.println("New sender. Address: " +
                        newSenderSocket.getLocalSocketAddress());
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        } finally {
            try {
                socket.close();
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }
    }


}
