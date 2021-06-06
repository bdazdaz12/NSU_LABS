package ru.nsu.yevsyukof.factory.suppliers;

import ru.nsu.yevsyukof.factory.products.IDIssuingService;
import ru.nsu.yevsyukof.factory.products.car.parts.Body;
import ru.nsu.yevsyukof.factory.warehouses.Storage;

public class BodySupplier extends SupplierThread<Body> {

    public BodySupplier(Storage<Body> destinationStorage) {
        super(destinationStorage);
    }

    @Override
    public Body createProduct() {
        return new Body(IDIssuingService.getNewProductID());
    }
}
