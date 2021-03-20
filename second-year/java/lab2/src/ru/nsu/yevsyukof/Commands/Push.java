package ru.nsu.yevsyukof.Commands;

import ru.nsu.yevsyukof.Executor.ExecutionContext;
import java.util.LinkedList;

public class Push extends ExecutableCommand {

    @Override
    public void execute(ExecutionContext context, LinkedList<Object> inputArgs) {
        try {
            context.getStack().push(Double.valueOf(inputArgs.getFirst().toString()));
        } catch (NumberFormatException e) {
            System.err.println(e.getMessage());
        }
    }

}
