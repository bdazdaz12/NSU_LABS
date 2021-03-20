package ru.nsu.yevsyukof.Commands;

import ru.nsu.yevsyukof.Executor.ExecutionContext;

import java.util.List;

public abstract class ExecutableCommand {

    public abstract void execute(ExecutionContext context, List<String> inputArgs);
}
