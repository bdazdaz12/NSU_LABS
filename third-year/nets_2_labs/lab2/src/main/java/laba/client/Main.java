package laba.client;

public class Main {

    public static void main(String[] args) {
        String filePath = args[0];
        String serverName = args[1];
        String serverPort = args[2];

        int port = -1;
        try {
            port = Integer.parseInt(serverPort);
        } catch (NumberFormatException e) {
            System.err.println("INVALID SERVER PORT");
            System.exit(1);
        }

        SendService.sendFile(filePath, serverName, port);
    }
}
