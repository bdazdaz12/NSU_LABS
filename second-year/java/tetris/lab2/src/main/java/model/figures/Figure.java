package model.figures;

import java.awt.*;

public abstract class Figure {

    public abstract void rotateRight(); // стрелка вверх

    public abstract void moveRight();

    public abstract void moveLeft();

    public abstract void slideDown();

    public abstract Color getFigureColor();
}
