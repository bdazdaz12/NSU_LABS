package yevsyukof.fit;

import java.io.*;

public class InputFileReader {

    public String getNextWord(String filePath) throws IOException {
        Reader reader = null;
        try {
            reader = new InputStreamReader(new FileInputStream(filePath));
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        return "123";
    }
}
