package server;

import java.util.TimerTask;

public class SpeedCounter extends TimerTask {

    long oldBytes = 0;
    SenderThread parent;

    public SpeedCounter(SenderThread parent) {
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
