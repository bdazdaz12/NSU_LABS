package model.figures;

import java.awt.*;
import java.util.ArrayList;

public class T extends Figure {

    public T() {
        figureColor = Color.magenta;
        boxCoords = new Coords(2, 4);

        inBoxCoords = new ArrayList<>(4);
        inBoxCoords.add(new Coords(0, 1));
        inBoxCoords.add(new Coords(1, 0));
        inBoxCoords.add(new Coords(1, 1));
        inBoxCoords.add(new Coords(1, 2));
    }

    @Override
    protected Coords mapCoords(Coords curCoords) {
        return new Coords(curCoords.getY(), -curCoords.getX() + 2);
    }
}
