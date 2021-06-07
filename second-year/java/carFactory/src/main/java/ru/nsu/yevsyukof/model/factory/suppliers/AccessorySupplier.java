package ru.nsu.yevsyukof.model.factory.suppliers;

import ru.nsu.yevsyukof.model.factory.Delay;
import ru.nsu.yevsyukof.model.factory.products.IDIssuingService;
import ru.nsu.yevsyukof.model.factory.products.car.parts.Accessory;
import ru.nsu.yevsyukof.model.factory.warehouses.Storage;

public class AccessorySupplier extends SupplierThread<Accessory> {

    public AccessorySupplier(Storage<Accessory> destinationStorage, Delay accessorySupplierDelay) {
        super(destinationStorage, accessorySupplierDelay, "AccessorySupplierThread");
    }

    @Override
    public Accessory createProduct() {
        return new Accessory(IDIssuingService.getNewProductID());
    }
}
