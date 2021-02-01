package yevsyukof.fit;

import java.io.*;

public class ReportService {



    public void printStrInFile(final String str, final String outputFilePath) {
        try {
            FileWriter outFile = new FileWriter(outputFilePath);
            outFile.write(str);
            outFile.close();
        } catch (IOException e) {
            System.err.print("Error " + e);
        }
    }
}
