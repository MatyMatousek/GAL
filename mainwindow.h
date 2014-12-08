#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graphscene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setButtonUnchecked();


private slots:
    void on_actionNewNode_triggered();
    void on_actionNewEdge_triggered();
    void on_actionCursor_triggered();
    void on_actionDelete_triggered();
    void on_actionO_programu_triggered();

    void on_actionStart_triggered();

    void on_actionCil_triggered();

    void on_actionNextstep();
    void on_actionResetsteps();
    void on_actionMakeAllSteps();

private:
    Ui::MainWindow *ui;

    GraphScene *scene;
    QGraphicsView *view;
};

#endif // MAINWINDOW_H
