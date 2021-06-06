package ru.nsu.yevsyukof.factory.suppliers;

import ru.nsu.yevsyukof.factory.products.IdentifiableProduct;
import ru.nsu.yevsyukof.factory.warehouses.Storage;

/* По умолчанию в классе Thread уже есть поле "Runnable target", в нем хранится
*  выполняемая этим потоком задача */

abstract class SupplierThread<SuppliedProductType extends IdentifiableProduct> extends Thread {

    protected final Storage<SuppliedProductType> destinationStorage;

    public SupplierThread(Storage<SuppliedProductType> destinationStorage, String threadName) {
        super(threadName);
        this.destinationStorage = destinationStorage;
    }

    public abstract SuppliedProductType createProduct();

    @Override
    public void run() {
        while (!Thread.currentThread().isInterrupted()) {
            destinationStorage.storeProduct(createProduct()); // TODO добавить задержку
        }
    }
}
