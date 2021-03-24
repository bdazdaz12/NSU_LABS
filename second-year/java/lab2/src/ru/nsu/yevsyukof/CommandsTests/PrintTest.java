package ru.nsu.yevsyukof.CommandsTests;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import ru.nsu.yevsyukof.Commands.ExecutableCommand;
import ru.nsu.yevsyukof.Commands.Print;
import ru.nsu.yevsyukof.Executor.ExecutionContext;

import static org.junit.jupiter.api.Assertions.*;

class PrintTest {

    @Test
    @DisplayName("Print operation test")
    void execute() {
        ExecutionContext context = new ExecutionContext();
        context.getStack().push(1.);
        context.getStack().push(3.);
        ExecutableCommand print = new Print();
        print.execute(context, null);
        assertEquals(3., context.getStack().peek(), 0.0);
    }
}