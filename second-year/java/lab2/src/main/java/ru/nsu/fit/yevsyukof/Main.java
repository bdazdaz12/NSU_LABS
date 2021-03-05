package ru.nsu.fit.yevsyukof;


import ru.nsu.fit.yevsyukof.Executor.StackCalculator;

import java.io.FileNotFoundException;

public class Main {

    public static void main(String[] args) {
        try {
            StackCalculator stackCalculator = new StackCalculator("123.txt");
            stackCalculator.calculate();
        } catch (FileNotFoundException e) {
            System.err.println(e.getLocalizedMessage());
        }
    }
}
