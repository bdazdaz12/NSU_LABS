package ru.nsu.yevsyukof.factory;

public final class Delay {

    private volatile int delay; // задержка исполения в секундах

    public Delay(int delay) {
        this.delay = delay;
    }

    public synchronized int getDelay() {
        return delay;
    }

    public synchronized void setDelay(int delay) {
        this.delay = delay;
    }
}
