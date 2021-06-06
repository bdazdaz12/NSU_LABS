package ru.nsu.yevsyukof.factory.warehouses;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class Storage <ProductType> {

    private final BlockingQueue<ProductType> blockingQueue;
    private final int capacity;

    public Storage(int capacity) {
        this.capacity = capacity;
        blockingQueue = new LinkedBlockingQueue<>(capacity);
    }

    public synchronized void storeProduct(ProductType product) {
        while (isStorageFull()) {
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace(); // TODO
            }
        }
        blockingQueue.add(product);
        this.notifyAll();
    }

    public synchronized ProductType getProduct() {
        while (isStorageEmpty()) {
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace(); // TODO
            }
        }
        this.notifyAll();
        return blockingQueue.poll();
    }

    private synchronized boolean isStorageFull() {
        return capacity == blockingQueue.size();
    }

    private synchronized boolean isStorageEmpty() {
        return blockingQueue.size() == 0;
    }
}

