package ru.nsu.yevsyukof.model.factory.workers;

import ru.nsu.yevsyukof.model.factory.Delay;
import ru.nsu.yevsyukof.model.factory.products.IDIssuingService;
import ru.nsu.yevsyukof.model.factory.products.car.Car;
import ru.nsu.yevsyukof.model.factory.products.car.parts.Accessory;
import ru.nsu.yevsyukof.model.factory.products.car.parts.Body;
import ru.nsu.yevsyukof.model.factory.products.car.parts.Engine;
import ru.nsu.yevsyukof.model.factory.warehouses.Storage;

public final class BuildCarTask implements Runnable {

    private final Storage<Engine> engineStorage;
    private final Storage<Body> bodyStorage;
    private final Storage<Accessory> accessoryStorage;
    private final Storage<Car> carStorage;

    private final Delay workerDelay;

    public BuildCarTask(Storage<Engine> engineStorage, Storage<Body> bodyStorage,
                        Storage<Accessory> accessoryStorage, Storage<Car> carStorage, Delay workerDelay) {
        this.engineStorage = engineStorage;
        this.bodyStorage = bodyStorage;
        this.accessoryStorage = accessoryStorage;
        this.carStorage = carStorage;
        this.workerDelay = workerDelay;
    }

    @Override
    public void run() {
        Engine engine = engineStorage.getProduct();
        Body body = bodyStorage.getProduct();
        Accessory accessory = accessoryStorage.getProduct();

        try {
            Thread.sleep(1000L * workerDelay.getDelay()); // TODO TODO TODO
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        Car newCar = new Car(IDIssuingService.getNewProductID(), engine, body, accessory);
        carStorage.storeProduct(newCar);
    }
}
