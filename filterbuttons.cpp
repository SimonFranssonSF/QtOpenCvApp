#include "filterbuttons.h"
#include <QList>
#include <iostream>
#include <computervision.h>
#include <QtDebug>
#include <leftcolwidget.h>
#include <factorygroupbox.h>

FilterButtons::FilterButtons(QWidget *parent, QLabel* qLabel, ComputerVision* computerVision, LeftColWidget* leftColWidget, QLabel* statusBar) : FactoryGroupBox() {
    this->qLabel = qLabel;
    this->computerVision = computerVision;
    this->leftColWidget = leftColWidget;
    this->statusBar = statusBar;

    boxButtons = new QVBoxLayout;
    this->setTitle("Filters");
    noFilterButton = new QPushButton(tr("No filter"));
    boxFilterButton = new QPushButton(tr("Box filter"));
    medianFilterButton = new QPushButton(tr("Median filter"));
    gaussianFilterButton = new QPushButton(tr("Gaussian filter"));
    sobelFilterButton = new QPushButton(tr("Sobel filter"));
    laplacianFilterButton = new QPushButton(tr("Laplacian filter"));

    buttons.append(noFilterButton);
    buttons.append(boxFilterButton);
    buttons.append(medianFilterButton);
    buttons.append(gaussianFilterButton);
    buttons.append(sobelFilterButton);
    buttons.append(laplacianFilterButton);

    applyButtonCharacteristics();

    noFilterButton->setChecked(true);

    setLayout(boxButtons);
}

void FilterButtons::applyButtonCharacteristics() {
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

void FilterButtons::buttonClicked() {
    resetButtons();
    QObject* button = QObject::sender();
    statusBar->setText("");

    if (button == noFilterButton && noFilterButton->isChecked() == false) {
        noFilterButton->setChecked(true);
        this->computerVision->applyNoFilter();
    } else if (button == boxFilterButton && boxFilterButton->isChecked() == false) {
        boxFilterButton->setChecked(true);
        int kernelSize = this->computerVision->getBoxKernel();
        this->computerVision->applyBoxFilter(kernelSize);
    } else if (button == medianFilterButton && medianFilterButton->isChecked() == false) {
        medianFilterButton->setChecked(true);
        int kernelSize = this->computerVision->getMedianKernel();
        this->computerVision->applyMedianFilter(kernelSize);
    } else if (button == gaussianFilterButton && gaussianFilterButton->isChecked() == false) {
        gaussianFilterButton->setChecked(true);
        int kernelSize = this->computerVision->getGaussianKernel();
        int sigma = this->computerVision->getGaussianSigma();
        this->computerVision->applyGaussianFilter(kernelSize, sigma);
    } else if (button == sobelFilterButton && sobelFilterButton->isChecked() == false) {
        sobelFilterButton->setChecked(true);
        try {
            int kernelSize = this->computerVision->getSobelKernel();
            int dx = this->computerVision->getSobelDx();
            int dy = this->computerVision->getSobelDy();
            this->computerVision->applySobelFilter(kernelSize, dx, dy);
        } catch (std::invalid_argument &e)  {
            this->statusBar->setText(e.what());
        }
    } else if (button == laplacianFilterButton && laplacianFilterButton->isChecked() == false) {
        laplacianFilterButton->setChecked(true);
        int kernelSize = this->computerVision->getLaplacianKernel();
        this->computerVision->applyLaplacianFilter(kernelSize);
    }

    QImage qImage = this->computerVision->getDisplayImage();
    this->qLabel->setPixmap(QPixmap::fromImage(qImage));
    this->leftColWidget->showCorrectParam(button);
    if (button != sobelFilterButton) {
        this->statusBar->setText("");
    }
}

void FilterButtons::resetButtons() {
    for (int i = 0; i < this->buttons.length(); ++i) {
        this->buttons[i]->setChecked(false);
    }
}

void FilterButtons::enableButtons(bool enable) {
    for (int i = 0; i < this->buttons.length(); ++i) {
        this->buttons[i]->setEnabled(enable);
    }
}
