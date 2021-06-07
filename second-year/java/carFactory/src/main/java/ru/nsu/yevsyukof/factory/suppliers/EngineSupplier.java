package ru.nsu.yevsyukof.factory.suppliers;

import ru.nsu.yevsyukof.factory.Delay;
import ru.nsu.yevsyukof.factory.products.IDIssuingService;
import ru.nsu.yevsyukof.factory.products.car.parts.Engine;
import ru.nsu.yevsyukof.factory.warehouses.Storage;

public class EngineSupplier extends SupplierThread<Engine> {

    public EngineSupplier(Storage<Engine> destinationStorage, Delay engineSupplierDelay) {
        super(destinationStorage, engineSupplierDelay, "EngineSupplierThread");
    }

    @Override
    public Engine createProduct() {
        return new Engine(IDIssuingService.getNewProductID());
    }
}
