package ru.nsu.yevsyukof.CommandsTests;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import ru.nsu.yevsyukof.Commands.Define;
import ru.nsu.yevsyukof.Commands.ExecutableCommand;
import ru.nsu.yevsyukof.Executor.ExecutionContext;

import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class DefineTest {

    @Test
    @DisplayName("Define operation test")
    void execute() {
        ExecutionContext context = new ExecutionContext();
        ExecutableCommand define = new Define();
        List<String> inputArgs = Arrays.asList("a", "2");
        define.execute(context, inputArgs);
        assertTrue(context.getDefines().containsKey("a"));
        assertEquals(2.0, context.getDefines().get("a"), 0.0);
    }
}