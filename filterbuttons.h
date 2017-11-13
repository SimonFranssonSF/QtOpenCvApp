#ifndef FILTERBUTTON_H
#define FILTERBUTTON_H

#include <QWidget>
#include <filterwidget.h>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QSlider>
#include <QWidget>
#include <QList>
#include <computervision.h>

class FilterWidget;

class FilterButtons : public QGroupBox {
    Q_OBJECT
    public:
        FilterButtons(QWidget *parent = 0, QLabel* qLabel = 0, ComputerVision* computerVision = 0, FilterWidget* filterWidget = 0, QLabel* statusBar = 0);
        ComputerVision* computerVision;
        FilterWidget* filterWidget;
        void enableButtons(bool enable);
    private:
        QLabel* qLabel;
        QLabel* statusBar;

        QList<QPushButton*> buttons;

        QGroupBox *groupBoxFilters;

        QVBoxLayout *boxButtons;

        QPushButton *radioNoFilter;
        QPushButton *radioBFilter;
        QPushButton *radioMFilter;
        QPushButton *radioGFilter;
        QPushButton *radioSFilter;
        QPushButton *radioLFilter;

        void applyButtonCharacteristics();
        void resetButtons();

    public slots:
        void buttonClicked();
};

#endif // FILTERBUTTON_H
