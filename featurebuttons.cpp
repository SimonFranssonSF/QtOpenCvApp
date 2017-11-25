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
    noDetector = new QPushButton(tr("No Detector"));
    siftButton = new QPushButton(tr("SIFT"));
    surfButton = new QPushButton(tr("SURF"));
    fastButton = new QPushButton(tr("FAST"));

    buttons.append(noDetector);
    buttons.append(siftButton);
    buttons.append(surfButton);
    buttons.append(fastButton);

    applyButtonCharacteristics();

    //noDetector->setChecked(true);

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
    this->leftColWidget->currentContent = button->objectName().toStdString();

    if (button == siftButton && siftButton->isChecked() == false) {
        siftButton->setChecked(true);
        computerVision->applySIFT();
    } else if (button == surfButton && surfButton->isChecked() == false) {
        surfButton->setChecked(true);
        computerVision->applySURF();
    } else if (button == fastButton && fastButton->isChecked() == false) {
        fastButton->setChecked(true);
        computerVision->applyFAST();
    } else if (button == noDetector && fastButton->isChecked() == false) {
        noDetector->setChecked(true);
        computerVision->applyNoDetector();
    }
    QImage qImage = this->computerVision->getDisplayImageFeatures();
    this->qLabel->setPixmap(QPixmap::fromImage(qImage));
    this->leftColWidget->showCorrectParam(button->objectName());
}

void FeatureButtons::passiveButtonClick(QString type) {
    if (type == "SIFT") {
        computerVision->applySIFT();
    } else if (type == "SURF") {
        computerVision->applySURF();
    } else if (type == "FAST") {
        computerVision->applyFAST();
    }
    QImage qImage = this->computerVision->getDisplayImageFeatures();
    this->qLabel->setPixmap(QPixmap::fromImage(qImage));
    this->leftColWidget->showCorrectParam(type);
}

void FeatureButtons::resetButtons() {
    for (int i = 0; i < this->buttons.length(); ++i) {
        this->buttons[i]->setChecked(false);
    }
}

void FeatureButtons::enableButtons(bool enable, bool nofilter) {
    for (int i = 0; i < this->buttons.length(); ++i) {
        this->buttons[i]->setEnabled(enable);
    }

    if (enable && nofilter) {
        buttons[0]->setChecked(true);
    }
}

void FeatureButtons::setActiveButton(QString buttonName) {
    for (int i = 0; i < buttons.length(); i++) {
        if(buttons[i]->text() == buttonName) {
            buttons[i]->setChecked(true);
        }
    }
}
