#ifndef ADD_RECIPE_H
#define ADD_RECIPE_H

#include <QDialog>
#include <QtCore>
#include <QtGui>

namespace Ui {
class Add_Recipe;
}

class Add_Recipe : public QDialog
{
    Q_OBJECT

public:
    explicit Add_Recipe(QWidget *parent = nullptr);
    ~Add_Recipe();

    QList<QJsonObject> Names;
    QStringList Recipes;

    bool add;
    int recipe_count;
    int ID_edit;

    QJsonObject newObject;
    QString newObjectName;

    void save();
    void reloadIngredients();
    void deleteProduct(int row_index);
    void load_data();

signals:
    void Add_RecipeCloses();

private slots:

    void on_delete_ingredient_btn_clicked();

    void on_add_ingredient_btn_clicked();

    void on_save_btn_clicked();

    void on_cancel_btn_clicked();

private:
    Ui::Add_Recipe *ui;
    QStandardItemModel *model;
};

#endif // ADD_RECIPE_H
