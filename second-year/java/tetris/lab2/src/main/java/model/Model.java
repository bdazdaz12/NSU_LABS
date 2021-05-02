package model;

import controller.Command;
import model.figures.Figure;
import utils.GameConstants;
import utils.Observable;
import utils.Observer;

import java.awt.*;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

/*
* ИГРОВОЕ ПОЛЕ С КЛЕТКАМИ ориентируется осями X и Y.
* X - направлена от левого верхнего угла поля ВНИЗ по строкам
* Y - направлена от левого верхнего угла ВПРАВО по столбцам
*/

public class Model implements Observable {

    private List<Observer> observers;

    private ModelStates curModelState;
    private int scores;

    private Color[][] gameField;
    private byte[] countFilledCellsInLine;
    private Figure curFigure;

    public Model() {
        observers = new LinkedList<>();
        gameField = new Color[GameConstants.GAME_FIELD_HEIGHT][GameConstants.GAME_FIELD_WIDTH];
        countFilledCellsInLine = new byte[GameConstants.GAME_FIELD_WIDTH]; //по умолчанию заполнен нулями
    }

    public void initModel() {
        curModelState = ModelStates.IN_PROCESS;
        scores = 0;
        initFieldCells();
        curFigure = Figure.generateNewFigure();
        // заспавнить новую фигуру
        notifyObservers();
    }

    private void initFieldCells() {
        for (int i = 0; i < GameConstants.GAME_FIELD_HEIGHT; ++i) {
            Arrays.fill(gameField[i],GameConstants.EMPTY_CELL);
        }
    }

    public synchronized void handleRequest(Command command) {
        if (curModelState.equals(ModelStates.END) || curModelState.equals(ModelStates.PAUSE)) {
            return;
        }
        boolean hasModelChange = false;
        switch (command) {
            case ROTATE -> {
                hasModelChange = curFigure.rotateRight(gameField);
                System.err.println("rotate");
            }
            case SLIDE_DOWN -> {
                hasModelChange = curFigure.slideDown(gameField);
                System.err.println("slide");
                if (!hasModelChange) {
                    curFigure = Figure.generateNewFigure();
                } else {
                    //
                }
            }
            case MOVE_LEFT -> {
                hasModelChange = curFigure.moveLeft(gameField);
                System.err.println("moveLeft");
            }
            case MOVE_RIGHT -> {
                hasModelChange = curFigure.moveRight(gameField);
                System.err.println("moveRight");
            }
            default -> { }
        }
        if (hasModelChange) {
            System.err.println("model has change");
            notifyObservers();
        }
    }

    public Color[][] getGameField() {
        return gameField;
    }

    public byte[] getCountFilledCellsInLine() {
        return countFilledCellsInLine;
    }

    public int getScores() {
        return scores;
    }

    public ModelStates getCurModelState() {
        return curModelState;
    }

    @Override
    public void addObserver(Observer observer) {
        observers.add(observer);
    }

    @Override
    public void removeObserver(Observer observer) {
        observers.remove(observer);
    }

    @Override
    public void notifyObservers() {
        for (Observer observer : observers) {
            observer.handleEvent();
        }
    }
}
