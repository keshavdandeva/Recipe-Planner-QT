#include "recipe_planner.h"

#include <QApplication>
#include <QDialog>
#include <QtCore>
#include <QtGui>

int main(int argc, char *argv[]){

    QApplication a(argc, argv);
    Recipe_Planner w;
    w.show();
    return a.exec();
}
