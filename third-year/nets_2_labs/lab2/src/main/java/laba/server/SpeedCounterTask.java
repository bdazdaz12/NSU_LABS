package laba.server;

import java.util.TimerTask;

public class SpeedCounterTask extends TimerTask {

    private long oldBytes = 0;
    private final ReceiverTask parent;

    public SpeedCounterTask(ReceiverTask parent) {
        this.parent = parent;
    }

    @Override
    public void run() {
        long newBytes = parent.getReceivedBytes();
        System.out.printf("Current speed of '%s': %.3f Mb/s, received %.1f %%\n", parent.getFileName(),
                ((newBytes - oldBytes) * 1000.0 / 1000 / 8 / 1024 / 1024),
                ((double) newBytes / parent.getFileSize() * 100.0));
        oldBytes = newBytes;
    }
}
