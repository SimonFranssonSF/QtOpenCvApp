#include "filterinfo.h"
#include <fstream>
#include <QDebug>
#include <QFile>
#include <QTextBrowser>
#include <QDesktopWidget>

FilterInfo::FilterInfo()
{
    this->setMaximumSize(QSize(1000, 1000));
    this->setMinimumSize(QSize(400, 500));
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
}

void FilterInfo::setContent(std::string filter) {
    QString filterType;
    if (filter == "Box filter") {
        filterType = "boxFilter.html";
    } else if (filter == "Median filter") {
        filterType = "medianFilter.html";
    } else if (filter == "Gaussian filter") {
        filterType = "gaussianFilter.html";
    } else if (filter == "Sobel filter") {
        filterType = "sobelFilter.html";
    } else if (filter == "Laplacian filter") {
        filterType = "laplacianFilter.html";
    }
    readHtml(filterType);
}

void FilterInfo::readHtml(QString filter) {
    QString url = ":/filter/info/resources/" + filter;
    QFile file(url);
    QString html;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull()) {
        html += line;
        line = in.readLine();
    }

    this->setOpenExternalLinks(true);
    this->setHtml(html);
}
