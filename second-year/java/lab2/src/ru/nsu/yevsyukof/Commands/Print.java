package ru.nsu.yevsyukof.Commands;

import ru.nsu.yevsyukof.Executor.ExecutionContext;

import java.util.List;

public class Print extends ExecutableCommand {

    @Override
    public void execute(ExecutionContext context, List<String> inputArgs) {
        System.out.println(context.getStack().peek());
    }
}
