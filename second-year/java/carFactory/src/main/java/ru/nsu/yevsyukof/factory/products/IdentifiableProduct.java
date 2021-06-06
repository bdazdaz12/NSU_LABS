package ru.nsu.yevsyukof.factory.products;

public class IdentifiableProduct {

    private final long productID;

    public IdentifiableProduct(long newProductID) {
        this.productID = newProductID;
    }

    public long getProductID() {
        return productID;
    }
}
