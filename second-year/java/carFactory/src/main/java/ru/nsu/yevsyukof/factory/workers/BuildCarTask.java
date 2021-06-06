package ru.nsu.yevsyukof.factory.workers;

import ru.nsu.yevsyukof.factory.products.IDIssuingService;
import ru.nsu.yevsyukof.factory.products.car.Car;
import ru.nsu.yevsyukof.factory.products.car.parts.Accessory;
import ru.nsu.yevsyukof.factory.products.car.parts.Body;
import ru.nsu.yevsyukof.factory.products.car.parts.Engine;
import ru.nsu.yevsyukof.factory.warehouses.Storage;

public final class BuildCarTask implements Runnable {

    private final Storage<Engine> engineStorage;
    private final Storage<Body> bodyStorage;
    private final Storage<Accessory> accessoryStorage;
    private final Storage<Car> carStorage;

    public BuildCarTask(Storage<Engine> engineStorage, Storage<Body> bodyStorage,
                        Storage<Accessory> accessoryStorage, Storage<Car> carStorage) {
        this.engineStorage = engineStorage;
        this.bodyStorage = bodyStorage;
        this.accessoryStorage = accessoryStorage;
        this.carStorage = carStorage;
    }

    @Override
    public void run() {
        Engine engine = engineStorage.getProduct();
        Body body = bodyStorage.getProduct();
        Accessory accessory = accessoryStorage.getProduct();
        Car newCar = new Car(IDIssuingService.getNewProductID(), engine, body, accessory);
        carStorage.storeProduct(newCar);
    }
}
