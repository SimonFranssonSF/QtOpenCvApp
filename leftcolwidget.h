#ifndef LEFTCOLWIDGET_H
#define LEFTCOLWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QSlider>
#include <QWidget>
#include <filterbuttons.h>
#include <computervision.h>
#include <featurebuttons.h>

class FilterButtons;
class FeatureButtons;

class LeftColWidget : public QWidget {
    Q_OBJECT
    public:
        LeftColWidget(QWidget *parent = 0, QLabel* canvas = 0, ComputerVision* computerVision = 0, QLabel* statusBar = 0, std::string mode = "filter");
        ComputerVision* computerVision;
        QLabel* canvas;
        QLabel* statusBar;
        void showCorrectParam(QString activeFilter);
        void enableButtons(bool enable, bool nofilter);
        QString getActiveButton();
        void setActiveButton(QString buttonName);
        std::string currentContent;
        void passiveButtonClick(QString type);
    private:
        std::string mode;

        FactoryGroupBox* groupBoxButtons;

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
#endif // LEFTCOLWIDGET_H
