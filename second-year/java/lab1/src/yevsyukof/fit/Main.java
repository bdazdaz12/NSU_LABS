package yevsyukof.fit;

import java.io.*;

public class Main {

    public static void main(String[] args) {
        WordStatService stat = new WordStatService();
        try (BufferedReader reader = new BufferedReader(new FileReader(args[0]))){
            String nextLine;
            while ((nextLine = reader.readLine()) != null) {
                for (String nextWord : nextLine.split("[\\W]")) {
                    if (!nextWord.isEmpty()) {
                        stat.putWordInStat(nextWord);
                    }
                }
            }
        } catch (IOException e){
            System.err.println("Error while read file: " + e);
        }
        ReportService reportService = new ReportService();
        reportService.printStatistic(args[1], stat.makeStatistic());
    }
}
