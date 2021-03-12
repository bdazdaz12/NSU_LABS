package ru.nsu.fit.yevsyukof.Factory;

import ru.nsu.fit.yevsyukof.Commands.ExecutableCommand;
import ru.nsu.fit.yevsyukof.Executor.ExecutionContext;

import java.util.LinkedList;
import java.util.Properties;

public class CommandFactory {
    private static Properties commandConfig = new Properties();
    private static volatile CommandFactory instance = null;

    private CommandFactory() {
        var cnf = CommandFactory.class.getResourceAsStream(con)
    }

    public static synchronized CommandFactory getInstance() {
        if (instance == null) {
            instance = new CommandFactory();
        }
        return instance;
    }

    public ExecutableCommand buildCommand(String commandName) {
        CommandFactory.class.getResourceAsStream("123");
        //TODO
        return new ExecutableCommand() {
            @Override
            public void execute(ExecutionContext context, LinkedList<Object> inputArgs) {
                
            }
        };
    }
}
