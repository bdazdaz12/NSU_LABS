package ru.nsu.fit.yevsyukof.Commands;

import ru.nsu.fit.yevsyukof.Executor.ExecutionContext;

import java.util.LinkedList;

public abstract class ExecutableCommand {

    public abstract void execute(ExecutionContext context, LinkedList<Object> inputArgs);
}
