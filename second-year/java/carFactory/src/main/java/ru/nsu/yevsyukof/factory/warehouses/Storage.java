package ru.nsu.yevsyukof.factory.warehouses;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class Storage <ProductType> {

    private final BlockingQueue<ProductType> blockingQueue;
    private final int capacity;

//    private final NotifyList consumersList; // TODO
//    private final NotifyList suppliersList;

    public Storage(int capacity) {
        this.capacity = capacity;
        blockingQueue = new LinkedBlockingQueue<>(capacity);
    }

    public synchronized void storeProduct(ProductType product) {
        while (isFull()) {
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace(); // TODO обработать прерывание потоков
            }
        }
        blockingQueue.add(product);
        this.notifyAll();
    }

    public synchronized ProductType getProduct() {
        while (isEmpty()) {
            try {
                this.wait();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                // TODO обработать прерывание потоков
            }
        }
        this.notifyAll();
        return blockingQueue.poll();
    }

    public synchronized boolean isFull() {
        return capacity == blockingQueue.size();
    }

    public synchronized boolean isEmpty() {
        return blockingQueue.size() == 0;
    }

    public synchronized int countAvailablePlaces() {
        return capacity - blockingQueue.size();
    }
}

