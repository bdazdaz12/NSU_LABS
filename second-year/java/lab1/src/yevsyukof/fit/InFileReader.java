package yevsyukof.fit;

import java.io.*;

public class InFileReader {

    public String getNextWord(String filePath) throws IOException {
        Reader reader = null;
        try {
            reader = new InputStreamReader(new FileInputStream(filePath));
            StringBuilder stringBuilder = new StringBuilder();
            int nextElem;
            while ((nextElem = reader.read()) != -1){
                if(Character.isAlphabetic((char)nextElem) || Character.isDigit((char)nextElem)){
                    stringBuilder.append((char)nextElem);
                } else {
                    break;
                }
            }
            return stringBuilder.toString();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        return "";
    }
}
