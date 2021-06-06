package ru.nsu.yevsyukof.threadpool;

import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.Executor;

public class ThreadPool implements Executor {

    private final Queue<Runnable> tasksQueue = new ConcurrentLinkedQueue<>();
    private volatile boolean isRunning = true;

    public ThreadPool(int countThreads) {
        for (int i = 0; i < countThreads; ++i) {
            new Thread(new ThreadInPoolWorkScenario(), "ThreadInPool").start();
        }
    }

    @Override
    public void execute(Runnable task) {
        if (isRunning) {
            tasksQueue.offer(task);
        }
    }

    public void shutdown() {
        isRunning = false;
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
