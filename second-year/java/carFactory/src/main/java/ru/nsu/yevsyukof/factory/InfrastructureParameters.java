package ru.nsu.yevsyukof.factory;

public final class InfrastructureParameters {

    private volatile int ENGINE_SUPPLIER_DELAY = 4;
    private volatile int BODY_SUPPLIER_DELAY = 4;
    private volatile int ACCESSORY_SUPPLIER_DELAY = 4;

    private volatile int DEALER_DELAY = 3;

    private volatile int WORKER_DELAY = 3;


    public synchronized int getEngineSupplierDelay() {
        return ENGINE_SUPPLIER_DELAY;
    }

    public synchronized void setEngineSupplierDelay(int ENGINE_SUPPLIER_DELAY) {
        this.ENGINE_SUPPLIER_DELAY = ENGINE_SUPPLIER_DELAY;
    }

    public synchronized int getBodySupplierDelay() {
        return BODY_SUPPLIER_DELAY;
    }

    public synchronized void setBodySupplierDelay(int BODY_SUPPLIER_DELAY) {
        this.BODY_SUPPLIER_DELAY = BODY_SUPPLIER_DELAY;
    }

    public synchronized int getAccessorySupplierDelay() {
        return ACCESSORY_SUPPLIER_DELAY;
    }

    public synchronized void setAccessorySupplierDelay(int ACCESSORY_SUPPLIER_DELAY) {
        this.ACCESSORY_SUPPLIER_DELAY = ACCESSORY_SUPPLIER_DELAY;
    }

    public synchronized int getDealerDelay() {
        return DEALER_DELAY;
    }

    public synchronized void setDealerDelay(int DEALER_DELAY) {
        this.DEALER_DELAY = DEALER_DELAY;
    }

    public synchronized int getWorkerDelay() {
        return WORKER_DELAY;
    }

    public synchronized void setWorkerDelay(int WORKER_DELAY) {
        this.WORKER_DELAY = WORKER_DELAY;
    }
}
