package ru.nsu.fit.yevsyukof.Factory;

import ru.nsu.fit.yevsyukof.Commands.ExecutableCommand;
import ru.nsu.fit.yevsyukof.Executor.ExecutionContext;

import java.util.LinkedList;

public class CommandFactory {
    private static CommandFactory instance = null;
    private CommandFactory() {}

    public static synchronized CommandFactory getInstance() {
        if (instance == null) {
            instance = new CommandFactory();
        }
        return instance;
    }

    public ExecutableCommand buildCommand(String commandName) {
        //TODO
        return new ExecutableCommand() {
            @Override
            public void execute(ExecutionContext context, LinkedList<Object> inputArgs) {
                
            }
        };
    }
}
