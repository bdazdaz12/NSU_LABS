package ru.nsu.yevsyukof.Factory;


import ru.nsu.yevsyukof.Commands.ExecutableCommand;
import ru.nsu.yevsyukof.Exceptions.CommandNotFoundException;
import ru.nsu.yevsyukof.Executor.ExecutionContext;

import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.LinkedList;
import java.util.Properties;

public class CommandFactory {

    private static final Properties commandsConfig = new Properties();
    private static volatile CommandFactory instance = null;

    private CommandFactory() {
        try {
            commandsConfig.load(Class.class.getResourceAsStream("configuration_file.properties"));
        } catch (IOException e) {
//            throw new RuntimeException(); //todo
        }
    }

    public static synchronized CommandFactory getInstance() {
        if (instance == null) {
            instance = new CommandFactory();
        }
        return instance;
    }

    public ExecutableCommand buildCommand(String commandName) throws CommandNotFoundException {
        if (!commandsConfig.contains(commandName)) {
            throw new CommandNotFoundException("Command " + commandName + " - does not exist");
        }
        ExecutableCommand newCommand = null;
        try {
            newCommand = (ExecutableCommand) Class.forName(commandsConfig.getProperty(commandName)).
                    getDeclaredConstructor().newInstance();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return newCommand;
    }
}
