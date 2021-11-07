package laba.server;

import java.io.IOException;

public class Main {

    public static void main(String[] args) {

        int listeningPort = -1;
        try {
            listeningPort = Integer.parseInt(args[0]);
        } catch (NumberFormatException e) {
            System.err.println("INVALID LISTENING PORT");
            System.exit(1);
        }

        try {
            Thread listenerThread = new Thread(new ListenerTask(listeningPort));
            listenerThread.start();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}