package view;

import java.awt.*;

public final class GameConstants {

    public static final int GAME_FIELD_WIDTH = 10;  // matrix in model
    public static final int GAME_FIELD_HEIGHT = 20 + 4; // добавление места для генерации фигур

    public static final int CELLS_FIELD_WIDTH = 10;  // cnt of cells panels
    public static final int CELLS_FIELD_HEIGHT = 20;
    public static final Color CELL_COLOR = Color.white;
    public static final Color EMPTY_CELL = CELL_COLOR;

    public static final int CELL_SIZE = 30; // cnt of pixels in cell
    public static final int VIEW_FIELD_WIDTH = CELLS_FIELD_WIDTH * CELL_SIZE + CELLS_FIELD_WIDTH - 1;
    public static final int VIEW_FIELD_HEIGHT = CELLS_FIELD_HEIGHT * CELL_SIZE + CELLS_FIELD_HEIGHT - 1;

    public static final int MENU_BAR_WIDTH = VIEW_FIELD_WIDTH; // не имеет смысла, т.к. растягивается
    public static final int MENU_BAR_HEIGHT = 20;

    public static final int STATISTIC_PANEL_WIDTH = VIEW_FIELD_WIDTH / 2; // cnt of pixels
    public static final int STATISTIC_PANEL_HEIGHT = 30; // не имеет смысла, т.к. растягивается

    public static final int MAIN_WINDOW_WIDTH = VIEW_FIELD_WIDTH + STATISTIC_PANEL_WIDTH * 2; // cnt of pixels
    public static final int MAIN_WINDOW_HEIGHT = VIEW_FIELD_HEIGHT + MENU_BAR_HEIGHT;

    public static final int TIMER_DELAY = 500;


}
