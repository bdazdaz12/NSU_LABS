package ru.nsu.yevsyukof.Factory;


import ru.nsu.yevsyukof.Commands.ExecutableCommand;
import ru.nsu.yevsyukof.Exceptions.BuildCommandException;
import ru.nsu.yevsyukof.Exceptions.CommandNotFoundException;

import java.io.IOException;
import java.util.Properties;

public class CommandFactory {

    private static final Properties commandsConfig = new Properties();
    private static volatile CommandFactory instance = null;

    private CommandFactory() {
        try {
            commandsConfig.load(CommandFactory.class.getResourceAsStream("configuration_file.properties"));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static synchronized CommandFactory getInstance() {
        if (instance == null) {
            instance = new CommandFactory();
        }
        return instance;
    }

    public ExecutableCommand buildCommand(String commandName) throws CommandNotFoundException, BuildCommandException {
        if (!commandsConfig.containsKey(commandName)) {
            throw new CommandNotFoundException("Command " + commandName + " - does not exist");
        }
        ExecutableCommand newCommand;
        try {
            newCommand = (ExecutableCommand) Class.forName(commandsConfig.getProperty(commandName)).
                    getDeclaredConstructor().newInstance();
        } catch (Exception e) {
            throw  new BuildCommandException();
        }
        return newCommand;
    }
}
