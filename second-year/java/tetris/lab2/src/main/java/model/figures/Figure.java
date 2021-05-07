package model.figures;

import view.GameConstants;

import java.awt.*;
import java.util.ArrayList;
import java.util.List;

/*
* 1) Каждая фигура будет располагаться в своем "хитбоксе" == "BOX"
*   Будем отслеживать перемещение этого хитбока по полю, зная координаты его левого верхнего угла
*   Относительно координат хитбокса - будем отсчитывать реальные координаты 4х компонентов фигуры на поле
* 2) Поворот фигуры будет выглядеть как поворот вправо хитбокса на 90 градусов
*   Поворот реализуеться в 2 шага - высчитывание "будущих" координат и
*   проверка возможности расположения фигуры по таким "будущим" координатам
* 3) Для всех фигур хитбокс будет представлять собой матрицу 4х4, но только 1 фигура будет реально её использовать
*   Все остальные будут использовать подматрицу 3х3 этой матрицы, т.е. поворачивать будет она
* 4) Так выглядит БОКС, в котором распролагаються фигуры:
*
*     0 1 2 3  - (координаты по Y)
*     _ _ _ _
*   0|* * * *
*   1|* * * *
*   2|* * * *
*   3|* * * *
*   |
*   (координаты по Х)
*
* 5) В общем для матрицы 4х4 случае отображение повернутых вправо координат происходит по формулам
*       newX = oldY
*       newY = -1 * oldX + 3
* */

public abstract class Figure {

    protected Color figureColor;
    protected List<Coords> inBoxCoords;
    protected Coords boxCoords; // координаты левого верхнего угла "бокса"

    private enum FigureType {
        I, J, L, O, S, T, Z
    }

    public static Figure generateNewFigure() {
        FigureType newFigureType = FigureType.values()[(int) (Math.random() * 7)];
        switch (newFigureType) {
            case I -> { return new I(); }
            case J -> { return new J(); }
            case L -> { return new L(); }
            case O -> { return new O(); }
            case S -> { return new S(); }
            case T -> { return new T(); }
            case Z -> { return new Z(); }
            default -> { return null; } // этот случай никогда не должен выполняться
        }
    }

    protected void clearFigureDraw(Color[][] curGameField) {
        for (Coords square : inBoxCoords) { // очищаем предыдущее положение фигуры
            curGameField[boxCoords.getX() + square.getX()][boxCoords.getY() + square.getY()]
                    = GameConstants.EMPTY_CELL;
        }
    }

    protected void drawFigure(Color[][] curGameField) {
        for (Coords square : inBoxCoords) { // рисуем новое положение фигуры
            curGameField[boxCoords.getX() + square.getX()][boxCoords.getY() + square.getY()] = figureColor;
        }
    }

    protected abstract Coords mapCoords(Coords curCoords);

    public boolean rotateRight(Color[][] curGameField) { // поворот при нажатии стрелки вверх
        clearFigureDraw(curGameField);
        boolean canRotateRight = true;

        List<Coords> newInBoxCoords = new ArrayList<>(4);
        for (Coords square : inBoxCoords) {
            Coords mapped = mapCoords(square);
            if (boxCoords.getX() + mapped.getX() >= GameConstants.GAME_FIELD_HEIGHT ||
                    boxCoords.getY() + mapped.getY() >= GameConstants.GAME_FIELD_WIDTH ||
                    boxCoords.getY() + mapped.getY() < 0 ||
                    !curGameField[boxCoords.getX() + mapped.getX()][boxCoords.getY() + mapped.getY()]
                            .equals(GameConstants.EMPTY_CELL)) {
                canRotateRight = false;
                break;
            } else {
                newInBoxCoords.add(mapped);
            }
        }

        if (canRotateRight) {
            inBoxCoords = newInBoxCoords;
        }
        drawFigure(curGameField);
        return canRotateRight;
    }

    public boolean moveRight(Color[][] curGameField) {
        clearFigureDraw(curGameField);
        boolean canMoveRight = true;

        for (Coords square : inBoxCoords) {
            if (boxCoords.getY() + square.getY() + 1 >= GameConstants.GAME_FIELD_WIDTH ||
                    !curGameField[boxCoords.getX() + square.getX()][boxCoords.getY() + square.getY() + 1]
                            .equals(GameConstants.EMPTY_CELL)) {
                canMoveRight = false;
                break;
            }
        }

        if (canMoveRight) {
            boxCoords.setX(boxCoords.getX());
            boxCoords.setY(boxCoords.getY() + 1);
        }
        drawFigure(curGameField);
        return canMoveRight;
    }

    public boolean moveLeft(Color[][] curGameField) {
        clearFigureDraw(curGameField);
        boolean canMoveLeft = true;

        for (Coords square : inBoxCoords) {
            if (boxCoords.getY() + square.getY() - 1 < 0 ||
                    !curGameField[boxCoords.getX() + square.getX()][boxCoords.getY() + square.getY() - 1]
                            .equals(GameConstants.EMPTY_CELL)) {
                canMoveLeft = false;
                break;
            }
        }

        if (canMoveLeft) {
            boxCoords.setX(boxCoords.getX());
            boxCoords.setY(boxCoords.getY() - 1);
        }
        drawFigure(curGameField);
        return canMoveLeft;
    }

    public boolean slideDown(Color[][] curGameField) {
        clearFigureDraw(curGameField); // баг - фигура не может упасть сама в себя
        boolean canSlideDown = true;

        for (Coords square : inBoxCoords) {
            if (boxCoords.getX() + square.getX() + 1 >= GameConstants.GAME_FIELD_HEIGHT ||
                    !curGameField[boxCoords.getX() + square.getX() + 1][boxCoords.getY() + square.getY()]
                    .equals(GameConstants.EMPTY_CELL)) {
                canSlideDown = false;
                break;
            }
        }

        if (canSlideDown) {
            boxCoords.setX(boxCoords.getX() + 1);
            boxCoords.setY(boxCoords.getY());
        }
        drawFigure(curGameField);
        return canSlideDown;
    }

    public List<Coords> getFigureCoordsOnGameField() {
        List<Coords> onGameFieldCoords = new ArrayList<>(4);
        for (Coords square : inBoxCoords) {
            onGameFieldCoords.add(new Coords(boxCoords.getX() + square.getX(), boxCoords.getY() + square.getY()));
        }
        return onGameFieldCoords;
    }
}
