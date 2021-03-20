package ru.nsu.yevsyukof.Commands;

import ru.nsu.yevsyukof.Executor.ExecutionContext;

import java.util.LinkedList;

public class Multiplication extends ExecutableCommand {

    @Override
    public void execute(ExecutionContext context, LinkedList<Object> inputArgs) {
        try {
            if (inputArgs.size() < 2) {
                throw new IllegalArgumentException("Wrong count of input arguments for command 'MUL'!");
            }
            
        } catch (IllegalArgumentException e) {
            System.out.println(e.getLocalizedMessage());
        }
    }
}
