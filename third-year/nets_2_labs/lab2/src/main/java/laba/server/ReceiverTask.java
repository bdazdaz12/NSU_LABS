package laba.server;

import laba.FileService;

import java.io.*;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;
import java.util.Timer;

public class ReceiverTask implements Runnable {

    private static final String UPLOADS_DIR = System.getProperty("user.dir") + "/uploads";
    private static final int BUF_SIZE = 4096;

    private final Socket receiverSocket;

    private String fileName;
    private long fileSize;
    private byte[] receivedHash;
    private File receivedFile;

    private long bytesReceived = 0;
    private Timer timer;

    public ReceiverTask(Socket receiverSocket) throws IOException {
        this.receiverSocket = receiverSocket;
    }

    @Override
    public void run() {
        try {
            receiveHeader();
            receiveFile();

            byte[] receivedFileHash = FileService.calculateFileHash(receivedFile);
            if (Arrays.equals(receivedHash, receivedFileHash)) {
                System.out.println("--- The integrity of the received file is not violated ---");
            } else {
                System.err.println("The integrity of the received file is violated");
            }
        } catch (IOException | IndexOutOfBoundsException e) {
            System.err.println("Connection to sender of '" + fileName +
                    "' lost. File didn't receive");
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } finally {
            timer.cancel();
            try {
                receiverSocket.close();
                System.out.println("\n---- Socket of '" + fileName + "' closed ----\n");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    void receiveHeader() throws IOException {
        DataInputStream dis = new DataInputStream(receiverSocket.getInputStream());

        byte[] fileNameLengthBuf = new byte[2];
        dis.readFully(fileNameLengthBuf);
        int fileNameLength = ByteBuffer.wrap(fileNameLengthBuf).getShort();

        byte[] fileNameBuf = new byte[fileNameLength];
        dis.readFully(fileNameBuf);
        fileName = new String(fileNameBuf, StandardCharsets.UTF_8);

        byte[] fileSizeBuf = new byte[8];
        dis.readFully(fileSizeBuf);
        fileSize = ByteBuffer.wrap(fileSizeBuf).getLong();

        byte[] hashBuf = new byte[32];
        dis.readFully(hashBuf);
        receivedHash = hashBuf;

        System.out.println("---- Header received: '" + fileName + "', " + fileSize + " bytes ----");
    }

    void receiveFile() throws IOException, IndexOutOfBoundsException {
        new File(UPLOADS_DIR).mkdirs(); // если эта директория существует, то поступает как Оксимирон

        receivedFile = new File(UPLOADS_DIR + "/" + fileName);

//        if (outFile.exists()) {}

        if (!receivedFile.createNewFile()) {
            System.err.println("Failed to create file - " + fileName
                    + " - by thread: " + Thread.currentThread().getName());
            System.err.println("NO POKA MNYE LEN ETO FIXITb");
        }

        timer = new Timer();
        SpeedCounterTask speedCounterTask = new SpeedCounterTask(this);
        timer.schedule(speedCounterTask, 1000, 1000);

        FileOutputStream fileOutputStream = new FileOutputStream(receivedFile);
        byte[] fileBuf = new byte[BUF_SIZE];
        long bytesRemain = fileSize;

        long startTime = System.currentTimeMillis();
        while (bytesRemain > 0) {
            int bytesReceivedNow = receiverSocket.getInputStream().read(fileBuf, 0,
                    bytesRemain < BUF_SIZE ? (int) bytesRemain : BUF_SIZE);
            bytesReceived += bytesReceivedNow;
            bytesRemain -= bytesReceivedNow;
            fileOutputStream.write(fileBuf, 0, bytesReceivedNow);
            fileOutputStream.flush();
        }
        long endTime = System.currentTimeMillis();

        System.out.printf("---- File '%s' received! Average speed: %.3f Mb/s ----\n", fileName,
                (fileSize * 1000.0 / (endTime - startTime) / 8 / 1024 / 1024));
    }

    public long getReceivedBytes() {
        return bytesReceived;
    }

    public String getFileName() {
        return fileName;
    }

    public long getFileSize() {
        return fileSize;
    }
}