#include "computervision.h"
#include </usr/local/Cellar/opencv3/3.2.0/include/opencv2/opencv.hpp>
#include </usr/local/Cellar/opencv3/3.2.0/include/opencv2/core/core.hpp>
#include </usr/local/Cellar/opencv3/3.2.0/include/opencv2/features2d.hpp>
#include </usr/local/Cellar/opencv3/3.2.0/include/opencv2/features2d/features2d.hpp>
#include </usr/local/Cellar/opencv3/3.2.0/include/opencv2/highgui/highgui.hpp>
#include </usr/local/Cellar/opencv3/3.2.0/include/opencv2/stitching.hpp>
#include </usr/local/Cellar/opencv3/3.2.0/include/opencv2/imgcodecs.hpp>
#include </usr/local/Cellar/opencv3/3.2.0/include/opencv2/imgproc.hpp>
#include </usr/local/Cellar/opencv3/3.2.0/include/opencv2/calib3d.hpp>
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

void ComputerVision::applyORB() {
    cv::Ptr<cv::ORB> detector = cv::ORB::create();
    vector<cv::KeyPoint> keypoints;
    detector->detect(displayImage, keypoints);
    cv::drawKeypoints(displayImage, keypoints, displayImageFeature, Scalar(0,255,0));
}

void ComputerVision::applyBRISK() {
    cv::Ptr<cv::BRISK> detector = cv::BRISK::create();
    vector<cv::KeyPoint> keypoints;
    detector->detect(displayImage, keypoints);
    cv::drawKeypoints(displayImage, keypoints, displayImageFeature, Scalar(0,0,255));
}

void ComputerVision::applyFAST() {
    cv::Ptr<cv::FastFeatureDetector> detector = cv::FastFeatureDetector::create(40);
    vector<cv::KeyPoint> keypoints;
    detector->detect(displayImage, keypoints);

    cv::drawKeypoints(displayImage, keypoints, displayImageFeature, Scalar(255,0,0));
}

QPixmap ComputerVision::stitchTwoImg(QPixmap a, QPixmap b) {
    /*QImage qa = a.toImage();
    QImage qb = b.toImage();

    cv::Mat left = ComputerVision::qImageToMatS(qa);
    cv::Mat right = ComputerVision::qImageToMatS(qa);
    cv:Mat greyMatL, greyMatR;*/
    //cv::cvtColor(left, greyMatL, cv::COLOR_BGR2GRAY);
    //cv::cvtColor(right, greyMatR, cv::COLOR_BGR2GRAY);

    // Will contains the images to be stitched (only 2 static atm)
    vector<Mat> imgs;

    // reading directly from hdd, some error when converting from QPixmap to Mat.
    Mat am = imread("/Users/Simon/Documents/workspaces/QTrealOpenCV/resources/1.jpg");
    Mat bm = imread("/Users/Simon/Documents/workspaces/QTrealOpenCV/resources/2.jpg");

    imgs.push_back(am);
    imgs.push_back(bm);

    // Brisk detector detects keypoints and belonging descptors
    cv::Ptr<cv::BRISK> detector = cv::BRISK::create();
    vector<cv::KeyPoint> keypoints1;
    Mat descript1, descript2;
    detector->detectAndCompute(imgs[0], Mat(), keypoints1, descript1);
    vector<cv::KeyPoint> keypoints2;
    detector->detectAndCompute(imgs[1],Mat(), keypoints2, descript2);

    // Brisk matcher using norm_hamming (XOR-operator) distance to see the distance inbetween the descriptors
    BFMatcher bf = BFMatcher(NORM_HAMMING, true);
    vector<DMatch> matches;
    bf.match(descript1,descript2, matches);

    // Sorting the matches so that the best are first.
    Mat index;
    int nbMatch=int(matches.size());
    Mat tab(nbMatch, 1, CV_32F);

    for (int i = 0; i<nbMatch; i++) {
        tab.at<float>(i, 0) = matches[i].distance;
    }
    sortIdx(tab, index, SORT_EVERY_COLUMN + SORT_ASCENDING);
    vector<DMatch> bestMatches;

    // Pushes the 60 best matches into an array
    for(int i = 0; i<60; i++) {
        bestMatches.push_back(matches[index.at<int>(i, 0)]);
    }

    /*Mat result;
    drawMatches(am, keypoints1, bm, keypoints2, bestMatches, result);
    imshow("hej", result);*/

    // Takes the keeps the keypoints corresponging to the best matches
    vector< Point2f > left;
    vector< Point2f > right;
    for (int i = 0; i < (int)bestMatches.size(); i++) {
         left.push_back(keypoints1[ bestMatches[i].queryIdx ].pt);
         right.push_back(keypoints2[ bestMatches[i].trainIdx ].pt);
     }

    // Finds homography based on the keypoints corresponding to good enough matches
    Mat H = findHomography( right, left, CV_RANSAC );

    // Warps right img to the coordinatesystem of left img then copies them into the same coord
    cv::Mat out;
    warpPerspective(bm, out, H, cv::Size( bm.cols+am.cols, bm.rows));
    cv::Mat half(out, cv::Rect(0, 0, am.cols, am.rows));
    am.copyTo(half);
    // imshow("ds", out);

    return QPixmap::fromImage(matToQImageS(out));
}

cv::Mat ComputerVision::qImageToMatS(QImage &img) {
    QImage   swappedImg = img;
    if (img.format() == QImage::Format_RGB32) {
       swappedImg = swappedImg.convertToFormat(QImage::Format_RGB888);
    }
    swappedImg = swappedImg.rgbSwapped();
    return cv::Mat(swappedImg.height(), swappedImg.width(), CV_8UC3, const_cast<uchar*>(swappedImg.bits()), static_cast<size_t>(swappedImg.bytesPerLine())).clone();
}

QImage ComputerVision::matToQImageS(cv::Mat &imgMat) {
    QImage image(imgMat.data, imgMat.cols, imgMat.rows,static_cast<int>(imgMat.step),QImage::Format_RGB888);
    return image.rgbSwapped();
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
