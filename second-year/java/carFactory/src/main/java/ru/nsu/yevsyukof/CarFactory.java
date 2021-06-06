package ru.nsu.yevsyukof;

import org.apache.log4j.Logger;
import org.apache.log4j.spi.LoggerFactory;
import ru.nsu.yevsyukof.factory.FactoryInfrastructure;

public class CarFactory implements Runnable {

    private static Logger logger = LoggerFactory;

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
        // gui run
    }
}
