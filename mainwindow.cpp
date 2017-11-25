#include "mainwindow.h"
#include "leftcolwidget.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QRectF>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QtWidgets>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <computervision.h>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    computerVision = new ComputerVision();

    widget = new QWidget;
    setCentralWidget(widget);

    applyLayout();
}

void MainWindow::applyLayout() {
    QCoreApplication::setApplicationName(QString("Some app"));
    setWindowTitle("Filters");
    statusBar = statusBar = new QLabel();
    applyLeftCol("filter");
    applyMenuBarActions();
    applyMenuBar();
}

void MainWindow::open() {
    QString fName = QFileDialog::getOpenFileName(this,tr("Open Image"), "/home", tr("Images (*.png *.xpm *.jpg)"));
    pixmapImage = QPixmap(fName);
    if (pixmapImage.width() > 799 || pixmapImage.height() > 799) {
        pixmapImage = pixmapImage.scaled(800, 800, Qt::KeepAspectRatio);
    }

    if (!pixmapImage.isNull()) {
        computerVision->setWorkingImage(pixmapImage);

        //sets working image as display image.
        computerVision->applyNoFilter();

        // This was needed to center the mainWindow after using QFileDialog
        canvas->hide();
        canvas->setPixmap(pixmapImage);
        canvas->show();
        qApp->processEvents();
        centerWindow(this);

        leftCol->enableButtons(true, true);
        saveAct->setEnabled(true);
        printAct->setEnabled(true);
    }
}

void MainWindow::saveAs() {
    // From www.qt.io docs
    const QString format = "png";
    QString initialPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    if (initialPath.isEmpty())
        initialPath = QDir::currentPath();
    initialPath += tr("/untitled.") + format;

    QFileDialog fileDialog(this, tr("Save As"), initialPath);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setDirectory(initialPath);
    QStringList mimeTypes;
    foreach (const QByteArray &bf, QImageWriter::supportedMimeTypes())
        mimeTypes.append(QLatin1String(bf));
    fileDialog.setMimeTypeFilters(mimeTypes);
    fileDialog.selectMimeTypeFilter("image/" + format);
    fileDialog.setDefaultSuffix(format);
    if (fileDialog.exec() != QDialog::Accepted)
        return;
    const QString fileName = fileDialog.selectedFiles().first();
    if (!canvas->pixmap()->save(fileName)) {
        QMessageBox::warning(this, tr("Save Error"), tr("The image could not be saved to \"%1\".")
                             .arg(QDir::toNativeSeparators(fileName)));
    }
}

void MainWindow::print() {
    canvas->setText(tr("Print, TBA"));
}

void MainWindow::filter() {
    setWindowTitle("Filters");

    if(leftCol->getActiveButton() != "") {
        activeFeatureButton = leftCol->getActiveButton();
    }
    /*if (activeFilterButton == "") {
        activeFilterButton = leftCol->getActiveButton();
    }*/
    openAct->setEnabled(true);
    pixmapImage = QPixmap::fromImage(this->computerVision->getDisplayImage());
    applyLeftCol("filter");
    leftCol->showCorrectParam(activeFilterButton);
    leftCol->setActiveButton(activeFilterButton);
    if (pixmapImage.isNull() != true) {
        this->canvas->setPixmap(pixmapImage);
        saveAct->setEnabled(true);
        printAct->setEnabled(true);
        qDebug() << activeFilterButton;
        if(activeFilterButton != "") {
            leftCol->enableButtons(true, false);
        } else {
            leftCol->enableButtons(true, true);
        }
    }
}

void MainWindow::feature() {
    setWindowTitle("Features");

    if(leftCol->getActiveButton() != "") {
        activeFilterButton = leftCol->getActiveButton();
    }
    /*if (activeFeatureButton == "") {
        activeFeatureButton = leftCol->getActiveButton();
    }*/
    pixmapImage = QPixmap::fromImage(this->computerVision->getDisplayImage());
    applyLeftCol("feature");
    leftCol->setActiveButton(activeFeatureButton);
    if (pixmapImage.isNull() != true) {
        this->canvas->setPixmap(pixmapImage);
        saveAct->setEnabled(true);
        printAct->setEnabled(true);
        qDebug() << activeFeatureButton;
        if(activeFeatureButton != "") {
            leftCol->enableButtons(true, false);
            leftCol->passiveButtonClick(activeFeatureButton);
        } else {
            leftCol->enableButtons(true, true);
        }
    }
    openAct->setEnabled(true);
}

void MainWindow::applyMenuBarActions() {
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAct = new QAction(tr("&Save as"), this);
    saveAct->setShortcuts(QKeySequence::SaveAs);
    saveAct->setStatusTip(tr("Save the image to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveAs);

    printAct = new QAction(tr("&Print..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the document"));
    connect(printAct, &QAction::triggered, this, &MainWindow::print);

    filterAct = new QAction(tr("&Filters"), this);
    filterAct->setStatusTip(tr("View the filtering tools"));
    connect(filterAct, &QAction::triggered, this, &MainWindow::filter);

    featureAct = new QAction(tr("&Feature detection"), this);
    featureAct->setStatusTip(tr("View the feature detection tools"));
    connect(featureAct, &QAction::triggered, this, &MainWindow::feature);

    saveAct->setEnabled(false);
    printAct->setEnabled(false);
}

void MainWindow::applyMenuBar() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(printAct);

    fileMenu = menuBar()->addMenu(tr("&Tools"));
    fileMenu->addAction(filterAct);
    fileMenu->addAction(featureAct);
}

void MainWindow::applyLeftCol(std::string mode) {
    qDeleteAll(widget->children());
    QVBoxLayout* outer = new QVBoxLayout;
    statusBar = new QLabel();

    QWidget* filterAndDisp = new QWidget;
    if (mode == "filter") {
        canvas = new QLabel(tr("<i>Choose an image by clicking: File > Open </i><br><i>Image of width or height greater than 800px will be rescaled</i>"));
    } else if (mode == "feature") {
        canvas = new QLabel(tr("Feature section selected, TBA"));
    }
    canvas->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    canvas->setAlignment(Qt::AlignCenter);
    canvas->setStyleSheet("border: 0px;");

    if (mode == "filter") {
        leftCol = new LeftColWidget(this, canvas, this->computerVision, statusBar, "filter");
    } else if (mode == "feature") {
        leftCol = new LeftColWidget(this, canvas, this->computerVision, statusBar, "feature");
    }
    leftCol->setMinimumWidth(200);
    leftCol->setMaximumWidth(200);
    QGroupBox* canvasGroup = new QGroupBox();
    QVBoxLayout *layoutOuterCan = new QVBoxLayout;
    layoutOuterCan->addStretch();
    layoutOuterCan->addWidget(canvas);
    layoutOuterCan->addStretch();
    canvasGroup->setLayout(layoutOuterCan);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(5);
    layout->addWidget(leftCol);
    layout->addWidget(canvasGroup);

    filterAndDisp->setLayout(layout);

    outer->addWidget(filterAndDisp);
    outer->addWidget(statusBar);
    widget->setLayout(outer);
}

void MainWindow::centerWindow(QWidget* window) {
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
