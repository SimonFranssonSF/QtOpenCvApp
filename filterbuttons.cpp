#include "filterbuttons.h"
#include <QGroupBox>
#include <QList>
#include <iostream>
#include <computervision.h>
#include <QtDebug>
#include <filterwidget.h>

FilterButtons::FilterButtons(QWidget *parent, QLabel* qLabel, ComputerVision* computerVision, FilterWidget* filterWidget, QLabel* statusBar) : QGroupBox(parent) {
    this->qLabel = qLabel;
    this->computerVision = computerVision;
    this->filterWidget = filterWidget;
    this->statusBar = statusBar;

    boxButtons = new QVBoxLayout;
    this->setTitle("Filters");
    radioNoFilter = new QPushButton(tr("No filter"));
    radioBFilter = new QPushButton(tr("Box filter"));
    radioMFilter = new QPushButton(tr("Median filter"));
    radioGFilter = new QPushButton(tr("Gaussian filter"));
    radioSFilter = new QPushButton(tr("Sobel filter"));
    radioLFilter = new QPushButton(tr("Laplacian filter"));

    buttons.append(radioNoFilter);
    buttons.append(radioBFilter);
    buttons.append(radioMFilter);
    buttons.append(radioGFilter);
    buttons.append(radioSFilter);
    buttons.append(radioLFilter);

    applyButtonCharacteristics();

    radioNoFilter->setChecked(true);

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

    if (button == radioNoFilter && radioNoFilter->isChecked() == false) {
        radioNoFilter->setChecked(true);
        this->computerVision->applyNoFilter();
    } else if (button == radioBFilter && radioBFilter->isChecked() == false) {
        radioBFilter->setChecked(true);
        int kernelSize = this->computerVision->getBoxKernel();
        this->computerVision->applyBoxFilter(kernelSize);
    } else if (button == radioMFilter && radioMFilter->isChecked() == false) {
        radioMFilter->setChecked(true);
        int kernelSize = this->computerVision->getMedianKernel();
        this->computerVision->applyMedianFilter(kernelSize);
    } else if (button == radioGFilter && radioGFilter->isChecked() == false) {
        radioGFilter->setChecked(true);
        int kernelSize = this->computerVision->getGaussianKernel();
        int sigma = this->computerVision->getGaussianSigma();
        this->computerVision->applyGaussianFilter(kernelSize, sigma);
    } else if (button == radioSFilter && radioSFilter->isChecked() == false) {
        radioSFilter->setChecked(true);
        try {
            int kernelSize = this->computerVision->getSobelKernel();
            int dx = this->computerVision->getSobelDx();
            int dy = this->computerVision->getSobelDy();
            this->computerVision->applySobelFilter(kernelSize, dx, dy);
        } catch (std::invalid_argument &e)  {
            this->statusBar->setText(e.what());
        }
    } else if (button == radioLFilter && radioLFilter->isChecked() == false) {
        radioLFilter->setChecked(true);
        int kernelSize = this->computerVision->getLaplacianKernel();
        this->computerVision->applyLaplacianFilter(kernelSize);
    }

    QImage qImage = this->computerVision->getDisplayImage();
    this->qLabel->setPixmap(QPixmap::fromImage(qImage));
    this->filterWidget->showCorrectParam(button);
    if (button != radioSFilter) {
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
