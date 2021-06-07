package ru.nsu.yevsyukof.view.panels;

import ru.nsu.yevsyukof.model.factory.warehouses.Storage;
import ru.nsu.yevsyukof.utils.Observer;

import javax.swing.*;
import java.awt.*;

public class StoragePanel extends JPanel implements Observer  {

    private final String storageName;
    private final JLabel storageLabel;
    private final JProgressBar storageFullnessBar;

    private final Storage<?> observedStorage;

    public StoragePanel(String storageName, Storage<?> observedStorage) {
        super(new GridLayout(2, 1, 0, 2));

        this.observedStorage = observedStorage;
        observedStorage.addObserver(this);

        this.storageName = storageName;
        this.storageLabel = new JLabel(storageName);
        this.storageFullnessBar = new JProgressBar(0, 100);

        addPanelComponents();
        setStorageFullness(0, 0);
    }

    private void addPanelComponents() {
        storageFullnessBar.setStringPainted(true);

        this.add(storageLabel);
        this.add(storageFullnessBar);
    }

    public void setStorageFullness(int fullness, int capacity) {
        storageFullnessBar.setValue((int) ((float) fullness / capacity * 100));

        String storageStatus = String.format(storageName + ": %d / %d", fullness, capacity);
        storageLabel.setText(storageStatus);
    }

    @Override
    public void handleEvent() {
        synchronized (observedStorage) {
            setStorageFullness(observedStorage.getFullness(), observedStorage.getCapacity());
        }
    }
}

