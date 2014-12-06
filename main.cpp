#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
/**
 *
 * TODO:
 *
 * start, cil uzel
 * nazev uzlu, ohodnoceni hran, kontrola zpetne hrany
 * krokovani, prehrani, reset
 * algoritmus
 * clear
 * postrani seznam s kroky alg.
 * zoom platna
 * ulozeni, nacteni
 *
*/
