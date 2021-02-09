package yevsyukof.fit;

import java.io.*;

public class ReportService {

    public void printStatistic(String outputFilePath, final StatisticObj[] statistic) {
        try (FileWriter fileWriter = new FileWriter(outputFilePath);){
            for (StatisticObj str: statistic){
                fileWriter.write(str.word + ", ");
                fileWriter.write(str.frequency + ", ");
                fileWriter.write(str.freqPercentage + "\n");
            }
        } catch (IOException e) {
            System.err.print("Error " + e + "when try write at file " + outputFilePath);
        }
    }
}
