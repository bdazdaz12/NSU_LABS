package ru.nsu.yevsyukof.Commands;

import ru.nsu.yevsyukof.Executor.ExecutionContext;

import java.util.LinkedList;

public abstract class ExecutableCommand {

    public abstract void execute(ExecutionContext context, LinkedList<Object> inputArgs);
}
