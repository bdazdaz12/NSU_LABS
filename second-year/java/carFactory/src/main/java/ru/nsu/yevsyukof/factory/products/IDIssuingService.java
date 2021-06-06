package ru.nsu.yevsyukof.factory.products;

public final class IDIssuingService {

    private static long availableProductID = 0;

    public static synchronized long getNewProductID() {
        return availableProductID++;
    }
}
