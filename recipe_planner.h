#ifndef RECIPE_PLANNER_H
#define RECIPE_PLANNER_H

#include <QWidget>
#include <QFile>
#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Recipe_Planner; }
QT_END_NAMESPACE

class Recipe_Planner : public QWidget{

    Q_OBJECT

public:
    Recipe_Planner(QWidget *parent = nullptr);
    ~Recipe_Planner();

    QList<QJsonObject> Names;
    QStringList Recipes;

private slots:

    void on_btnAdd_clicked();

    void on_btn_edit_clicked();

    void on_btn_delete_clicked();

    void on_ing_calc_btn_clicked();

public slots:
    void reloadMainWindow();

private:
    Ui::Recipe_Planner *ui;
    QStandardItemModel *model;

    bool read_file();
    void display_data(QList<QJsonObject> a, QStringList b);
    void delete_object(int row_index);

};
#endif // RECIPE_PLANNER_H
