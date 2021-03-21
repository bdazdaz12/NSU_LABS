package ru.nsu.yevsyukof.CommandsTests;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import ru.nsu.yevsyukof.Commands.ExecutableCommand;
import ru.nsu.yevsyukof.Commands.Multiplication;
import ru.nsu.yevsyukof.Executor.ExecutionContext;

import static org.junit.jupiter.api.Assertions.*;

class MultiplicationTest {

    @Test
    @DisplayName("Multiplication operation test")
    void execute() {
        ExecutionContext context = new ExecutionContext();
        context.getStack().push(3.);
        context.getStack().push(-3.);
        ExecutableCommand multiplication = new Multiplication();
        multiplication.execute(context, null);
        assertEquals(-9., context.getStack().peek(), 0.0);
    }
}