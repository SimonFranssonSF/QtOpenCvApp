#include "factorygroupbox.h"
#include <QGroupBox>
#include <leftcolwidget.h>



FactoryGroupBox* FactoryGroupBox::create(std::string mode, QWidget *parent, QLabel* canvas, ComputerVision* computerVision, LeftColWidget* leftColWidget, QLabel* statusBar) {
    if (mode == "filter") {
        return new FilterButtons(0, canvas, computerVision, leftColWidget, statusBar);
    } else if (mode == "feature") {
        return new FeatureButtons(0, canvas, computerVision, leftColWidget, statusBar);
    } else {
        return NULL;
    }
}

