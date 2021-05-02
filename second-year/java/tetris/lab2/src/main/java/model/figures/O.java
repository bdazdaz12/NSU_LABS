package model.figures;

import java.awt.*;
import java.util.ArrayList;

public class O extends Figure {

    public O() {
        figureColor = Color.yellow;
        boxCoords = new Coords(1, 4);

        inBoxCoords = new ArrayList<>(4);
        inBoxCoords.add(new Coords(1, 1));
        inBoxCoords.add(new Coords(1, 2));
        inBoxCoords.add(new Coords(2, 1));
        inBoxCoords.add(new Coords(2, 2));
    }

    // аналогично I
    // в принципе пока её вообще можно и не поворачивать
    @Override
    protected Coords mapCoords(Coords curCoords) {
//        return new Coords(curCoords.getY(), -curCoords.getX() + 3);
        return curCoords;
    }
}
