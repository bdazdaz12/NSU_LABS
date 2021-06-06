package ru.nsu.yevsyukof.factory;

import ru.nsu.yevsyukof.factory.products.car.Car;
import ru.nsu.yevsyukof.factory.products.car.parts.Accessory;
import ru.nsu.yevsyukof.factory.products.car.parts.Body;
import ru.nsu.yevsyukof.factory.products.car.parts.Engine;
import ru.nsu.yevsyukof.factory.warehouses.Storage;
import ru.nsu.yevsyukof.factory.workers.BuildCarTask;
import ru.nsu.yevsyukof.threadpool.ThreadPool;

public class CarStorageController extends Thread {

    private final Storage<Car> carStorage;
    private final ThreadPool workersPool;

    private final Storage<Engine> engineStorage;
    private final Storage<Body> bodyStorage;
    private final Storage<Accessory> accessoryStorage;

    public CarStorageController(Storage<Car> carStorage, ThreadPool workersPool,
                                Storage<Engine> engineStorage, Storage<Body> bodyStorage,
                                Storage<Accessory> accessoryStorage) {
        super("CarStorageController");
        this.carStorage = carStorage;
        this.workersPool = workersPool;

        this.engineStorage = engineStorage;
        this.bodyStorage = bodyStorage;
        this.accessoryStorage = accessoryStorage;
    }

    @Override
    public void run() {
        synchronized (carStorage) {
            while (carStorage.isFull()) {
                try {
                    carStorage.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace(); // TODO
                }
            }
            for (int i = 0; i < carStorage.countAvailablePlaces(); ++i) {
                workersPool.execute(new BuildCarTask(engineStorage, bodyStorage, accessoryStorage, carStorage));
            }
        }
    }
}
