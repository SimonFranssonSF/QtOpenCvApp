#include "info.h"
#include <fstream>
#include <QDebug>
#include <QFile>
#include <QTextBrowser>
#include <QDesktopWidget>

Info::Info()
{
    this->setMaximumSize(QSize(1000, 1000));
    this->setMinimumSize(QSize(400, 500));
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
}

void Info::setContent(std::string content) {
    QString contentType;
    if (content == "Box filter") {
        contentType = "boxFilter.html";
    } else if (content == "Median filter") {
        contentType = "medianFilter.html";
    } else if (content == "Gaussian filter") {
        contentType = "gaussianFilter.html";
    } else if (content == "Sobel filter") {
        contentType = "sobelFilter.html";
    } else if (content == "Laplacian filter") {
        contentType = "laplacianFilter.html";
    } else if (content == "SIFT") {
        contentType = "SIFT.html";
    } else if (content == "SURF") {
        contentType = "SURF.html";
    } else if (content == "FAST") {
        contentType = "FAST.html";
    }
    readHtml(contentType);
}

void Info::readHtml(QString content) {
    QString url = ":/filter/info/resources/" + content;
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
