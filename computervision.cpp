#include "computervision.h"
#include </usr/local/Cellar/opencv3/3.2.0/include/opencv2/opencv.hpp>
#include </usr/local/Cellar/opencv3/3.2.0/include/opencv2/core/core.hpp>
#include </usr/local/Cellar/opencv3/3.2.0/include/opencv2/features2d.hpp>
#include </usr/local/Cellar/opencv3/3.2.0/include/opencv2/features2d/features2d.hpp>
#include </usr/local/Cellar/opencv3/3.2.0/include/opencv2/highgui/highgui.hpp>
#include <math.h>
#include <iostream>
#include <QtDebug>
#include <QTextStream>

using namespace cv;
using namespace std;

ComputerVision::ComputerVision() {

}

void ComputerVision::setWorkingImage(QPixmap pixmap) {
    QImage qImage = pixmap.toImage();
    workingImage = qImageToMat(qImage);
}

cv::Mat ComputerVision::qImageToMat(QImage &img) {
    QImage   swappedImg = img;
    if (img.format() == QImage::Format_RGB32) {
       swappedImg = swappedImg.convertToFormat(QImage::Format_RGB888);
    }
    swappedImg = swappedImg.rgbSwapped();
    return cv::Mat(swappedImg.height(), swappedImg.width(), CV_8UC3, const_cast<uchar*>(swappedImg.bits()), static_cast<size_t>(swappedImg.bytesPerLine())).clone();
}

QImage ComputerVision::matToQImage(cv::Mat &imgMat) {
    QImage image(imgMat.data, imgMat.cols, imgMat.rows,static_cast<int>(imgMat.step),QImage::Format_RGB888);
    return image.rgbSwapped();
}

QImage ComputerVision::matToQImageGrayScale(cv::Mat &imgMat) {
    QImage image(imgMat.data, imgMat.cols, imgMat.rows,static_cast<int>(imgMat.step),QImage::Format_Grayscale8);
    return image;
}

void ComputerVision::applyNoFilter() {
    this->displayImage = this->workingImage.clone();
}

void ComputerVision::applyBoxFilter(int kernelSize) {
    if (this->workingImage.empty()) {
        throw std::invalid_argument("Error: No image open yet.");
    }
    boxKernelSize = kernelSize;
    cv::Size* kSize = new cv::Size(boxKernelSize, boxKernelSize);
    cv::Point anchorPoint = cv::Point(-1, -1);

    // use -1 for deppth to use same depth as source img.
    // boxFilter(src, dst, ddepth, ksize, anchor, normalize, borderType)
    cv::boxFilter(this->workingImage, this->displayImage, -1, *kSize, anchorPoint, true);
    delete kSize;
}

void ComputerVision::applyMedianFilter(int kernelSize) {
    // kernel size needs to be odd.
    // void medianBlur(InputArray src, OutputArray dst, int ksize)
    medianKernelSize = kernelSize;
    cv::medianBlur(this->workingImage, this->displayImage, medianKernelSize);
}

void ComputerVision::applyGaussianFilter(int kernelSize, double sigma) {
    if (kernelSize != -1) {
        gaussianKernelSize = kernelSize;
    }

    if (sigma != -1) {
        gaussianSigma = sigma;
    }
    // Size of kernel has to be odd
    // GaussianBlur(InputArray src, OutputArray dst, Size ksize, double sigmaX, double sigmaY=0, int borderType=BORDER_DEFAULT )
    cv::Size* kSize = new cv::Size(gaussianKernelSize, gaussianKernelSize);
    cv::GaussianBlur(this->workingImage, this->displayImage, *kSize, gaussianSigma, gaussianSigma);
    delete kSize;
}

void ComputerVision::applySobelFilter(int kernelSize, int dx, int dy) {
    // Sobel(InputArray src, OutputArray dst, int ddepth, int dx, int dy, int ksize=3, double scale=1, double delta=0, int borderType=BORDER_DEFAULT )
    // kernelsize must be 1, 3, 5 or 7
    int scale = 1;
    int delta = 0;

    if (kernelSize != -1) {
        sobelKernelSize = kernelSize;
    }

    if (dx >= 0) {
        sobelDx = dx;
    }

    if (dy >= 0) {
        sobelDy = dy;
    }

    if(sobelKernelSize > sobelDx && sobelKernelSize > sobelDy && (sobelDx + sobelDy) > 0) {
        Mat src, srcGray;

        src = this->workingImage.clone();
        GaussianBlur(src, src, Size(3,3), 0, 0, BORDER_DEFAULT);

        cvtColor(src, srcGray, CV_BGR2GRAY);

        cv::Mat gradX, gradY;
        Mat absGradX, absGradY;

        cv::Sobel(srcGray, gradX, -1, sobelDx, 0, sobelKernelSize, scale, delta);
        cv::Sobel(srcGray, gradY, -1, 0, sobelDy, sobelKernelSize, scale, delta);

        cv::convertScaleAbs(gradX, absGradX);
        cv::convertScaleAbs(gradY, absGradY);

        cv::addWeighted(absGradX, 0.5, absGradY, 0.5, 0, this->displayImage);
    } else if (sobelKernelSize <= sobelDx || sobelKernelSize <= sobelDy) {
        throw std::invalid_argument("Error: Kernel size has to be greater than value of Dx & Dy.");
    } else if ((sobelDx + sobelDy) <= 0) {
        throw std::invalid_argument("Error: (Dx + Dy) needs to be greater than 0.");
    }
}

void ComputerVision::applyLaplacianFilter(int kernelSize) {
    // Laplacian(InputArray src, OutputArray dst, int ddepth, int ksize=1, double scale=1, double delta=0, int borderType=BORDER_DEFAULT )
    // KernelSize must be odd
    if (kernelSize != -1) {
        laplacianKernelSize = kernelSize;
    }

    double scale = 1.0;

    Mat src, srcGray, out, absOut;
    src = this->workingImage.clone();

    /// Remove noise by blurring with a Gaussian filter
    GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );
    cvtColor( src, srcGray, CV_RGB2GRAY );

    Laplacian(srcGray, out, CV_16S, 3, 1, 0, BORDER_DEFAULT);
    cv::Laplacian(srcGray, this->displayImage, -1, laplacianKernelSize, scale, 0);
    convertScaleAbs(this->displayImage, this->displayImage);
}

void ComputerVision::applyNoDetector() {
    this->displayImageFeature = this->displayImage.clone();
}

void ComputerVision::applySIFT() {
    /*cv:: detector;
    vector<cv::KeyPoint> keypoints;
    detector.detect(displayImage, keypoints);

    cv::drawKeypoints(displayImage, keypoints, displayImage);*/

}

void ComputerVision::applySURF() {

}

void ComputerVision::applyFAST() {
    cv::Ptr<cv::FastFeatureDetector> detector = cv::FastFeatureDetector::create(40);
    vector<cv::KeyPoint> keypoints;
    detector->detect(displayImage, keypoints);

    cv::drawKeypoints(displayImage, keypoints, displayImageFeature);
}

QImage ComputerVision::getDisplayImage() {
   QImage qImage;
   if (this->displayImage.type() == 16) {
       qImage = matToQImage(this->displayImage);
   } else if (this->displayImage.type() == 0) {
       qImage = matToQImageGrayScale(this->displayImage);
   }
    return qImage;
}

int ComputerVision::getBoxKernel() {
    return boxKernelSize;
}

int ComputerVision::getMedianKernel() {
    return medianKernelSize;
}

int ComputerVision::getGaussianKernel() {
    return gaussianKernelSize;
}

double ComputerVision::getGaussianSigma() {
    return gaussianSigma;
}

int ComputerVision::getSobelKernel() {
    return sobelKernelSize;
}

int ComputerVision::getSobelDx() {
    return sobelDx;
}

int ComputerVision::getSobelDy() {
    return sobelDy;
}

int ComputerVision::getLaplacianKernel() {
    return laplacianKernelSize;
}

QImage ComputerVision::getDisplayImageFeatures() {
    QImage qImage;
    if (this->displayImageFeature.type() == 16) {
        qImage = matToQImage(this->displayImageFeature);
    } else if (this->displayImageFeature.type() == 0) {
        qImage = matToQImageGrayScale(this->displayImageFeature);
    }
     return qImage;
}
