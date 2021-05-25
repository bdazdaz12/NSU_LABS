package model;

import controller.Command;
import model.figures.Coords;
import model.figures.Figure;
import view.GameConstants;
import utils.Observable;
import utils.Observer;

import java.awt.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

/*
 * ИГРОВОЕ ПОЛЕ С КЛЕТКАМИ ориентируется осями X и Y.
 * X - направлена от левого верхнего угла поля ВНИЗ по строкам
 * Y - направлена от левого верхнего угла ВПРАВО по столбцам
 */

public class Model implements Observable {

//    private static final List<Integer> scoresTop = new ArrayList<>();

    private final List<Observer> observers;

    private ModelStates curModelState;
    private int scores;

    private final Color[][] gameField;
    private final byte[] countFilledCellsInLine;
    private Figure curFigure;

    public Model() {
        observers = new LinkedList<>();
        gameField = new Color[GameConstants.GAME_FIELD_HEIGHT][GameConstants.GAME_FIELD_WIDTH];
        countFilledCellsInLine = new byte[GameConstants.GAME_FIELD_HEIGHT];
    }

    public void initNewModel() {
        curModelState = ModelStates.IN_PROCESS;
        scores = 0;
        initNewFieldCells();
        spawnNewFigure();
        Arrays.fill(countFilledCellsInLine, (byte) 0);
        notifyObservers();
    }

    private void initNewFieldCells() {
        for (int i = 0; i < GameConstants.GAME_FIELD_HEIGHT; ++i) {
            Arrays.fill(gameField[i], GameConstants.EMPTY_CELL);
        }
    }

    public synchronized void setModelState(ModelStates newModelState) {
        curModelState = newModelState;
    }

    public synchronized void handleRequest(Command command) {
        if (curModelState.equals(ModelStates.END) || curModelState.equals(ModelStates.PAUSE)) {
            return;
        }
        boolean modelHasChanged = false;

        switch (command) {
            case ROTATE -> {
                modelHasChanged = curFigure.rotateRight(gameField);
                System.err.println("rotate");
            }
            case SLIDE_DOWN -> {
                System.err.println("slide");
                modelHasChanged = curFigure.slideDown(gameField);

                if (!modelHasChanged) {
                    updateCountFilledCellsInLine();
                    int countOfFilledLines = calcNumOfFilledLines();
                    if (countOfFilledLines > 0) {
                        earnPoints(countOfFilledLines);
                        destroyFilledLines(countOfFilledLines);
//                        notifyObservers(); // здесь ли или ниже?
                    }
                    if (isGameEnd()) {
                        curModelState = ModelStates.END;
                    } else {
                        spawnNewFigure();
                    }
                    notifyObservers();
                }
            }
            case MOVE_LEFT -> {
                modelHasChanged = curFigure.moveLeft(gameField);
                System.err.println("moveLeft");
            }
            case MOVE_RIGHT -> {
                modelHasChanged = curFigure.moveRight(gameField);
                System.err.println("moveRight");
            }
            default -> { }
        }

        if (modelHasChanged) {
            System.err.println("model has change");
            notifyObservers();
        }
    }

    private boolean isGameEnd() {
        return countFilledCellsInLine[3] > 0;
    }

    private void destroyFilledLines(int countOfFilledLines) {
        for (int i = 0; i < countOfFilledLines; ++i) {
            for (int x = GameConstants.GAME_FIELD_HEIGHT - 1; x >= 4; --x) {
                if (countFilledCellsInLine[x] == 10) {
                    destroyLine(x);
                }
            }
        }
    }

    private void destroyLine(int lineNum) {
        System.arraycopy(countFilledCellsInLine, 0, countFilledCellsInLine, 1, lineNum);
        for (int x = lineNum; x >= 4; --x) {
            System.arraycopy(gameField[x - 1], 0, gameField[x], 0, GameConstants.GAME_FIELD_WIDTH);
        }
    }

    private void earnPoints(int countOfFilledLines) {
        switch (countOfFilledLines) {
            case 1 -> scores += 100;
            case 2 -> scores += 300;
            case 3 -> scores += 700;
            case 4 -> scores += 1500;
        }
    }

    private void updateCountFilledCellsInLine() {
        for (Coords figureSquare : curFigure.getFigureCoordsOnGameField()) {
            ++countFilledCellsInLine[figureSquare.getX()];
        }
    }

    private int calcNumOfFilledLines() {
        int countFilledLines = 0;
        for (int x = 4; x < GameConstants.GAME_FIELD_HEIGHT; ++x) {
            if (countFilledCellsInLine[x] == 10) {
                ++countFilledLines;
            }
        }
        return countFilledLines;
    }

    private void spawnNewFigure() {
        curFigure = Figure.generateNewFigure();
    }

    public Color[][] getGameField() {
        return gameField;
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
