package ru.nsu.fit.yevsyukof.Factory;

import ru.nsu.fit.yevsyukof.Commands.ExecutableCommand;

public class CommandFactory {
    private static CommandFactory instance = null;
    private CommandFactory() {}

    public static synchronized CommandFactory getInstance() {
        if (instance == null) {
            instance = new CommandFactory();
        }
        return instance;
    }

//    public ExecutableCommand buildCommand() {
//        //TODO
//        return new ExecutableCommand();
//    }
}
