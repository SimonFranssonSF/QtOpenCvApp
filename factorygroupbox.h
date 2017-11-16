#ifndef FACTORYGROUPBOX_H
#define FACTORYGROUPBOX_H

#include <QGroupBox>
#include <QWidget>
#include <QLabel>
#include <computervision.h>

class LeftColWidget;

class FactoryGroupBox : public QGroupBox
{

public:
    static FactoryGroupBox* create(std::string mode, QWidget *parent = 0, QLabel* qLabel = 0, ComputerVision* computerVision = 0, LeftColWidget* leftColWidget = 0, QLabel* statusBar = 0);
    virtual void enableButtons(bool enable){};
public slots:
    virtual void buttonClicked(){};
};

#endif // FACTORYGROUPBOX_H
