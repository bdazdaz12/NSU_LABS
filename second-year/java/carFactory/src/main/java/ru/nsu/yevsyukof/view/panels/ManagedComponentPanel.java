package ru.nsu.yevsyukof.view.panels;

import ru.nsu.yevsyukof.view.ViewConstants;

import javax.swing.*;
import javax.swing.event.ChangeListener;
import java.awt.*;

public class ManagedComponentPanel extends JPanel {

    private final JLabel componentLabel;

    private final JSlider componentDelaySlider;

    public ManagedComponentPanel(String componentName, ChangeListener changeListener) {
        super(new GridLayout(2, 1, 0, 2));

        this.componentLabel = new JLabel(componentName);

        this.componentDelaySlider = new JSlider(ViewConstants.MIN_DELAY, ViewConstants.MAX_DELAY);
        this.componentDelaySlider.addChangeListener(changeListener);

        addPanelComponents();
    }

    private void addPanelComponents() {
        configureComponents();

        this.add(componentLabel);
        this.add(componentDelaySlider);
    }

    private void configureComponents() {
        componentDelaySlider.setPaintLabels(true); // отрисовка делений
        componentDelaySlider.setMajorTickSpacing(1); // шаг значений ползунка
        componentDelaySlider.setValue(5); // TODO
    }
}

