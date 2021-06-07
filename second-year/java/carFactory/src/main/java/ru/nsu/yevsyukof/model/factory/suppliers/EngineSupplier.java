package ru.nsu.yevsyukof.model.factory.suppliers;

import ru.nsu.yevsyukof.model.factory.Delay;
import ru.nsu.yevsyukof.model.factory.products.IDIssuingService;
import ru.nsu.yevsyukof.model.factory.products.car.parts.Engine;
import ru.nsu.yevsyukof.model.factory.warehouses.Storage;

public class EngineSupplier extends SupplierThread<Engine> {

    public EngineSupplier(Storage<Engine> destinationStorage, Delay engineSupplierDelay) {
        super(destinationStorage, engineSupplierDelay, "EngineSupplierThread");
    }

    @Override
    public Engine createProduct() {
        return new Engine(IDIssuingService.getNewProductID());
    }
}
