package ru.nsu.yevsyukof.Executor;


import ru.nsu.yevsyukof.Commands.ExecutableCommand;
import ru.nsu.yevsyukof.Exceptions.BuildCommandException;
import ru.nsu.yevsyukof.Exceptions.CommandNotFoundException;
import ru.nsu.yevsyukof.Factory.CommandFactory;
import ru.nsu.yevsyukof.Validator.Validator;

import java.io.*;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

public class StackCalculator {

    private BufferedReader reader = null;

    public StackCalculator(String inputStreamName) throws FileNotFoundException {
        if (inputStreamName == null) {
            reader = new BufferedReader(new InputStreamReader(System.in));
        } else {
            reader = new BufferedReader(new FileReader(inputStreamName));
        }
    }

    public void calculate() {
        String nextLine;
        ExecutionContext context = new ExecutionContext();
        List<String> inputArgs;
        try {
            while ((nextLine = reader.readLine()) != null) {
                String[] commandLine = nextLine.split("\\s+");
                ExecutableCommand nextCommand = CommandFactory.getInstance().buildCommand(commandLine[0]);
                inputArgs = Arrays.asList(commandLine).subList(1, commandLine.length);
                if (Validator.getInstance().validate(nextCommand, context, inputArgs)) {
                    nextCommand.execute(context, inputArgs);
                } else {
                    System.err.println(Validator.getInstance().getLastDecision());
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        } catch (CommandNotFoundException e) {
            e.printStackTrace();
        } catch (BuildCommandException e) {
            e.printStackTrace();
        }
    }
}
