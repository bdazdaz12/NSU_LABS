package ru.nsu.yevsyukof.CommandsTests;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.testng.IExpectedExceptionsHolder;
import ru.nsu.yevsyukof.Commands.ExecutableCommand;
import ru.nsu.yevsyukof.Commands.Push;
import ru.nsu.yevsyukof.Executor.ExecutionContext;

import java.util.LinkedList;

import static org.junit.jupiter.api.Assertions.assertEquals;

class PushTest {

    @Test
    @DisplayName("Push operation test")
    void execute() {
        ExecutionContext context = new ExecutionContext();
        context.getStack().push(1.);
        context.getStack().push(3.);
        ExecutableCommand push = new Push();
        push.execute(context, new LinkedList<String>());
        assertEquals(2, context.getStack().size());
        assertEquals(3., context.getStack().peek(), 0.0);
    }
}