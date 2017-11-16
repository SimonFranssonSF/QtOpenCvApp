#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include "leftcolwidget.h"
#include <computervision.h>
#include <QWidget>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = 0);
private:
    Ui::MainWindow* ui;
    LeftColWidget* leftCol;
    QLabel* canvas;

    QLabel* statusBar;

    QWidget* widget;

    void applyLayout();
    void applyMenuBarActions();
    void applyMenuBar();
    QMenu* fileMenu;

    void open();
    void saveAs();
    void print();
    void filter();
    void feature();
    QAction* openAct;
    QAction* saveAct;
    QAction* printAct;
    QAction* filterAct;
    QAction* featureAct;

    QPixmap pixmapImage = QPixmap();

    void applyLeftCol(std::string mode);

    void centerWindow(QWidget* window);

    ComputerVision* computerVision;
};

#endif // MAINWINDOW_H
