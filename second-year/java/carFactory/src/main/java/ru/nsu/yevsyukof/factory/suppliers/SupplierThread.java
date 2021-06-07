package ru.nsu.yevsyukof.factory.suppliers;

import ru.nsu.yevsyukof.factory.Delay;
import ru.nsu.yevsyukof.factory.products.IdentifiableProduct;
import ru.nsu.yevsyukof.factory.warehouses.Storage;

/* По умолчанию в классе Thread уже есть поле "Runnable target", в нем хранится
 *  выполняемая этим потоком задача */

abstract class SupplierThread<SuppliedProductType extends IdentifiableProduct> extends Thread {

    protected final Storage<SuppliedProductType> destinationStorage;

    protected final Delay supplierDelay;

    public SupplierThread(Storage<SuppliedProductType> destinationStorage, Delay supplierDelay, String threadName) {
        super(threadName);
        this.destinationStorage = destinationStorage;
        this.supplierDelay = supplierDelay;
    }

    public abstract SuppliedProductType createProduct();

    @Override
    public void run() {
        while (!Thread.currentThread().isInterrupted()) {
            destinationStorage.storeProduct(createProduct());

//            System.err.println(Thread.currentThread().getName() + " поставил продукт ");

            try {
                Thread.sleep(1000L * supplierDelay.getDelay());
            } catch (InterruptedException e) { // TODO обрабатывать номрально прерывание
                e.printStackTrace();
            }
        }
    }
}
