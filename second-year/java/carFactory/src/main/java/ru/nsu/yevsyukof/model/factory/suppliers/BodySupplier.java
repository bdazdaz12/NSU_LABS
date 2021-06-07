package ru.nsu.yevsyukof.model.factory.suppliers;

import ru.nsu.yevsyukof.model.factory.Delay;
import ru.nsu.yevsyukof.model.factory.products.IDIssuingService;
import ru.nsu.yevsyukof.model.factory.products.car.parts.Body;
import ru.nsu.yevsyukof.model.factory.warehouses.Storage;

public class BodySupplier extends SupplierThread<Body> {

    public BodySupplier(Storage<Body> destinationStorage, Delay bodySupplierDelay) {
        super(destinationStorage, bodySupplierDelay, "BodySupplierThread");
    }

    @Override
    public Body createProduct() {
        return new Body(IDIssuingService.getNewProductID());
    }
}
