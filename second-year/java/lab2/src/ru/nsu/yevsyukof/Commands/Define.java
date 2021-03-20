package ru.nsu.yevsyukof.Commands;

import ru.nsu.yevsyukof.Executor.ExecutionContext;

import java.util.List;

public class Define extends ExecutableCommand {

    @Override
    public void execute(ExecutionContext context, List<String> inputArgs) {
        try {
            context.getDefines().put(inputArgs.get(0) , Double.valueOf(inputArgs.get(1)));
        } catch (NumberFormatException e) {
            System.err.println("Error when converting to a real number " + e.getLocalizedMessage());
        }
    }
}
