package ru.nsu.yevsyukof.factory.suppliers;

import ru.nsu.yevsyukof.factory.products.IDIssuingService;
import ru.nsu.yevsyukof.factory.products.car.parts.Accessory;
import ru.nsu.yevsyukof.factory.warehouses.Storage;

public class AccessorySupplier extends SupplierThread<Accessory> {

    public AccessorySupplier(Storage<Accessory> destinationStorage) {
        super(destinationStorage);
    }

    @Override
    public Accessory createProduct() {
        return new Accessory(IDIssuingService.getNewProductID());
    }
}
