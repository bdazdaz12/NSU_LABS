package ru.nsu.yevsyukof.Executor;

import java.util.HashMap;
import java.util.Stack;

public class ExecutionContext {

    private final Stack<Double> stack = new Stack<>();
    private final HashMap<String, Double> defines = new HashMap<>();

    public Stack<Double> getStack() {
        return stack;
    }

    public HashMap<String, Double> getDefines() {
        return defines;
    }
}
