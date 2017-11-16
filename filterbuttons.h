#ifndef FILTERBUTTON_H
#define FILTERBUTTON_H

#include <QPushButton>
#include <QVBoxLayout>
#include <QSlider>
#include <QList>
#include <factorygroupbox.h>

class FactoryGroupBox;

class FilterButtons : public FactoryGroupBox {
    Q_OBJECT
    public:
        FilterButtons(QWidget *parent = 0, QLabel* qLabel = 0, ComputerVision* computerVision = 0, LeftColWidget* leftColWidget = 0, QLabel* statusBar = 0);
        friend class FactoryGroupBox;
        ComputerVision* computerVision;
        LeftColWidget* leftColWidget;
        void enableButtons(bool enable);
    private:
        QLabel* qLabel;
        QLabel* statusBar;

        QList<QPushButton*> buttons;

        QGroupBox *groupBoxFilters;

        QVBoxLayout *boxButtons;

        QPushButton *noFilterButton;
        QPushButton *boxFilterButton;
        QPushButton *medianFilterButton;
        QPushButton *gaussianFilterButton;
        QPushButton *sobelFilterButton;
        QPushButton *laplacianFilterButton;

        void applyButtonCharacteristics();
        void resetButtons();

    public slots:
        void buttonClicked();
};

#endif // FILTERBUTTON_H
