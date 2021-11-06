package server;

import java.io.IOException;

public class Main {

    public static void main(String[] args) {
        int port = Integer.parseInt(args[0]);
        try {
            Thread receiver_thread = new Thread(new Receiver(port));
            receiver_thread.start();
        }
        catch (IOException ex) {
            ex.printStackTrace();
        }
    }
}