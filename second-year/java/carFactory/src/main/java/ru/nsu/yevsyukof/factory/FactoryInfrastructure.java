package ru.nsu.yevsyukof.factory;

import ru.nsu.yevsyukof.factory.products.car.Car;
import ru.nsu.yevsyukof.factory.products.car.parts.Accessory;
import ru.nsu.yevsyukof.factory.products.car.parts.Body;
import ru.nsu.yevsyukof.factory.products.car.parts.Engine;
import ru.nsu.yevsyukof.factory.suppliers.AccessorySupplier;
import ru.nsu.yevsyukof.factory.suppliers.BodySupplier;
import ru.nsu.yevsyukof.factory.suppliers.EngineSupplier;
import ru.nsu.yevsyukof.factory.warehouses.Storage;
import ru.nsu.yevsyukof.threadpool.ThreadPool;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

public class FactoryInfrastructure implements Runnable {

    private static final Properties properties = new Properties();

    private static Storage<Engine> engineStorage;
    private static Storage<Body> bodyStorage;
    private static Storage<Accessory> accessoryStorage;
    private static Storage<Car> carStorage;

    private static EngineSupplier engineSupplier;
    private static BodySupplier bodySupplier;
    private static final List<AccessorySupplier> accessorySuppliers = new ArrayList<>();
//    private static final List<AccessorySupplier> accessorySuppliers = new ArrayList<>();
    // dealers
    private static ThreadPool workersPool;
    private static CarStorageController carStorageController;

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

        createStorages();
        createThreads();
        //создаем потоки
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

    private void createThreads() {
        engineSupplier = new EngineSupplier(engineStorage);

        bodySupplier = new BodySupplier(bodyStorage);

        for (int i = 0; i < Integer.parseInt(properties.getProperty("AccessorySuppliersCount")); ++i) {
            accessorySuppliers.add(new AccessorySupplier(accessoryStorage));
        }

        workersPool = new ThreadPool(Integer.parseInt(properties.getProperty("WorkersCount")));

        carStorageController = new CarStorageController(carStorage, workersPool,
                engineStorage, bodyStorage, accessoryStorage);


        // TODO dealers
    }

    @Override
    public void run() {
        // TODO runThreads
    }

    public void shutdown() {
        for (AccessorySupplier accessorySupplier : accessorySuppliers) {
            accessorySupplier.interrupt();
        }
        workersPool.shutdown();

    }
}
