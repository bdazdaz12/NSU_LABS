package ru.nsu.yevsyukof.Validator;

import ru.nsu.yevsyukof.Commands.ExecutableCommand;
import ru.nsu.yevsyukof.Executor.ExecutionContext;

import java.io.IOException;
import java.util.List;
import java.util.Properties;

public class Validator {

    private static Validator instance = null;
    private String lastDecision;
    private static final Properties contextProperties = new Properties();
    private static final Properties inputArgumentsProperties = new Properties();

    private Validator() {
        try {
            contextProperties.load(Validator.class.getResourceAsStream("context.properties"));
            inputArgumentsProperties.load(Validator.class.getResourceAsStream("inputArguments.properties"));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static Validator getInstance() {
        if (instance == null) {
            instance = new Validator();
        }
        return instance;
    }

    public boolean validate(ExecutableCommand command, ExecutionContext context, List<String> inputArgs) {
        String curCommandName = command.getClass().getName();
        if (context.getStack().size() < Integer.parseInt(contextProperties.getProperty(curCommandName))) {
            lastDecision = curCommandName + ": Not enough arguments in stack to do this command!";
            return false;
        } else if (inputArgs.size() != Integer.parseInt(inputArgumentsProperties.getProperty(curCommandName))) {
            lastDecision = curCommandName + ": Illegal count of input arguments!";
            return false;
        }
        return true;
    }

    public String getLastDecision() {
        return lastDecision;
    }
}
