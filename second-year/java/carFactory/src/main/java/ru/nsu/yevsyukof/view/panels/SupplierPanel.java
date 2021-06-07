package ru.nsu.yevsyukof.view.panels;

import ru.nsu.yevsyukof.model.factory.warehouses.Storage;
import ru.nsu.yevsyukof.utils.Observer;
import ru.nsu.yevsyukof.view.ViewConstants;

import javax.swing.*;
import javax.swing.event.ChangeListener;
import java.awt.*;

public class SupplierPanel extends JPanel implements Observer {

    private final String supplierType;
    private final JLabel supplierLabel;
    private final JProgressBar storageFullnessBar;

    private final JSlider supplierDelaySlider;

    private final Storage<?> observedStorage;

    public SupplierPanel(String supplierType, ChangeListener changeListener, Storage<?> observedStorage) {
        super(new GridLayout(3, 1, 0, 2));

        this.observedStorage = observedStorage;
        observedStorage.addObserver(this);

        this.supplierType = supplierType;
        this.supplierLabel = new JLabel(supplierType);
        this.storageFullnessBar = new JProgressBar(0, 100);

        this.supplierDelaySlider = new JSlider(ViewConstants.MIN_DELAY, ViewConstants.MAX_DELAY);
        this.supplierDelaySlider.addChangeListener(changeListener);

        addPanelComponents();
        setStorageFullness(0, 0);
    }

    private void addPanelComponents() {
        configureComponents();

        this.add(supplierLabel);
        this.add(storageFullnessBar);
        this.add(supplierDelaySlider);
    }

    private void configureComponents() {
        storageFullnessBar.setStringPainted(true);

        supplierDelaySlider.setPaintLabels(true); // отрисовка делений
        supplierDelaySlider.setMajorTickSpacing(1); // шаг значений ползунка
        supplierDelaySlider.setValue(5); // TODO
    }

    public void setStorageFullness(int fullness, int capacity) {
        storageFullnessBar.setValue((int) ((float) fullness / capacity * 100));

        String storageStatus = String.format(supplierType + ": %d / %d", fullness, capacity);
        supplierLabel.setText(storageStatus);
    }

    @Override
    public void handleEvent() {
        synchronized (observedStorage) {
            setStorageFullness(observedStorage.getFullness(), observedStorage.getCapacity());
        }
    }
}

