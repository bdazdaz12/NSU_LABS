package laba;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class FileService {

    public static byte[] calculateFileHash(File file)
            throws NoSuchAlgorithmException {
        byte[] buffer = new byte[8192];
        int count;
        MessageDigest digest = MessageDigest.getInstance("SHA-256");
        try (BufferedInputStream bis = new BufferedInputStream(new FileInputStream(file))) {
            while ((count = bis.read(buffer)) > 0) {
                digest.update(buffer, 0, count);
            }
        } catch (IOException e) {
            System.err.println("IO ERROR when calculating the hash");
            e.printStackTrace();
        }

        return digest.digest();
    }
}
