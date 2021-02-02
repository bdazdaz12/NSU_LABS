package yevsyukof.fit;

import java.io.*;

public class ReportService {

    public void printStatistic(String outputFilePath, final StatisticString [] statistic) {
        try {
            FileWriter fileWriter = new FileWriter(outputFilePath);
            for (StatisticString str: statistic){
                fileWriter.write(str.word + ", ");
                fileWriter.write(str.frequency + ", ");
                fileWriter.write(str.freqPercentage + "\n");
            }
            fileWriter.close();
        } catch (IOException e) {
            System.err.print("Error " + e);
        }
    }
}
