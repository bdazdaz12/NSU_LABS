package ru.nsu.yevsyukof.model.factory;

import ru.nsu.yevsyukof.model.factory.products.car.Car;
import ru.nsu.yevsyukof.model.factory.products.car.parts.Accessory;
import ru.nsu.yevsyukof.model.factory.products.car.parts.Body;
import ru.nsu.yevsyukof.model.factory.products.car.parts.Engine;
import ru.nsu.yevsyukof.model.factory.warehouses.Storage;
import ru.nsu.yevsyukof.model.factory.workers.BuildCarTask;
import ru.nsu.yevsyukof.threadpool.ThreadPool;

public class CarStorageController extends Thread {

    private final Storage<Car> carStorage;
    private final ThreadPool workersPool;

    private final Delay workerDelay;

    private final Storage<Engine> engineStorage;
    private final Storage<Body> bodyStorage;
    private final Storage<Accessory> accessoryStorage;


    public CarStorageController(Storage<Car> carStorage, ThreadPool workersPool, Delay workerDelay,
                                Storage<Engine> engineStorage, Storage<Body> bodyStorage,
                                Storage<Accessory> accessoryStorage) {
        super("CarStorageController");
        this.carStorage = carStorage;
        this.workersPool = workersPool;

        this.workerDelay = workerDelay;

        this.engineStorage = engineStorage;
        this.bodyStorage = bodyStorage;
        this.accessoryStorage = accessoryStorage;
    }

    @Override
    public void run() {
        while (!Thread.currentThread().isInterrupted()) {
            synchronized (carStorage) {
                while (workersPool.countTasksInQueue() >= carStorage.countAvailablePlaces()) {
                    try {
                        carStorage.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace(); // TODO добавить прерывание
                    }
                }

                for (int i = 0; i < carStorage.countAvailablePlaces() -
                        workersPool.countTasksInQueue() + workersPool.getPoolSize(); ++i) {
                    workersPool.execute(new BuildCarTask(engineStorage, bodyStorage, accessoryStorage,
                            carStorage, workerDelay));
                }
            }
        }
    }
}
