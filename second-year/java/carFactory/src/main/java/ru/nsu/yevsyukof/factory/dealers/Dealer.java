package ru.nsu.yevsyukof.factory.dealers;

import ru.nsu.yevsyukof.CarFactory;
import ru.nsu.yevsyukof.factory.products.car.Car;
import ru.nsu.yevsyukof.factory.warehouses.Storage;

public class Dealer extends Thread {

    private final Storage<Car> carStorage;

    public Dealer(Storage<Car> carStorage) {
        this.carStorage = carStorage;
    }

    @Override
    public void run() {
        while(!Thread.currentThread().isInterrupted()) {
            // TODO добавить задержку

            synchronized (carStorage) {
                Car car = carStorage.getProduct();
                CarFactory.getInstance().getLogger().info(
                        "Получение машины дилером: Dealer {}: Car {} (Engine: {} Body: {} Accessory: {}) ",
                        Thread.currentThread().getId(), car.getProductID(), car.getEngine().getProductID(),
                        car.getBody().getProductID(), car.getAccessory().getProductID());
                carStorage.notify(); // уведомляем контроллер склада машин
            }
        }
    }
}
