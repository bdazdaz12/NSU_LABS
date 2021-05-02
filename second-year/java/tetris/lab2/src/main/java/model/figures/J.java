package model.figures;

import java.awt.*;
import java.util.ArrayList;

public class J extends Figure {

    // эта фигура будет находится в подматрице 3х3, отступаю от её нижней грани 1 строку вверх
    public J() {
        figureColor = Color.blue;
        boxCoords = new Coords(2, 4);

        inBoxCoords = new ArrayList<>(4);
        inBoxCoords.add(new Coords(0, 0));
        inBoxCoords.add(new Coords(1, 0));
        inBoxCoords.add(new Coords(1, 1));
        inBoxCoords.add(new Coords(1, 2));
    }

    // обычный поворот подматрицы 3х3 в левом верхнем углу
    @Override
    protected Coords mapCoords(Coords curCoords) {
        return new Coords(curCoords.getY(), -curCoords.getX() + 2);
    }
}
