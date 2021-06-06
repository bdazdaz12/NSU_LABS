package ru.nsu.yevsyukof;

import ru.nsu.yevsyukof.factory.CarStorageController;
import ru.nsu.yevsyukof.factory.FactoryInfrastructure;
import ru.nsu.yevsyukof.factory.products.car.Car;
import ru.nsu.yevsyukof.factory.products.car.parts.Engine;
import ru.nsu.yevsyukof.factory.suppliers.EngineSupplier;
import ru.nsu.yevsyukof.factory.warehouses.Storage;

public class Main {

    public static void main(String[] args) {
        CarFactory.getInstance().run();
    }
}
