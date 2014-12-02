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

private slots:
    void nodeInserted(Node *item);

private:
    Ui::MainWindow *ui;

    GraphScene *scene;
    QGraphicsView *view;
};

#endif // MAINWINDOW_H
