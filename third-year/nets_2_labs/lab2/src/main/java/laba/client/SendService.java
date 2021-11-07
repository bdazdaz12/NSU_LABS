package laba.client;

import laba.FileService;

import java.io.*;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.NoSuchAlgorithmException;

public class SendService {

    private static void sendFileMetadata(BufferedOutputStream sockOutStream,
                                         File fileToSend) throws IOException {
        byte[] fileNameLengthBuf = ByteBuffer.allocate(2)
                .putShort((short) fileToSend.getName().length()).array();

        sockOutStream.write(fileNameLengthBuf);

        byte[] fileNameBuf = fileToSend.getName().getBytes(StandardCharsets.UTF_8);
        sockOutStream.write(fileNameBuf);

        byte[] fileSizeBuf = ByteBuffer.allocate(8).putLong(fileToSend.length()).array();
        sockOutStream.write(fileSizeBuf);

        try {
            sockOutStream.write(FileService.calculateFileHash(fileToSend));
        } catch (NoSuchAlgorithmException ignored) {
            System.err.println("NE YDALOS CALC HASH pochemyto blya");
        }

        System.out.println("-------- Sent file metadata --------");
    }

    private static void sendFileData(BufferedOutputStream sockOutStream,
                                     File fileToSend) throws IOException {
        Files.copy(Path.of(fileToSend.getPath()), sockOutStream);
        System.out.println("-------- File data sent --------");
    }

    public static void sendFile(String filePath, String hostName, int port) {
        try (Socket socket = new Socket(InetAddress.getByName(hostName), port);
             BufferedOutputStream sockOutStream = new BufferedOutputStream(socket.getOutputStream())) {
            File fileToSend = new File(filePath);
            sendFileMetadata(sockOutStream, fileToSend);
            sendFileData(sockOutStream, fileToSend);
        } catch (UnknownHostException e) {
            System.err.println("UNKNOWN_HOST_ERROR: invalid laba.server address (-_-)");
            e.printStackTrace();
            System.exit(2);
        } catch (IOException e) {
            System.err.println("SEND_ERROR: Error during sending");
            e.printStackTrace();
            System.exit(3);
        }
    }
}
