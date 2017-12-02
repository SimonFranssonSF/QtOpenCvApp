#include "leftcolwidget.h"
#include <QtWidgets>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QSlider>
#include <QWidget>
#include <filterbuttons.h>
#include <info.h>
#include <featurebuttons.h>
#include <QDebug>

LeftColWidget::LeftColWidget(QWidget *parent, QLabel* canvas, ComputerVision* computerVision, QLabel* statusBar, std::string mode):QWidget(parent) {
    this->computerVision = computerVision;
    this->canvas = canvas;
    this->statusBar = statusBar;
    this->mode = mode;

    outerBox = new QVBoxLayout;

    QString title = QString::fromStdString(mode);
    title.append(" info");
    infoButtonBox = new QGroupBox(title);
    QHBoxLayout* infoBoxLayout = new QHBoxLayout;
    QPushButton* info = new QPushButton("Info");
    connect(info, SIGNAL(clicked()), this, SLOT(infoButtonClicked()));
    info->setFixedHeight(40);
    info->setFixedWidth(40);
    info->setFlat(false);
    QFont infoFont("Times", 10, QFont::Bold, true);
    infoFont.setItalic(true);
    info->setFont(infoFont);
    infoBoxLayout->addWidget(info);
    infoButtonBox->setLayout(infoBoxLayout);

    if (mode == "filter") {
        groupBoxButtons = FactoryGroupBox::create("filter" ,0, canvas, computerVision, this, statusBar);
    } else if (mode == "feature") {
        groupBoxButtons = FactoryGroupBox::create("feature", 0, canvas, computerVision, this, statusBar);
    }
    groupBoxButtons->enableButtons(false, false);
    outerBox->addWidget(groupBoxButtons);

    boxFilterParams();
    medianFilterParams();
    gaussianFilterParams();
    sobelFilterParams();
    laplacianFilterParams();

    outerBox->addWidget(infoButtonBox);

    outerBox->addStretch();

    hideAllGroupParam();
    infoButtonBox->hide();

    setLayout(outerBox);
}

void LeftColWidget::sliderKernelBox(int value) {
    computerVision->sliderBoxVal = value;
    labelBKernel->setText("Kernel size: "+ QString::number(value));
    this->computerVision->applyBoxFilter(value);
    QImage qImage = this->computerVision->getDisplayImage();
    this->canvas->setPixmap(QPixmap::fromImage(qImage));
}

void LeftColWidget::sliderKernelMedian(int value) {
    computerVision->sliderMedianVal = value;
    // input to medianfilter function needs to be odd
    int val = value;
    if (value % 2 == 0) {
        val += 1;
    }
    labelMKernel->setText("Kernel size: "+ QString::number(val));
    this->computerVision->applyMedianFilter(val);
    QImage qImage = this->computerVision->getDisplayImage();
    this->canvas->setPixmap(QPixmap::fromImage(qImage));
}

void LeftColWidget::sliderKernelGaussian (int value) {
    computerVision->sliderGaussianVal = value;
    // input to medianfilter function needs to be odd
    int val = value;
    if (value % 2 == 0){
        val += 1;
    }
    labelGKernel->setText("Kernel size: "+ QString::number(val));
    this->computerVision->applyGaussianFilter(val, -1);
    QImage qImage = this->computerVision->getDisplayImage();
    this->canvas->setPixmap(QPixmap::fromImage(qImage));
}

void LeftColWidget::sliderSigmaGaussian(int value) {
    computerVision->sliderSigmaVal = value;
    // input to medianfilter function needs to be odd
    double val = value;
    if (val > 0){
        val /= 50;
    }
    labelGSigma->setText("Sigma value: "+ QString::number(val));
    this->computerVision->applyGaussianFilter(-1, val);
    QImage qImage = this->computerVision->getDisplayImage();
    this->canvas->setPixmap(QPixmap::fromImage(qImage));
}


void LeftColWidget::sliderKernelSobel(int value) {
    computerVision->sliderSobelVal = value;
    int val = (value * 2) + 1;

    labelSKernel->setText("Kernel size: "+ QString::number(val));
    try {
        this->computerVision->applySobelFilter(val, -1, -1);
        this->statusBar->setText("");
    } catch (std::invalid_argument &e)  {
        this->statusBar->setText(e.what());
    }
    QImage qImage = this->computerVision->getDisplayImage();
    this->canvas->setPixmap(QPixmap::fromImage(qImage));
}

void LeftColWidget::sliderDxSobel(int value) {
    computerVision->sliderDxVal = value;
    labelSDx->setText("Dx value: "+ QString::number(value));
    try {
        this->computerVision->applySobelFilter(-1, value, -1);
        this->statusBar->setText("");
    } catch (std::invalid_argument &e)  {
        this->statusBar->setText(e.what());
    }
    QImage qImage = this->computerVision->getDisplayImage();
    this->canvas->setPixmap(QPixmap::fromImage(qImage));
}

void LeftColWidget::sliderDySobel(int value) {
    computerVision->sliderDyVal = value;
    labelSDy->setText("Dy value: "+ QString::number(value));
    try {
        this->computerVision->applySobelFilter(-1, -1, value);
        this->statusBar->setText("");
    } catch (std::invalid_argument &e)  {
        this->statusBar->setText(e.what());
    }
    QImage qImage = this->computerVision->getDisplayImage();
    this->canvas->setPixmap(QPixmap::fromImage(qImage));
}

void LeftColWidget::sliderKernelLaplacian(int value) {
    computerVision->sliderLaplacianVal = value;
    // input to medianfilter function needs to be odd
    int val = value;
    if (value % 2 == 0){
        val += 1;
    }
    labelLKernel->setText("Kernel size: "+ QString::number(val));
    this->computerVision->applyLaplacianFilter(val);
    QImage qImage = this->computerVision->getDisplayImage();
    this->canvas->setPixmap(QPixmap::fromImage(qImage));
}

void LeftColWidget::infoButtonClicked() {
    Info* infoWindow = new Info();
    infoWindow->setContent(currentContent);
    infoWindow->show();
    centerWindow(infoWindow);
}

// Not used anymore as I decided to hide/show QgroupBoxes instead of removing replacing widgets over and over in one QGroupBox, but could be necessary for future stuff.
void LeftColWidget::clearWidgets(QLayout * layout) {
   if (layout) {
       while (auto qItem = layout->takeAt(0)) {
          delete qItem->widget();
          clearWidgets(qItem->layout());
       }
   }
}


void LeftColWidget::showCorrectParam(QString buttonName) {
    this->currentContent = buttonName.toStdString();
    hideAllGroupParam();

    if (buttonName == "Box filter" && this->groupBoxBoxParam->isHidden() == true) {
        this->groupBoxBoxParam->show();
    } else if (buttonName == "Median filter") {
        this->groupBoxMedianParam->show();
    } else if (buttonName == "Gaussian filter") {
        this->groupBoxGaussianParam->show();
    } else if (buttonName == "Sobel filter") {
        this->groupBoxSobelParam->show();
    } else if (buttonName == "Laplacian filter") {
        this->groupBoxLaplacianParam->show();
    }

    if (buttonName != "No filter" && buttonName != "No detector" && !buttonName.isNull() && buttonName != "") {
        infoButtonBox->show();
    } else {
        infoButtonBox->hide();
    }
}

void LeftColWidget::boxFilterParams() {

    groupBoxBoxParam = new QGroupBox("Box filter parameters");

    QGroupBox* boxnKernel = new QGroupBox();
    QVBoxLayout* boxLayout = new QVBoxLayout;

    QVBoxLayout* boxKernelLayout = new QVBoxLayout;
    labelBKernel = new QLabel("Kernel size: " + QString::number(computerVision->sliderBoxVal));
    QSlider* slider = new QSlider(Qt::Horizontal);
    slider->setValue(computerVision->sliderBoxVal);
    slider->setSingleStep(1);
    slider->setMinimum(1);
    slider->setMaximum(30);
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(sliderKernelBox(int)));
    boxKernelLayout->addWidget(labelBKernel);
    boxKernelLayout->addWidget(slider);

    boxnKernel->setLayout(boxKernelLayout);
    boxLayout->addWidget(boxnKernel);

    groupBoxBoxParam->setLayout(boxLayout);

    this->outerBox->addWidget(groupBoxBoxParam);
}

void LeftColWidget::medianFilterParams() {
    groupBoxMedianParam = new QGroupBox("Median filter parameters");

    QGroupBox* medianKernel = new QGroupBox();

    QVBoxLayout* medianKernelLayout = new QVBoxLayout;
    int valKern = computerVision->sliderMedianVal;
    if (computerVision->sliderMedianVal % 2 == 0){
        valKern += 1;
    }
    labelMKernel = new QLabel("Kernel size: " + QString::number(valKern));
    QSlider* slider = new QSlider(Qt::Horizontal);
    slider->setValue(computerVision->sliderMedianVal);
    slider->setSingleStep(2);
    slider->setMinimum(1);
    slider->setMaximum(51);
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(sliderKernelMedian(int)));
    medianKernelLayout->addWidget(labelMKernel);
    medianKernelLayout->addWidget(slider);

    medianKernel->setLayout(medianKernelLayout);
    QVBoxLayout* medianLayout = new QVBoxLayout;
    medianLayout->addWidget(medianKernel);

    groupBoxMedianParam->setLayout(medianLayout);
    this->outerBox->addWidget(groupBoxMedianParam);
}

void LeftColWidget::gaussianFilterParams() {
    groupBoxGaussianParam = new QGroupBox("Gaussian filter parameters");
    QVBoxLayout* baseLayout = new QVBoxLayout;

    QGroupBox* groupBoxGaussianKernel = new QGroupBox();
    QVBoxLayout* boxKernel = new QVBoxLayout;
    int valKern = computerVision->sliderGaussianVal;
    if (computerVision->sliderGaussianVal % 2 == 0){
        valKern += 1;
    }
    labelGKernel = new QLabel("Kernel size: " + QString::number(valKern));
    QSlider* sliderKernel = new QSlider(Qt::Horizontal);
    sliderKernel->setValue(computerVision->sliderGaussianVal);
    sliderKernel->setSingleStep(2);
    sliderKernel->setMinimum(1);
    sliderKernel->setMaximum(51);
    connect(sliderKernel,SIGNAL(valueChanged(int)),this,SLOT(sliderKernelGaussian(int)));
    boxKernel->addWidget(labelGKernel);
    boxKernel->addWidget(sliderKernel);
    groupBoxGaussianKernel->setLayout(boxKernel);

    QGroupBox* groupBoxGaussianSigma = new QGroupBox();
    QVBoxLayout* boxSigma = new QVBoxLayout;
    double val = 0;
    if (computerVision->sliderSigmaVal > 0) {
        val = computerVision->sliderSigmaVal / 50;
    }
    labelGSigma = new QLabel("Sigma size: " + QString::number(val));
    QSlider* sliderSigma = new QSlider(Qt::Horizontal);
    sliderSigma->setValue(computerVision->sliderSigmaVal);
    sliderSigma->setSingleStep(1);
    sliderSigma->setMinimum(0);
    sliderSigma->setMaximum(1400);
    connect(sliderSigma,SIGNAL(valueChanged(int)),this,SLOT(sliderSigmaGaussian(int)));
    boxSigma->addWidget(labelGSigma);
    boxSigma->addWidget(sliderSigma);
    groupBoxGaussianSigma->setLayout(boxSigma);

    baseLayout->addWidget(groupBoxGaussianKernel);
    baseLayout->addWidget(groupBoxGaussianSigma);
    baseLayout->addStretch();

    groupBoxGaussianParam->setLayout(baseLayout);
    this->outerBox->addWidget(groupBoxGaussianParam);
}

void LeftColWidget::sobelFilterParams() {
    groupBoxSobelParam = new QGroupBox("Sobel filter parameters");
    QVBoxLayout* baseLayout = new QVBoxLayout;

    QGroupBox* groupBoxSobelKernel = new QGroupBox();
    QVBoxLayout* boxKernel = new QVBoxLayout;
    QSlider* sliderKernel = new QSlider(Qt::Horizontal);
    sliderKernel->setValue(computerVision->sliderSobelVal);
    int val = computerVision->sliderSobelVal * 2 +1;
    labelSKernel = new QLabel("Kernel size: " + QString::number(val));
    sliderKernel->setSingleStep(1);
    sliderKernel->setMinimum(1);
    sliderKernel->setMaximum(3);
    connect(sliderKernel,SIGNAL(valueChanged(int)),this,SLOT(sliderKernelSobel(int)));
    boxKernel->addWidget(labelSKernel);
    boxKernel->addWidget(sliderKernel);
    groupBoxSobelKernel->setLayout(boxKernel);

    QGroupBox* groupBoxSobelDx = new QGroupBox();
    QVBoxLayout* boxDx = new QVBoxLayout;
    labelSDx = new QLabel("Dx value: " + QString::number(computerVision->sliderDxVal));
    QSlider* sliderDx = new QSlider(Qt::Horizontal);
    sliderDx->setValue(computerVision->sliderDxVal);
    sliderDx->setSingleStep(1);
    sliderDx->setMinimum(1);
    sliderDx->setMaximum(6);
    connect(sliderDx,SIGNAL(valueChanged(int)),this,SLOT(sliderDxSobel(int)));
    boxDx->addWidget(labelSDx);
    boxDx->addWidget(sliderDx);
    groupBoxSobelDx->setLayout(boxDx);

    QGroupBox* groupBoxSobelDy = new QGroupBox();
    QVBoxLayout* boxDy = new QVBoxLayout;
    labelSDy = new QLabel("Dy value: " + QString::number(computerVision->sliderDyVal));
    QSlider* sliderDy = new QSlider(Qt::Horizontal);
    sliderDy->setValue(computerVision->sliderDyVal);
    sliderDy->setSingleStep(1);
    sliderDy->setMinimum(1);
    sliderDy->setMaximum(6);
    connect(sliderDy,SIGNAL(valueChanged(int)),this,SLOT(sliderDySobel(int)));
    boxDy->addWidget(labelSDy);
    boxDy->addWidget(sliderDy);
    groupBoxSobelDy->setLayout(boxDy);

    baseLayout->addWidget(groupBoxSobelKernel);
    baseLayout->addWidget(groupBoxSobelDx);
    baseLayout->addWidget(groupBoxSobelDy);

    groupBoxSobelParam->setLayout(baseLayout);
    this->outerBox->addWidget(groupBoxSobelParam);
}

void LeftColWidget::laplacianFilterParams() {
    groupBoxLaplacianParam = new QGroupBox("Laplacian filter parameters");
    QVBoxLayout* baseLayout = new QVBoxLayout;

    QGroupBox* groupBoxLaplacianKernel = new QGroupBox();
    QVBoxLayout* boxKernel = new QVBoxLayout;
    int valKern = computerVision->sliderLaplacianVal;
    if (computerVision->sliderLaplacianVal % 2 == 0){
        valKern += 1;
    }
    labelLKernel = new QLabel("Kernel size: " + QString::number(valKern));
    QSlider* sliderKernel = new QSlider(Qt::Horizontal);
    sliderKernel->setValue(computerVision->sliderLaplacianVal);
    sliderKernel->setSingleStep(2);
    sliderKernel->setMinimum(1);
    sliderKernel->setMaximum(15);
    connect(sliderKernel,SIGNAL(valueChanged(int)),this,SLOT(sliderKernelLaplacian(int)));
    boxKernel->addWidget(labelLKernel);
    boxKernel->addWidget(sliderKernel);
    groupBoxLaplacianKernel->setLayout(boxKernel);

    baseLayout->addWidget(groupBoxLaplacianKernel);

    groupBoxLaplacianParam->setLayout(baseLayout);

    this->outerBox->addWidget(groupBoxLaplacianParam);
}

void LeftColWidget::hideAllGroupParam() {
    groupBoxBoxParam->hide();
    groupBoxMedianParam->hide();
    groupBoxGaussianParam->hide();
    groupBoxSobelParam->hide();
    groupBoxLaplacianParam->hide();
    //groupBoxNoParam->hide();
}

void LeftColWidget::enableButtons(bool enable, bool nofilter) {
    groupBoxButtons->enableButtons(enable, nofilter);
}

void LeftColWidget::centerWindow(QWidget* window) {
    QSize size = window->size();

    QDesktopWidget *desktop = QApplication::desktop();
    int dWidth = desktop->width();
    int dHeight = desktop->height();
    int wWidth = size.width();
    int wHeight = size.height();
    int centerWidth = (dWidth/2) - (wWidth/2);
    int centerHeight = (dHeight/2) - (wHeight/2);
    window->move(centerWidth, centerHeight);
}

QString LeftColWidget::getActiveButton() {
    return QString::fromStdString(currentContent);
};

void LeftColWidget::setActiveButton(QString buttonName) {
    this->groupBoxButtons->setActiveButton(buttonName);
}

void LeftColWidget::passiveButtonClick(QString type) {
   this->groupBoxButtons->passiveButtonClick(type);
}
