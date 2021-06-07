package ru.nsu.yevsyukof.threadpool;

import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.Executor;

public class ThreadPool implements Executor {

    private final Queue<Runnable> tasksQueue = new ConcurrentLinkedQueue<>();
    private volatile boolean isRunning = true;
    private int poolSize;

    public ThreadPool(int countThreads) {
        poolSize = countThreads;
        for (int i = 0; i < countThreads; ++i) {
            new Thread(new ThreadInPoolWorkScenario(), "ThreadInPool").start();
        }
    }

    public synchronized int countTasksInQueue() { // использовать осторожно
        return tasksQueue.size();
    }

    @Override
    public synchronized void execute(Runnable task) {
        if (isRunning) {
            tasksQueue.offer(task);
        }
    }

    public synchronized void shutdown() {
        isRunning = false;
    }

    public int getPoolSize() {
        return poolSize;
    }

    private final class ThreadInPoolWorkScenario implements Runnable {

        @Override
        public void run() {
            while (isRunning) {
                Runnable nextThreadTask = tasksQueue.poll();
                if (nextThreadTask != null) {
                    nextThreadTask.run();
                }
            }
        }
    }
}
