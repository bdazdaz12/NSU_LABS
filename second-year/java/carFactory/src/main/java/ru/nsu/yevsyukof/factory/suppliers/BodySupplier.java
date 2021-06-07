package ru.nsu.yevsyukof.factory.suppliers;

import ru.nsu.yevsyukof.factory.Delay;
import ru.nsu.yevsyukof.factory.products.IDIssuingService;
import ru.nsu.yevsyukof.factory.products.car.parts.Body;
import ru.nsu.yevsyukof.factory.warehouses.Storage;

public class BodySupplier extends SupplierThread<Body> {

    public BodySupplier(Storage<Body> destinationStorage, Delay bodySupplierDelay) {
        super(destinationStorage, bodySupplierDelay, "BodySupplierThread");
    }

    @Override
    public Body createProduct() {
        return new Body(IDIssuingService.getNewProductID());
    }
}
