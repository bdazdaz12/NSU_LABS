package ru.nsu.yevsyukof.controller;

import ru.nsu.yevsyukof.model.factory.FactoryInfrastructure;

public final class Controller {

    private final DelayController engineSupplierDelayController;
    private final DelayController bodySupplierDelayController;
    private final DelayController accessorySupplierDelayController;

    private final DelayController workersDelayController;
    private final DelayController dealersDelayController;

    public Controller(FactoryInfrastructure factoryInfrastructure) {
        engineSupplierDelayController = new DelayController(factoryInfrastructure.getEngineSupplierDelay());
        bodySupplierDelayController = new DelayController(factoryInfrastructure.getBodySupplierDelay());
        accessorySupplierDelayController = new DelayController(factoryInfrastructure.getAccessorySupplierDelay());

        workersDelayController = new DelayController(factoryInfrastructure.getWorkerDelay());
        dealersDelayController = new DelayController(factoryInfrastructure.getDealerDelay());
    }

    public DelayController getEngineSupplierDelayController() {
        return engineSupplierDelayController;
    }

    public DelayController getBodySupplierDelayController() {
        return bodySupplierDelayController;
    }

    public DelayController getAccessorySupplierDelayController() {
        return accessorySupplierDelayController;
    }

    public DelayController getWorkersDelayController() {
        return workersDelayController;
    }

    public DelayController getDealersDelayController() {
        return dealersDelayController;
    }
}
