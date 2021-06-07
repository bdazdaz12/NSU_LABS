package ru.nsu.yevsyukof;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import ru.nsu.yevsyukof.controller.Controller;
import ru.nsu.yevsyukof.model.factory.FactoryInfrastructure;
import ru.nsu.yevsyukof.view.View;

/*  Этот класс является точкой входа в приложения и организует начальное взаимодействие компонентов MVC */

public class CarFactory implements Runnable {

    private static final Logger logger = LoggerFactory.getLogger(CarFactory.class);
    private static boolean isLogging;

    private final FactoryInfrastructure factoryInfrastructure;

    private final View gui;
    private final Controller controller;

    public synchronized Logger getLogger() {
        return logger;
    }

    private static CarFactory instance;

    private CarFactory() {
        factoryInfrastructure = FactoryInfrastructure.getInstance();
        controller = new Controller(factoryInfrastructure);
        gui = new View(controller, factoryInfrastructure);
    }

    public static synchronized CarFactory getInstance() {
        if (instance == null) {
            instance = new CarFactory();
        }
        return instance;
    }

    @Override
    public void run() {
        factoryInfrastructure.run();
        gui.run();
    }
}
