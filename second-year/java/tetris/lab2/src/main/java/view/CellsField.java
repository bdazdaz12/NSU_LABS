package view;


import javax.swing.*;
import java.awt.*;

public class CellsField extends JPanel {

    private Cell[][] cellsField;

    public CellsField() {
        initCells();
    }

    private void initCells() {
        this.setSize(GameConstants.CELLS_FIELD_WIDTH * GameConstants.CELL_SIZE,
                GameConstants.CELLS_FIELD_HEIGHT * GameConstants.CELL_SIZE);
        // установлили размер панели
        this.setLayout(new GridLayout(
                GameConstants.CELLS_FIELD_HEIGHT, GameConstants.CELLS_FIELD_WIDTH, 1, 1));
        // задали концепцию добавления элементов на эту панель - таблица из ячеек c отступами 1 и 1

        cellsField = new Cell[GameConstants.CELLS_FIELD_HEIGHT][GameConstants.CELLS_FIELD_WIDTH];
        for (int x = 0; x < GameConstants.CELLS_FIELD_HEIGHT; ++x) {
            for (int y = 0; y < GameConstants.CELLS_FIELD_WIDTH; ++y) {
                cellsField[x][y] = new Cell(x, y);
                this.add(cellsField[x][y]);
            }
        }
    }

    public void updateCellsField(Color[][] gameField) {
        for (int x = 4; x < GameConstants.GAME_FIELD_HEIGHT; ++x) {
            for (int y = 0; y < GameConstants.GAME_FIELD_WIDTH; ++y) {
                cellsField[x - 4][y].setBackground(gameField[x][y]);
            }
        }
    }
}
