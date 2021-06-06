package ru.nsu.yevsyukof.factory.products.car;

import ru.nsu.yevsyukof.factory.products.car.parts.Accessory;
import ru.nsu.yevsyukof.factory.products.car.parts.Body;
import ru.nsu.yevsyukof.factory.products.car.parts.Engine;
import ru.nsu.yevsyukof.factory.products.IdentifiableProduct;

public class Car extends IdentifiableProduct {

    private final Engine engine;
    private final Body body;
    private final Accessory accessory;

    public Car(long newProductID, Engine engine, Body body, Accessory accessory) {
        super(newProductID);
        this.engine = engine;
        this.body = body;
        this.accessory = accessory;
    }

    public Engine getEngine() {
        return engine;
    }

    public Body getBody() {
        return body;
    }

    public Accessory getAccessory() {
        return accessory;
    }
}
