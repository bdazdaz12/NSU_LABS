package ru.nsu.yevsyukof.Exceptions;

public class CommandNotFoundException extends Exception {

    public CommandNotFoundException() {
        super();
    }

    public CommandNotFoundException(String message) {
        super(message);
    }

    @Override
    public String getMessage() {
        return super.getMessage();
    }
}
