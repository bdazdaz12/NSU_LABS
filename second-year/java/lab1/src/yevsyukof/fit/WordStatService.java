package yevsyukof.fit;

import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;

public class WordStatService {
    private HashMap<String, Long> wordStat = new HashMap<>();
    private long cntOfWords = 0;

    public void putWordInStat(String word) {
        Long curVal = 1L;
        cntOfWords++;
        if (wordStat.containsKey(word)) {
            curVal = wordStat.get(word);
            wordStat.replace(word, ++curVal);
        } else {
            wordStat.put(word, curVal);
        }
    }

    public HashMap<String, Long> getWordStat() {
        return wordStat;
    }

    public StatisticObj[] makeStatistic() {
        StatisticObj[] statistic = new StatisticObj[wordStat.size()];
        Iterator<HashMap.Entry<String, Long>> iterator = wordStat.entrySet().iterator();
        int idx = 0;
        for (HashMap.Entry<String, Long> entry : wordStat.entrySet()) {
            statistic[idx] = new StatisticObj();
            statistic[idx].word = entry.getKey();
            statistic[idx].frequency = entry.getValue();
            statistic[idx].freqPercentage = 100. * ((double) entry.getValue()) / cntOfWords;
            idx++;
        }
        Arrays.sort(statistic, new Comparator<StatisticObj>() {
            @Override
            public int compare(StatisticObj o1, StatisticObj o2) {
                return Long.compare(o2.frequency, o1.frequency);
            }
        });
        return statistic;
    }
}
