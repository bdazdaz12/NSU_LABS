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

public class Model implements Observable {

    private List<Observer> observers;

    private ModelStates curModelState;
    private int scores;

    private Color gameField[][];
    private byte countFilledCellsInLine[];
    private Figure curFigure;

    public Model() {
        observers = new LinkedList<>();
        gameField = new Color[GameConstants.GAME_FIELD_HEIGHT][GameConstants.GAME_FIELD_WIDTH];
        countFilledCellsInLine = new byte[GameConstants.GAME_FIELD_WIDTH]; //по умолчанию заполнен нулями
    }

    public void initModel() {
        curModelState = ModelStates.IN_PROGRESS;
        scores = 0;
        initFieldCells();
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
        boolean hasModelChange = true; /// обдумать
        // можно сделать так чтобы вызываемые методы возвращали true при измении модели
        switch (command) {
            case ROTATE -> curFigure.rotateRight();
//            case SLIDE_DOWN -> curFigure.slideDown(); // TODO
            case SLIDE_DOWN -> System.err.println("slideDown");
            case MOVE_LEFT -> curFigure.moveLeft();
            case MOVE_RIGHT -> curFigure.moveRight();
            default -> hasModelChange = false;
        }
        if (hasModelChange) {
            notifyObservers(); // TODO делать только если произошли изменения
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
