package ru.nsu.fit.yevsyukof.Commands;

import ru.nsu.fit.yevsyukof.Executor.ExecutionContext;
import java.util.EmptyStackException;
import java.util.LinkedList;

public class Pop extends ExecutableCommand {

    @Override
    public void execute(ExecutionContext context, LinkedList<Object> inputArgs) {
        try {
            context.getStack().pop();
        } catch (EmptyStackException e) {
            System.err.println(e.getLocalizedMessage());
        }
    }
}
