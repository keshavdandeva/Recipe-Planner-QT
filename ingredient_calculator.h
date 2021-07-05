#ifndef INGREDIENT_CALCULATOR_H
#define INGREDIENT_CALCULATOR_H

#include <QDialog>
#include <QWidget>
#include <QFile>
#include <QMainWindow>
#include <QtCore>
#include <QtGui>


namespace Ui {
class Ingredient_Calculator;
}

class Ingredient_Calculator : public QDialog
{
    Q_OBJECT

public:
    explicit Ingredient_Calculator(QWidget *parent = nullptr);
    ~Ingredient_Calculator();


private slots:
    void on_pushButton_clicked();

private:
    Ui::Ingredient_Calculator *ui;
};

#endif // INGREDIENT_CALCULATOR_H
