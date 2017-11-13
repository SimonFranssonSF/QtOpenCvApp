#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QSlider>
#include <QWidget>
#include <filterbuttons.h>
#include <computervision.h>

class FilterButtons;

class FilterWidget : public QWidget {
    Q_OBJECT
    public:
        FilterWidget(QWidget *parent = 0, QLabel* canvas = 0, ComputerVision* computerVision = 0, QLabel* statusBar = 0);
        ComputerVision* computerVision;
        QLabel* canvas;
        QLabel* statusBar;
        void showCorrectParam(QObject* button);
        void enableButtons(bool enable);
    private:
        FilterButtons* groupBoxFilters;

        QGroupBox* groupBoxParameters;

        QGroupBox* groupBoxBoxParam;
        QGroupBox* groupBoxMedianParam;
        QGroupBox* groupBoxGaussianParam;
        QGroupBox* groupBoxSobelParam;
        QGroupBox* groupBoxLaplacianParam;

        QGroupBox* infoButtonBox;

        QVBoxLayout *outerBox;

        QHBoxLayout *layout;

        QLabel* labelBKernel;

        QLabel* labelMKernel;

        QLabel* labelGKernel;
        QLabel* labelGSigma;

        QLabel* labelSKernel;
        QLabel* labelSDx;
        QLabel* labelSDy;

        QLabel* labelLKernel;

        std::string currentFilter;

        void clearWidgets(QLayout * layout);
        void boxFilterParams();
        void medianFilterParams();
        void gaussianFilterParams();
        void sobelFilterParams();
        void laplacianFilterParams();
        void hideAllGroupParam();
        void centerWindow(QWidget* window);

        public slots:
            void sliderKernelBox(int value);
            void sliderKernelMedian(int value);
            void sliderKernelGaussian(int value);
            void sliderSigmaGaussian(int value);
            void sliderKernelSobel(int value);
            void sliderDxSobel(int value);
            void sliderDySobel(int value);
            void sliderKernelLaplacian(int value);
            void infoButtonClicked();
};
#endif // FILTERWIDGET_H
