package ru.nsu.yevsyukof.factory.dealers;

import ru.nsu.yevsyukof.CarFactory;
import ru.nsu.yevsyukof.factory.Delay;
import ru.nsu.yevsyukof.factory.products.car.Car;
import ru.nsu.yevsyukof.factory.warehouses.Storage;

public class Dealer extends Thread {

    private final Storage<Car> carStorage;

    private final Delay dealerDelay;

    public Dealer(Storage<Car> carStorage, Delay dealerDelay) {
        this.carStorage = carStorage;
        this.dealerDelay = dealerDelay;
    }

    @Override
    public void run() {
        while(!Thread.currentThread().isInterrupted()) {

            Car car = carStorage.getProduct();
            CarFactory.getInstance().getLogger().info(
                    "Получение машины дилером: Dealer {}: Car {} (Engine: {} Body: {} Accessory: {}) ",
                    Thread.currentThread().getId(), car.getProductID(), car.getEngine().getProductID(),
                    car.getBody().getProductID(), car.getAccessory().getProductID());

            synchronized (carStorage) {
                carStorage.notifyAll();
            }

            try {
                Thread.sleep(1000L * dealerDelay.getDelay());
            } catch (InterruptedException e) { // TODO обработать нормально прерывание
                e.printStackTrace();
            }
        }
    }
}
