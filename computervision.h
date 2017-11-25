#ifndef COMPUTERVISION_H
#define COMPUTERVISION_H
#include <QPixmap>
#include </usr/local/Cellar/opencv3/3.2.0/include/opencv2/opencv.hpp>
#include <QImage>
#include <computervision.h>

class ComputerVision {
public:
    ComputerVision();
    QImage getDisplayImage();
    QImage getDisplayImageFeatures();

    int getBoxKernel();
    int getMedianKernel();
    int getGaussianKernel();
    double getGaussianSigma();
    int getSobelKernel();
    int getSobelDx();
    int getSobelDy();
    int getLaplacianKernel();

    void setWorkingImage(QPixmap pixmap);

    void applyNoFilter();
    void applyBoxFilter(int kernelSize = 3);
    void applyMedianFilter(int kernelSize = 3);
    void applyGaussianFilter(int kernelSize = 3, double sigma = 0.0);
    void applySobelFilter(int kernelSize = 3, int dx = 1, int dy = 1);
    void applyLaplacianFilter(int kernelSize = 3);

    void applyNoDetector();
    void applySIFT();
    void applySURF();
    void applyFAST();

    cv::Mat workingImage;
    cv::Mat displayImage;
    cv::Mat displayImageFeature;

    int sliderBoxVal = 3;
    int sliderMedianVal = 3;
    int sliderGaussianVal = 3;
    double sliderSigmaVal = 0;
    int sliderSobelVal = 1;
    int sliderDxVal = 1;
    int sliderDyVal = 1;
    int sliderLaplacianVal = 3;

private:
    cv::Mat qImageToMat(QImage &img);
    QImage matToQImage(cv::Mat &mat);
    QImage matToQImageGrayScale(cv::Mat &imgMat);

    int boxKernelSize = 3;

    int medianKernelSize = 3;

    int gaussianKernelSize = 3;
    double gaussianSigma = 0;

    int sobelKernelSize = 3;
    int sobelDx = 1;
    int sobelDy = 1;

    int laplacianKernelSize = 3;
};

#endif // COMPUTERVISION_H
