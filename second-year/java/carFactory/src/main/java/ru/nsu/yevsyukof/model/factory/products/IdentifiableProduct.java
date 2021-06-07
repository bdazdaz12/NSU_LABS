package ru.nsu.yevsyukof.model.factory.products;

public class IdentifiableProduct {

    private final long productID;

    public IdentifiableProduct(long newProductID) {
        this.productID = newProductID;
    }

    public long getProductID() {
        return productID;
    }
}
