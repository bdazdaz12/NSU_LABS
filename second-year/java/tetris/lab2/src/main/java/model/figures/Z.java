package model.figures;

import java.awt.*;
import java.util.ArrayList;

public class Z extends Figure {

    public Z() {
        figureColor = Color.red;
        boxCoords = new Coords(2, 4);

        inBoxCoords = new ArrayList<>(4);
        inBoxCoords.add(new Coords(1, 0));
        inBoxCoords.add(new Coords(1, 1));
        inBoxCoords.add(new Coords(2, 1));
        inBoxCoords.add(new Coords(2, 2));
    }

    @Override
    protected Coords mapCoords(Coords curCoords) {
        return new Coords(curCoords.getY(), -curCoords.getX() + 2);
    }
}
