#ifndef FEATUREBUTTONS_H
#define FEATUREBUTTONS_H

#include <QPushButton>
#include <QVBoxLayout>
#include <QSlider>
#include <QList>
#include <factorygroupbox.h>

class FactoryGroupBox;

class FeatureButtons : public FactoryGroupBox {
    Q_OBJECT
    public:
        FeatureButtons(QWidget *parent = 0, QLabel* qLabel = 0, ComputerVision* computerVision = 0, LeftColWidget* leftColWidget = 0, QLabel* statusBar = 0);
        friend class FactoryGroupBox;
        ComputerVision* computerVision;
        LeftColWidget* leftColWidget;
        void enableButtons(bool enable, bool nofilter);
        void setActiveButton(QString buttonName);
        void passiveButtonClick(QString type);
    private:
        QLabel* qLabel;
        QLabel* statusBar;

        QList<QPushButton*> buttons;

        QGroupBox* groupBoxFeatureButton;

        QVBoxLayout* boxButtons;

        QPushButton* noDetector;
        QPushButton* orbButton;
        QPushButton* briskButton;
        QPushButton* fastButton;

        void applyButtonCharacteristics();
        void resetButtons();

    public slots:
        void buttonClicked();
};

#endif // FEATUREBUTTONS_H
