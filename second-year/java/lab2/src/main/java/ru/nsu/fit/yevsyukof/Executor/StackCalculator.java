package ru.nsu.fit.yevsyukof.Executor;

import ru.nsu.fit.yevsyukof.Factory.CommandFactory;

import java.io.*;

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
        try {
            String nextLine;
            ExecutionContext context = new ExecutionContext();
            while ((nextLine = reader.readLine()) != null) {
                //TODO вызов парсера
                //TODO вызов фабричного метода создания команды из первого аргумента
//                CommandFactory.getInstance().buildCommand();

                //TODO добавление оставшихся аргументов в лист входных аргументов
                //TODO выполнения полученного на 2ом шаге блока с передачей контекста
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
