package ru.nsu.yevsyukof.Executor;

import java.util.HashMap;
import java.util.Stack;

public class ExecutionContext {

    private Stack<Double> stack;
    private HashMap<String, Double> defines;

    public Stack<Double> getStack() {
        return stack;
    }

    public HashMap<String, Double> getDefines() {
        return defines;
    }
}
