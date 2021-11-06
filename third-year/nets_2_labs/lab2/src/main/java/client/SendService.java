package client;

import java.io.*;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

public class SendService {

    private static final int BUFFER_SIZE = 8192;

    private static void sendFileMetadata(BufferedOutputStream sockOutStream,
                                         File fileToSend) throws IOException {
        byte[] fileNameLengthBuf = ByteBuffer.allocate(2)
                .putShort((short) fileToSend.getName().length()).array();

        sockOutStream.write(fileNameLengthBuf);

        byte[] fileNameBuf = fileToSend.getName().getBytes(StandardCharsets.UTF_8);
        sockOutStream.write(fileNameBuf);

        byte[] fileSizeBuf = ByteBuffer.allocate(8).putLong(fileToSend.length()).array();
        sockOutStream.write(fileSizeBuf);

        System.out.println("\n-------- Sent file metadata --------");
    }

    private static void sendFileData(BufferedOutputStream sockOutStream,
                                     File fileToSend) throws IOException {
        Files.copy(Path.of(fileToSend.getPath()), sockOutStream);
        System.out.println("-------- File data sent 2--------");
    }


//    private static void sendFileData(BufferedOutputStream sockOutStream,
//                                     File fileToSend) throws IOException {
//        byte[] fileBuf = new byte[BUFFER_SIZE];
//        long bytesSent = 0;
//        long bytesRemain = fileToSend.length();
//        InputStream inputStream = new FileInputStream(fileToSend);
//        while (bytesSent < fileToSend.length()) {
//            int bytesToSend = bytesRemain < BUFFER_SIZE ? (int) bytesRemain : 4096;
//            int bytesReadNow = inputStream.read(fileBuf, 0, bytesToSend);
//            sockOutStream.write(fileBuf, 0, bytesReadNow);
//            sockOutStream.flush();
//            bytesSent += bytesReadNow;
//            bytesRemain -= bytesReadNow;
//        }
//        inputStream.close();
//        System.out.println("-------- File data sent --------");
//    }

    public static void sendFile(String filePath, String hostName, int port) {
        try (Socket socket = new Socket(InetAddress.getByName(hostName), port);
             BufferedOutputStream sockOutStream = new BufferedOutputStream(socket.getOutputStream())) {
            File fileToSend = new File(filePath);
            sendFileMetadata(sockOutStream, fileToSend);
            sendFileData(sockOutStream, fileToSend);
        } catch (UnknownHostException e) {
            System.err.println("UNKNOWN_HOST_ERROR: invalid server address (-_-)");
            e.printStackTrace();
            System.exit(2);
        } catch (IOException e) {
            System.err.println("SEND_ERROR: Error during sending");
            e.printStackTrace();
            System.exit(3);
        }
    }
}
