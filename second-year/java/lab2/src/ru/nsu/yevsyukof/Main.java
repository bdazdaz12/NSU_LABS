package ru.nsu.yevsyukof;

import ru.nsu.yevsyukof.Executor.StackCalculator;

import javax.swing.table.TableRowSorter;
import java.io.FileNotFoundException;

public class Main {

    public static void main(String[] args) {
        try {
            StackCalculator stackCalculator = new StackCalculator(args.length == 0 ? null : args[0]);
            stackCalculator.calculate();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }
}
