package controller;

import model.Model;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

public class Controller implements KeyListener, ActionListener {
    // должен следить за пользовательским "вводом в интерфейс"

    private Model model;

    public Controller(Model model) {
        this.model = model;
    }

    @Override
    public void keyTyped(KeyEvent e) { }

    @Override
    public synchronized void keyPressed(KeyEvent e) {
        switch (e.getKeyCode()) {
            case KeyEvent.VK_DOWN -> model.handleRequest(Command.SLIDE_DOWN);
            case KeyEvent.VK_LEFT -> model.handleRequest(Command.MOVE_LEFT);
            case KeyEvent.VK_RIGHT -> model.handleRequest(Command.MOVE_RIGHT);
            case KeyEvent.VK_UP -> model.handleRequest(Command.ROTATE);
        }
    }

    @Override
    public void keyReleased(KeyEvent e) { }

    @Override
    public void actionPerformed(ActionEvent e) { }

    public void handleTimerRequest() {
        model.handleRequest(Command.SLIDE_DOWN); // опускаем летящую фигуру вниз по таймеру
    }
}
