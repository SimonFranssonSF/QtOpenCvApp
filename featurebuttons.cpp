#include "featurebuttons.h"
#include <QList>
#include <iostream>
#include <computervision.h>
#include <QtDebug>
#include <leftcolwidget.h>
#include <factorygroupbox.h>

FeatureButtons::FeatureButtons(QWidget *parent, QLabel* qLabel, ComputerVision* computerVision, LeftColWidget* leftColWidget, QLabel* statusBar) : FactoryGroupBox() {
    this->qLabel = qLabel;
    this->computerVision = computerVision;
    this->leftColWidget = leftColWidget;
    this->statusBar = statusBar;

    boxButtons = new QVBoxLayout;
    this->setTitle("Feature detectors");
    siftButton = new QPushButton(tr("SIFT"));
    surfButton = new QPushButton(tr("SURF"));
    fastButton = new QPushButton(tr("FAST"));

    buttons.append(siftButton);
    buttons.append(surfButton);
    buttons.append(fastButton);

    applyButtonCharacteristics();

    siftButton->setChecked(true);

    setLayout(boxButtons);
}

void FeatureButtons::applyButtonCharacteristics() {
    for (int i = 0; i < buttons.length(); ++i) {
        buttons[i]->setCheckable(true);
        buttons[i]->setObjectName(buttons[i]->text());
        this->boxButtons->addWidget(buttons[i]);
        try {
            connect(buttons[i], SIGNAL(clicked()), this, SLOT(buttonClicked()));
        } catch (std::invalid_argument &e) {
            this->statusBar->setText(e.what());
        }
    }
}

void FeatureButtons::buttonClicked() {
    resetButtons();
    QObject* button = QObject::sender();
    statusBar->setText("");

}

void FeatureButtons::resetButtons() {
    for (int i = 0; i < this->buttons.length(); ++i) {
        this->buttons[i]->setChecked(false);
    }
}

void FeatureButtons::enableButtons(bool enable) {
    for (int i = 0; i < this->buttons.length(); ++i) {
        this->buttons[i]->setEnabled(enable);
    }
}
