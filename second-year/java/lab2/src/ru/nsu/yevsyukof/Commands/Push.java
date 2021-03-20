package ru.nsu.yevsyukof.Commands;

import ru.nsu.yevsyukof.Executor.ExecutionContext;

import java.util.List;

public class Push extends ExecutableCommand {

    @Override
    public void execute(ExecutionContext context, List<String> inputArgs) {
        try {
            if (context.getDefines().containsKey(inputArgs.get(0))) {
                context.getStack().push(context.getDefines().get(inputArgs.get(0)));
            } else {
                context.getStack().push(Double.valueOf(inputArgs.get(0)));
            }
        } catch (NumberFormatException e) {
            System.err.println(e.getMessage());
        }
    }

}
