package ru.nsu.yevsyukof.model.factory.warehouses;

import ru.nsu.yevsyukof.utils.Observable;
import ru.nsu.yevsyukof.utils.Observer;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class Storage <ProductType> implements Observable {

    private final List<Observer> observers;

    private final BlockingQueue<ProductType> blockingQueue;
    private final int capacity;

    public Storage(int capacity) {
        observers = new ArrayList<>(1);

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
        notifyObservers();
    }

    public synchronized ProductType getProduct() {
        while (isEmpty()) {
            try {
                notifyObservers();
                this.wait();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                // TODO обработать прерывание потоков
            }
        }
        this.notifyAll();
        notifyObservers();
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

    public synchronized int getCapacity() {
        return capacity;
    }

    public synchronized int getFullness() {
        return blockingQueue.size();
    }

    @Override
    public void addObserver(Observer observer) {
        observers.add(observer);
    }

    @Override
    public void removeObserver(Observer observer) {
        observers.remove(observer);
    }

    @Override
    public void notifyObservers() {
        for (Observer observer : observers) {
            observer.handleEvent();
        }
    }
}

