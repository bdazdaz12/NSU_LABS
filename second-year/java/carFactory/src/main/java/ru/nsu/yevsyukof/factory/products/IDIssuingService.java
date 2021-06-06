package ru.nsu.yevsyukof.factory.products;

public final class IDIssuingService {

    private static volatile long availableProductID = 0;

    public static synchronized long getNewProductID() {
        return availableProductID++;
    }
}
