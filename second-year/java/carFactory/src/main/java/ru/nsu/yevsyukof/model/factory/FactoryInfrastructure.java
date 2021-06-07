package ru.nsu.yevsyukof.model.factory;

import ru.nsu.yevsyukof.model.factory.dealers.Dealer;
import ru.nsu.yevsyukof.model.factory.products.car.Car;
import ru.nsu.yevsyukof.model.factory.products.car.parts.Accessory;
import ru.nsu.yevsyukof.model.factory.products.car.parts.Body;
import ru.nsu.yevsyukof.model.factory.products.car.parts.Engine;
import ru.nsu.yevsyukof.model.factory.suppliers.AccessorySupplier;
import ru.nsu.yevsyukof.model.factory.suppliers.BodySupplier;
import ru.nsu.yevsyukof.model.factory.suppliers.EngineSupplier;
import ru.nsu.yevsyukof.model.factory.warehouses.Storage;
import ru.nsu.yevsyukof.threadpool.ThreadPool;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

public class FactoryInfrastructure implements Runnable {

    private static final Properties properties = new Properties();

    private Storage<Engine> engineStorage;
    private Storage<Body> bodyStorage;
    private Storage<Accessory> accessoryStorage;
    private Storage<Car> carStorage;

    private EngineSupplier engineSupplier;
    private BodySupplier bodySupplier;
    private final List<AccessorySupplier> accessorySuppliers = new ArrayList<>();

    private ThreadPool workersPool;

    private CarStorageController carStorageController;
    private final List<Dealer> dealers = new ArrayList<>();

    private final Delay engineSupplierDelay;
    private final Delay bodySupplierDelay;
    private final Delay accessorySupplierDelay;
    private final Delay workerDelay;
    private final Delay dealerDelay;

    private static FactoryInfrastructure instance;

    public static synchronized FactoryInfrastructure getInstance() {
        if (instance == null) {
            instance = new FactoryInfrastructure();
        }
        return instance;
    }

    private FactoryInfrastructure() {
        try {
            properties.load(FactoryInfrastructure.class.getClassLoader()
                    .getResourceAsStream("factory.properties"));
        } catch (IOException e) {
            System.err.println("Load factory.properties ERROR!");
            e.printStackTrace();
        }
        engineSupplierDelay = new Delay(1);
        bodySupplierDelay = new Delay(1);
        accessorySupplierDelay = new Delay(1);
        workerDelay = new Delay(1);
        dealerDelay = new Delay(10);

        createStorages();
        createThreads();
    }

    private void createStorages() {
        try {
            engineStorage = new Storage<>(Integer.parseInt(properties.getProperty("EngineStorageSize")));
            bodyStorage = new Storage<>(Integer.parseInt(properties.getProperty("BodyStorageSize")));
            accessoryStorage = new Storage<>(Integer.parseInt(properties.getProperty("AccessoryStorageSize")));
            carStorage = new Storage<>(Integer.parseInt(properties.getProperty("CarStorageSize")));
        } catch (NumberFormatException e) {
            System.err.println("creatStorages ERROR!");
            e.printStackTrace();
        }
    }

    private void createThreads() throws NumberFormatException { // эксепшены
        engineSupplier = new EngineSupplier(engineStorage, engineSupplierDelay);

        bodySupplier = new BodySupplier(bodyStorage, bodySupplierDelay);

        for (int i = 0; i < Integer.parseInt(properties.getProperty("AccessorySuppliersCount")); ++i) {
            accessorySuppliers.add(new AccessorySupplier(accessoryStorage, accessorySupplierDelay));
        }

        workersPool = new ThreadPool(Integer.parseInt(properties.getProperty("WorkersCount")));

        carStorageController = new CarStorageController(carStorage, workersPool, workerDelay,
                engineStorage, bodyStorage, accessoryStorage);

        for (int i = 0; i < Integer.parseInt(properties.getProperty("DealersCount")); ++i) {
            dealers.add(new Dealer(carStorage, dealerDelay));
        }
    }

    @Override
    public void run() {
        engineSupplier.start();
        bodySupplier.start();
        for (AccessorySupplier accessorySupplier : accessorySuppliers) {
            accessorySupplier.start();
        }

        carStorageController.start();
        for (Dealer dealer : dealers) {
            dealer.start();
        }
    }

    public void shutdown() { // прерываем все потоки
        workersPool.shutdown();

        for (AccessorySupplier accessorySupplier : accessorySuppliers) {
            accessorySupplier.interrupt();
        }
        engineSupplier.interrupt();
        bodySupplier.interrupt();


        for (Dealer dealer : dealers) {
            dealer.interrupt();
        }
        carStorageController.interrupt();
    }

    public synchronized Delay getEngineSupplierDelay() {
        return engineSupplierDelay;
    }

    public synchronized Delay getBodySupplierDelay() {
        return bodySupplierDelay;
    }

    public synchronized Delay getAccessorySupplierDelay() {
        return accessorySupplierDelay;
    }

    public synchronized Delay getWorkerDelay() {
        return workerDelay;
    }

    public synchronized Delay getDealerDelay() {
        return dealerDelay;
    }

    public Storage<Engine> getEngineStorage() {
        return engineStorage;
    }

    public Storage<Body> getBodyStorage() {
        return bodyStorage;
    }

    public Storage<Accessory> getAccessoryStorage() {
        return accessoryStorage;
    }

    public Storage<Car> getCarStorage() {
        return carStorage;
    }
}
