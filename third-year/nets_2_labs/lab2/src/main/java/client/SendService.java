package client;

import java.io.*;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.nio.file.Files;
import java.nio.file.Path;

public class SendService {

    private static final int BUFFER_SIZE = 4096;

    public static void sendFile(String filePath, String hostName, int port) {
        String[] filePathComponents = filePath.split("/");
        String fileToSendName = filePathComponents[filePathComponents.length - 1];

        try(Socket socket = new Socket(InetAddress.getByName(hostName), port)) {
            File fileToSend = new File(filePath);
            long fileSize = fileToSend.length();

            BufferedInputStream fileInStream = new BufferedInputStream(Files.newInputStream(Path.of(filePath)));
            BufferedOutputStream sockOutStream = new BufferedOutputStream(socket.getOutputStream());

            byte[] buf = new byte[BUFFER_SIZE];

        } catch (UnknownHostException e) {
            System.err.println("INVALID SERVER ADDRESS");
            e.printStackTrace();
            System.exit(2);
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(3);
        }
    }
}
