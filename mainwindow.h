#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include "filterwidget.h"
#include <computervision.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = 0);
private:
    Ui::MainWindow* ui;
    FilterWidget* leftCol;
    QLabel* canvas;

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

    QPixmap pixmapImage;

    void applyFilterWidget();

    void centerWindow(QWidget* window);

    ComputerVision* computerVision;
};

#endif // MAINWINDOW_H
