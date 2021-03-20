package ru.nsu.yevsyukof.Commands;

import ru.nsu.yevsyukof.Executor.ExecutionContext;

import java.util.List;

public class Sqrt extends ExecutableCommand {

    @Override
    public void execute(ExecutionContext context, List<String> inputArgs) {
        double a = context.getStack().pop();
        if (a < 0) {
            System.err.println("Extracting the root of a negative number: " + a);
            return;
        }
        context.getStack().push(Math.sqrt(a));
    }
}
