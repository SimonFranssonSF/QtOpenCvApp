#include "filterwidget.h"
#include <QtWidgets>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QSlider>
#include <QWidget>
#include <filterbuttons.h>
#include <filterinfo.h>

FilterWidget::FilterWidget(QWidget *parent, QLabel* canvas, ComputerVision* computerVision, QLabel* statusBar):QWidget(parent) {
    this->computerVision = computerVision;
    this->canvas = canvas;
    this->statusBar = statusBar;

    groupBoxFilters = new FilterButtons(0, canvas, computerVision, this, statusBar);
    groupBoxFilters->enableButtons(false);

    outerBox = new QVBoxLayout;

    infoButtonBox = new QGroupBox("Filter info");
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

    outerBox->addWidget(groupBoxFilters);

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

void FilterWidget::sliderKernelBox(int value) {
    labelBKernel->setText("Kernel size: "+ QString::number(value));
    this->computerVision->applyBoxFilter(value);
    QImage qImage = this->computerVision->getDisplayImage();
    this->canvas->setPixmap(QPixmap::fromImage(qImage));
}

void FilterWidget::sliderKernelMedian(int value) {
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

void FilterWidget::sliderKernelGaussian (int value) {
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

void FilterWidget::sliderSigmaGaussian(int value) {
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


void FilterWidget::sliderKernelSobel(int value) {
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

void FilterWidget::sliderDxSobel(int value) {
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

void FilterWidget::sliderDySobel(int value) {
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

void FilterWidget::sliderKernelLaplacian(int value) {
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

void FilterWidget::infoButtonClicked() {
    FilterInfo* infoWindow = new FilterInfo();
    infoWindow->setContent(currentFilter);
    infoWindow->show();
    centerWindow(infoWindow);
}

// Not used anymore as I decided to hide/show QgroupBoxes instead of removing replacing widgets over and over in one QGroupBox, but could be necessary for future stuff.
void FilterWidget::clearWidgets(QLayout * layout) {
   if (layout) {
       while (auto qItem = layout->takeAt(0)) {
          delete qItem->widget();
          clearWidgets(qItem->layout());
       }
   }
}


void FilterWidget::showCorrectParam(QObject* button) {
    QString buttonName = button->objectName();
    currentFilter = buttonName.toStdString();
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

    if (buttonName != "No filter") {
        infoButtonBox->show();
    } else {
        infoButtonBox->hide();
    }
}

void FilterWidget::boxFilterParams() {

    groupBoxBoxParam = new QGroupBox("Box filter parameters");

    QGroupBox* boxnKernel = new QGroupBox();
    QVBoxLayout* boxLayout = new QVBoxLayout;

    QVBoxLayout* boxKernelLayout = new QVBoxLayout;
    labelBKernel = new QLabel("Kernel size: 3");
    QSlider* slider = new QSlider(Qt::Horizontal);
    slider->setValue(3);
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

void FilterWidget::medianFilterParams() {
    groupBoxMedianParam = new QGroupBox("Median filter parameters");

    QGroupBox* medianKernel = new QGroupBox();

    QVBoxLayout* medianKernelLayout = new QVBoxLayout;
    labelMKernel = new QLabel("Kernel size: 3");
    QSlider* slider = new QSlider(Qt::Horizontal);
    slider->setValue(3);
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

void FilterWidget::gaussianFilterParams() {
    groupBoxGaussianParam = new QGroupBox("Gaussian filter parameters");
    QVBoxLayout* baseLayout = new QVBoxLayout;

    QGroupBox* groupBoxGaussianKernel = new QGroupBox();
    QVBoxLayout* boxKernel = new QVBoxLayout;
    labelGKernel = new QLabel("Kernel size: 3");
    QSlider* sliderKernel = new QSlider(Qt::Horizontal);
    sliderKernel->setValue(3);
    sliderKernel->setSingleStep(2);
    sliderKernel->setMinimum(1);
    sliderKernel->setMaximum(51);
    connect(sliderKernel,SIGNAL(valueChanged(int)),this,SLOT(sliderKernelGaussian(int)));
    boxKernel->addWidget(labelGKernel);
    boxKernel->addWidget(sliderKernel);
    groupBoxGaussianKernel->setLayout(boxKernel);

    QGroupBox* groupBoxGaussianSigma = new QGroupBox();
    QVBoxLayout* boxSigma = new QVBoxLayout;
    labelGSigma = new QLabel("Sigma size: 0.00");
    QSlider* sliderSigma = new QSlider(Qt::Horizontal);
    sliderSigma->setValue(0.0);
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

void FilterWidget::sobelFilterParams() {
    groupBoxSobelParam = new QGroupBox("Sobel filter parameters");
    QVBoxLayout* baseLayout = new QVBoxLayout;

    QGroupBox* groupBoxSobelKernel = new QGroupBox();
    QVBoxLayout* boxKernel = new QVBoxLayout;
    labelSKernel = new QLabel("Kernel size: 3");
    QSlider* sliderKernel = new QSlider(Qt::Horizontal);
    sliderKernel->setValue(1);
    sliderKernel->setSingleStep(1);
    sliderKernel->setMinimum(1);
    sliderKernel->setMaximum(3);
    connect(sliderKernel,SIGNAL(valueChanged(int)),this,SLOT(sliderKernelSobel(int)));
    boxKernel->addWidget(labelSKernel);
    boxKernel->addWidget(sliderKernel);
    groupBoxSobelKernel->setLayout(boxKernel);

    QGroupBox* groupBoxSobelDx = new QGroupBox();
    QVBoxLayout* boxDx = new QVBoxLayout;
    labelSDx = new QLabel("Dx value: 1");
    QSlider* sliderDx = new QSlider(Qt::Horizontal);
    sliderDx->setValue(1);
    sliderDx->setSingleStep(1);
    sliderDx->setMinimum(1);
    sliderDx->setMaximum(6);
    connect(sliderDx,SIGNAL(valueChanged(int)),this,SLOT(sliderDxSobel(int)));
    boxDx->addWidget(labelSDx);
    boxDx->addWidget(sliderDx);
    groupBoxSobelDx->setLayout(boxDx);

    QGroupBox* groupBoxSobelDy = new QGroupBox();
    QVBoxLayout* boxDy = new QVBoxLayout;
    labelSDy = new QLabel("Dy value: 1");
    QSlider* sliderDy = new QSlider(Qt::Horizontal);
    sliderDy->setValue(1);
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

void FilterWidget::laplacianFilterParams() {
    groupBoxLaplacianParam = new QGroupBox("Laplacian filter parameters");
    QVBoxLayout* baseLayout = new QVBoxLayout;

    QGroupBox* groupBoxLaplacianKernel = new QGroupBox();
    QVBoxLayout* boxKernel = new QVBoxLayout;
    labelLKernel = new QLabel("Kernel size: 3");
    QSlider* sliderKernel = new QSlider(Qt::Horizontal);
    sliderKernel->setValue(3);
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

void FilterWidget::hideAllGroupParam() {
    groupBoxBoxParam->hide();
    groupBoxMedianParam->hide();
    groupBoxGaussianParam->hide();
    groupBoxSobelParam->hide();
    groupBoxLaplacianParam->hide();
    //groupBoxNoParam->hide();
}

void FilterWidget::enableButtons(bool enable) {
    groupBoxFilters->enableButtons(enable);
}

void FilterWidget::centerWindow(QWidget* window) {
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
