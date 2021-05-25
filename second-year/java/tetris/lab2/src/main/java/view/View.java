package view;

import model.Model;
import utils.Observer;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;
import java.awt.event.KeyListener;
import java.util.EventListener;

public class View implements Observer, Runnable { // все манипуляции с отрисовкой будем производить через этот класс

    private final Model model;
    private final EventListener eventListener;

    private final JFrame mainWindow;

    private final CellsField cellsField;

    private JMenuBar menuBar;

    private JPanel statisticPanel;
    private JLabel scores;


    public View(Model model, EventListener eventListener) {
        this.model = model;
        this.eventListener = eventListener;

        mainWindow = new JFrame("Tetris");
        mainWindow.setSize(GameConstants.MAIN_WINDOW_WIDTH, GameConstants.MAIN_WINDOW_HEIGHT);
        mainWindow.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        mainWindow.setLocationRelativeTo(null); // размещает главное окно в центре
        mainWindow.addKeyListener((KeyListener) eventListener);

        cellsField = new CellsField();
        createMenuPanel();
        createStatisticPanel();
    }

    private void createMenuPanel() {
        menuBar = new JMenuBar();
        menuBar.setPreferredSize(new Dimension(GameConstants.MENU_BAR_WIDTH, GameConstants.MENU_BAR_HEIGHT));

        JMenuItem exitButton = new JMenuItem("Exit");
        JMenuItem aboutButton = new JMenuItem("About");
        JMenuItem newGameButton = new JMenuItem("New Game");
        JMenuItem highScoresButton = new JMenuItem("High Scores");
        JMenuItem pauseButton = new JMenuItem("Pause");
        JMenuItem continueButton = new JMenuItem("Continue");

        exitButton.addActionListener((ActionListener) eventListener);
        aboutButton.addActionListener((ActionListener) eventListener);
        newGameButton.addActionListener((ActionListener) eventListener);
        highScoresButton.addActionListener((ActionListener) eventListener);
        pauseButton.addActionListener((ActionListener) eventListener);
        continueButton.addActionListener((ActionListener) eventListener);

        menuBar.add(exitButton);
        menuBar.add(aboutButton);
        menuBar.add(newGameButton);
        menuBar.add(highScoresButton);
        menuBar.add(pauseButton);
        menuBar.add(continueButton);
    }

    private void createStatisticPanel() {
        statisticPanel = new JPanel(new FlowLayout(FlowLayout.CENTER));
        statisticPanel = new JPanel();
        statisticPanel.setPreferredSize(
                new Dimension(GameConstants.STATISTIC_PANEL_WIDTH, GameConstants.STATISTIC_PANEL_HEIGHT));

        scores = new JLabel("Scores: " + model.getScores());
        statisticPanel.add(scores);
    }

    public JFrame getMainWindow() {
        return mainWindow;
    }

    public JPanel getCellsField() {
        return cellsField;
    }

    @Override
    public void handleEvent() {
        switch (model.getCurModelState()) {
            case IN_PROCESS -> {
                cellsField.updateCellsField(model.getGameField());
                scores.setText("Scores: " + model.getScores());
            }
            case END -> {
                JOptionPane.showMessageDialog(null, "End of game");
            }
            case PAUSE -> {}
        }
    }

    @Override
    public void run() {
        mainWindow.setJMenuBar(menuBar);
        mainWindow.add(cellsField, BorderLayout.CENTER);
        mainWindow.add(statisticPanel, BorderLayout.EAST);

        JPanel emptyPanel = new JPanel();
        emptyPanel.setPreferredSize(statisticPanel.getPreferredSize());
        mainWindow.add(emptyPanel, BorderLayout.WEST);

        mainWindow.setResizable(false);
        mainWindow.setFocusable(true);
        mainWindow.setVisible(true);
    }
}
