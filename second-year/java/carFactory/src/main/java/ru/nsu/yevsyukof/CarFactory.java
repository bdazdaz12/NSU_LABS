package ru.nsu.yevsyukof;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import ru.nsu.yevsyukof.factory.FactoryInfrastructure;

public class CarFactory implements Runnable {

    private static final Logger logger = LoggerFactory.getLogger(CarFactory.class);
    private static boolean isLogging;

    public synchronized Logger getLogger() {
        return logger;
    }

    private static CarFactory instance;

    private CarFactory() {}

    public static synchronized CarFactory getInstance() {
        if (instance == null) {
            instance = new CarFactory();
        }
        return instance;
    }

    @Override
    public void run() {
        FactoryInfrastructure.getInstance().run();
        //TODO gui run
    }
}
