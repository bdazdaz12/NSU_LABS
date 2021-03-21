package ru.nsu.yevsyukof.CommandsTests;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import ru.nsu.yevsyukof.Commands.Addition;
import ru.nsu.yevsyukof.Commands.ExecutableCommand;
import ru.nsu.yevsyukof.Executor.ExecutionContext;

import static org.junit.jupiter.api.Assertions.*;

class AdditionTest {

    @Test
    @DisplayName("Addition operation test")
    void execute() {
        ExecutionContext context = new ExecutionContext();
        context.getStack().push(1.);
        context.getStack().push(3.);
        ExecutableCommand addition = new Addition();
        addition.execute(context, null);
        assertEquals(4., context.getStack().peek(), 0.0);
    }
}