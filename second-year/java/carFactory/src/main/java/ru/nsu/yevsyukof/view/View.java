package ru.nsu.yevsyukof.view;

import ru.nsu.yevsyukof.controller.Controller;
import ru.nsu.yevsyukof.model.factory.FactoryInfrastructure;
import ru.nsu.yevsyukof.utils.Observer;
import ru.nsu.yevsyukof.view.panels.ManagedComponentPanel;
import ru.nsu.yevsyukof.view.panels.StoragePanel;
import ru.nsu.yevsyukof.view.panels.SupplierPanel;

import javax.swing.*;
import javax.swing.event.ChangeListener;
import java.awt.*;

// все манипуляции с отрисовкой будем производить через этот класс
public class View implements Runnable {

    private final JFrame mainWindow;
    private final JPanel mainPanel;

    private SupplierPanel engineSupplierPanel;
    private SupplierPanel bodySupplierPanel;
    private SupplierPanel accessorySupplierPanel;

    private ManagedComponentPanel workerDelayPanel;
    private ManagedComponentPanel dealerDelayPanel;

    private StoragePanel carStoragePanel;

    public View(Controller controller, FactoryInfrastructure factoryInfrastructure) {
        mainWindow = new JFrame("Car factory");
        mainWindow.setSize(ViewConstants.MAIN_WINDOW_WIDTH, ViewConstants.MAIN_WINDOW_HEIGHT);
        mainWindow.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        mainWindow.setLocationRelativeTo(null); // размещает главное окно в центре
        mainWindow.setLayout(new BorderLayout());

        mainPanel = new JPanel(new GridLayout(6, 1, 0, 5));
        createFactoryComponentsPanels(controller, factoryInfrastructure);
    }

    private void createFactoryComponentsPanels(Controller controller, FactoryInfrastructure factoryInfrastructure) {
        engineSupplierPanel = new SupplierPanel("Engine storage",
                controller.getEngineSupplierDelayController(), factoryInfrastructure.getEngineStorage());
        bodySupplierPanel = new SupplierPanel("Body storage",
                controller.getBodySupplierDelayController(), factoryInfrastructure.getBodyStorage());
        accessorySupplierPanel = new SupplierPanel("Accessory storage",
                controller.getAccessorySupplierDelayController(), factoryInfrastructure.getAccessoryStorage());

        carStoragePanel = new StoragePanel("Car storage", factoryInfrastructure.getCarStorage());

        dealerDelayPanel = new ManagedComponentPanel("Dealers delay:",
                controller.getDealersDelayController());
        workerDelayPanel = new ManagedComponentPanel("Workers delay:",
                controller.getWorkersDelayController());

        mainPanel.add(engineSupplierPanel);
        mainPanel.add(bodySupplierPanel);
        mainPanel.add(accessorySupplierPanel);

        mainPanel.add(carStoragePanel);
        mainPanel.add(dealerDelayPanel);
        mainPanel.add(workerDelayPanel);
    }

    private void addSideBordersAtMainWindow() {
        JPanel emptyPanel = new JPanel();
        emptyPanel.setPreferredSize(new Dimension(10, 0));
        mainWindow.add(emptyPanel, BorderLayout.WEST);

        JPanel emptyPanel1 = new JPanel();
        emptyPanel.setPreferredSize(new Dimension(10, 0));
        mainWindow.add(emptyPanel1, BorderLayout.EAST);
    }

    @Override
    public void run() {
        addSideBordersAtMainWindow();

        mainWindow.add(mainPanel);

//        mainWindow.setResizable(false);
        mainWindow.setFocusable(true);
        mainWindow.setVisible(true);
    }
}
