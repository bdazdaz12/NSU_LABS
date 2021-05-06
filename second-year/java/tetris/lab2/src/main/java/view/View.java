package view;

import model.Model;
import model.ModelStates;
import utils.GameConstants;
import utils.Observer;

import javax.swing.*;
import java.awt.*;

public class View implements Observer, Runnable { // все манипуляции с отрисовкой будем производить через этот класс

    private final JFrame mainWindow;
    private final CellsField cellsField;
    private final Model model;

    public View(Model model) {
        this.model = model;

        mainWindow = new JFrame("Tetris");
        mainWindow.setSize(GameConstants.MAIN_WINDOW_WIDTH, GameConstants.MAIN_WINDOW_HEIGHT);
        mainWindow.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        mainWindow.setLayout(null); // не дает ресайзить поле из клеток
        mainWindow.setLocationRelativeTo(null); // размещает главное окно в центре

        cellsField = new CellsField();
    }

    public JFrame getMainWindow() {
        return mainWindow;
    }

    public JPanel getCellsField() {
        return cellsField;
    }

    @Override
    public void handleEvent() {
        switch (model.getCurModelState()) {
            case IN_PROCESS -> {
                cellsField.updateCellsField(model.getGameField());
            }
            case END -> {}
            case PAUSE -> {}
        }
    }

    @Override
    public void run() { // TODO запустить эту штуку отложенно
        mainWindow.add(cellsField, BorderLayout.CENTER);
        mainWindow.setVisible(true);
    }
}
