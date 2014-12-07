#include "edge.h"
#include "node.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new GraphScene(this);
    scene->setSceneRect(QRectF(0, 0, 800, 400));
    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setButtonUnchecked()
{
    ui->actionCursor->setChecked(false);
    ui->actionDelete->setChecked(false);
    ui->actionNewEdge->setChecked(false);
    ui->actionNewNode->setChecked(false);
}

void MainWindow::on_actionNewNode_triggered()
{
    setButtonUnchecked();
    ui->actionNewNode->setChecked(true);
    scene->setCursorMode(GraphScene::InsertNode);
    QCursor myCursor(QPixmap(":images/node.png"));
    ui->graphicsView->setCursor(myCursor);
}

void MainWindow::on_actionNewEdge_triggered()
{
    setButtonUnchecked();
    ui->actionNewEdge->setChecked(true);
    scene->setCursorMode(GraphScene::InsertEdge);
    QCursor myCursor(QPixmap(":images/linepointer.png"));
    ui->graphicsView->setCursor(myCursor);
}

void MainWindow::on_actionCursor_triggered()
{
    setButtonUnchecked();
    ui->actionCursor->setChecked(true);
    scene->setCursorMode(GraphScene::MoveItem);
    ui->graphicsView->setCursor(Qt::ArrowCursor);
}

void MainWindow::on_actionDelete_triggered()
{
    setButtonUnchecked();
    ui->actionDelete->setChecked(true);
    scene->setCursorMode(GraphScene::DeleteItem);
    QCursor myCursor(QPixmap(":images/delete.png"));
    ui->graphicsView->setCursor(myCursor);
}

void MainWindow::on_actionO_programu_triggered()
{
    QMessageBox::about(this, tr("About Dinizt's graph"),
                       tr("lalalalalala"));
}

void MainWindow::on_actionStart_triggered()
{
    scene->setStart();
}

void MainWindow::on_actionCil_triggered()
{
    scene->setEnd();
}
