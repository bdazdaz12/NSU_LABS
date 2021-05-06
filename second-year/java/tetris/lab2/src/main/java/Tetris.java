import controller.Controller;
import model.Model;
import utils.GameConstants;
import view.View;

import javax.swing.*;

public class Tetris implements Runnable {

    private static Tetris instance;

    private static Controller controller;
    private static View gui;
    private static Model model;
    private static Timer timer;

    private static boolean endOfGame;

    private Tetris() { }

    public static Tetris getInstance() {
        if (instance == null) {
            instance = new Tetris();
        }
        return instance;
    }

    @Override
    public void run() {
        endOfGame = false;

        model = new Model();
        gui = new View(model);
        model.addObserver(gui);
        controller = new Controller(model);

        gui.getMainWindow().addKeyListener(controller);
        javax.swing.SwingUtilities.invokeLater(gui);

        model.initNewModel();
        timer = new Timer(GameConstants.TIMER_DELAY, e -> controller.handleTimerRequest());
        timer.start();
    }
}
