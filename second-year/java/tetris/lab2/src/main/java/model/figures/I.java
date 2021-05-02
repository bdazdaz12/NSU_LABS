package model.figures;

import java.awt.*;
import java.util.ArrayList;

public class I extends Figure {

    public I() {
        figureColor = Color.cyan;
        boxCoords = new Coords(1, 4);

        inBoxCoords = new ArrayList<>(4);
        inBoxCoords.add(new Coords(2, 0));
        inBoxCoords.add(new Coords(2, 1));
        inBoxCoords.add(new Coords(2, 2));
        inBoxCoords.add(new Coords(2, 3));
    }

    // обычный поворот полной матрицы 4х4
    @Override
    protected Coords mapCoords(Coords curCoords) {
        return new Coords(curCoords.getY(), -curCoords.getX() + 3);
    }

}
