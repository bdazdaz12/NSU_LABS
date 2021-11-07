package laba.server;

import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

public class ListenerTask implements Runnable {

    private final ServerSocket serverSocket;

    public ListenerTask(int listeningPort) throws IOException {
        serverSocket = new ServerSocket(listeningPort, 0, InetAddress.getLocalHost());
        System.out.println("--------- Server started. Address: " +
                serverSocket.getLocalSocketAddress().toString() + " --------");
    }

    @Override
    public void run() {
        try {
            while (!Thread.currentThread().isInterrupted()) {
                Socket newReceiverSocket = serverSocket.accept();
                Thread newSenderThread = new Thread(new ReceiverTask(newReceiverSocket));
                newSenderThread.start();
                System.out.println("------- New receiver. Address: " +
                        newReceiverSocket.getLocalSocketAddress().toString() + " --------");
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                serverSocket.close();
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }
    }
}
