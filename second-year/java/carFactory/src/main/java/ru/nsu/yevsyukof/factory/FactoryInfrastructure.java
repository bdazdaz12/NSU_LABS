package ru.nsu.yevsyukof.factory;

import java.io.IOException;
import java.util.Properties;

public class FactoryInfrastructure {

    private static final Properties properties = new Properties();

    public FactoryInfrastructure() {
        try {
            properties.load(FactoryInfrastructure.class.getClassLoader()
                    .getResourceAsStream("factory.properties"));
        } catch (IOException e) {
            e.printStackTrace(); // TODO
        }
    }

}
