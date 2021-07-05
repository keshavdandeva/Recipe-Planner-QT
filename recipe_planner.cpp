#include "recipe_planner.h"
#include "ui_recipe_planner.h"
#include "add_recipe.h"
#include "ingredient_calculator.h"
#include "globalvariables.h"
#include <QFile>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>
#include <QDialog>
#include <QtCore>
#include <QtGui>


Recipe_Planner::Recipe_Planner(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Recipe_Planner){

    ui->setupUi(this);
    read_file();
    display_data(this->Names, this->Recipes);

}

Recipe_Planner::~Recipe_Planner(){

    delete ui;
}

bool Recipe_Planner::read_file(){

    QFile file("C:/Users/Keshav Dandeva/Desktop/kod/EGUI Lab 1/Recipes.js");

    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QMessageBox::critical(this, "Error 404", "File not found!");
        return false;
    }


    //Close the file after reading the data
    const QByteArray raw_data=file.readAll();
    file.close();

    //Parse to Json document
    QJsonParseError json_error;
    QJsonDocument json_doc=QJsonDocument::fromJson(raw_data,&json_error);

    //global variable to use in ingredient calculator
    globalptr = json_doc.object();

    //Whether the Json data is parsed normally
    if(json_doc.isNull()||json_doc.isEmpty()||json_error.error!=QJsonParseError::NoError)
        return false;

    //fetching the object from json file
    if(json_doc.isObject()) {

        QJsonObject json_obj = json_doc.object();
        QJsonObject::iterator iter;

        QList<QJsonObject> temp_value;
        QStringList temp_key;

        //loop to store data from object
        for(iter = json_obj.begin(); iter != json_obj.end(); ++iter) {

            temp_key.push_back(iter.key());
            QJsonObject temp_obj = iter.value().toObject();
            temp_value.push_back(temp_obj);

        }

        //storing the data for further use
        this->Names = temp_value;
        this->Recipes = temp_key;

    }
    return true;
}

void Recipe_Planner::display_data(QList<QJsonObject> a, QStringList b){

    model = new QStandardItemModel(b.length(),3,this);

    //loop to populate the table
    for(int row = 0; row < b.length(); ++row) {
        for(int column = 0; column < 3; ++column) {

            QModelIndex index =  model->index(row,column, QModelIndex());

            //taking the object from list
            QJsonObject obj = a[row];

            //filling the 3 columns
            if(column==0){

                model->setData(index,b[row]);

            }else if(column==1){

                QJsonArray description = obj.value("recipe").toArray();
                QString recipe_lines = "";

                for(const auto obj : description) {
                    recipe_lines += obj.toString() + "\n";
                }

                model->setData(index,recipe_lines);

            } else {

                QStringList ingredient_list = obj.keys();
                QString ingredient_string = "";

                for(int i=0; i < ingredient_list.length(); i++) {
                    if(ingredient_list[i] != "recipe") {
                        ingredient_string = ingredient_string + ingredient_list[i] + " " + obj.value(ingredient_list[i]).toString() + "\n";
                    }
                }

                model->setData(index,ingredient_string);

            }
        }
    }

    model->setHeaderData(0,Qt::Horizontal,tr("Name"));
    model->setHeaderData(1,Qt::Horizontal,tr("Recipe"));
    model->setHeaderData(2,Qt::Horizontal,tr("Ingredients"));

    ui->tableView->setModel(model);

    //set row and column measurements
    QHeaderView* header=ui->tableView->verticalHeader();
    header->setDefaultSectionSize(250);

    ui->tableView->setColumnWidth(0,100);
    ui->tableView->setColumnWidth(1,400);
    ui->tableView->setColumnWidth(2,300);

}

void Recipe_Planner::on_btnAdd_clicked(){

    Add_Recipe* dlg = new Add_Recipe(this);
    dlg->setWindowTitle("Add New Recipe");
    dlg->show();

    dlg->Names = this->Names;
    dlg->Recipes = this->Recipes;

    //condition to know add or edit
    dlg->add = true;

    //initialising the recipe count
    dlg->recipe_count = 0;

    connect(dlg, SIGNAL(Add_RecipeCloses()), this, SLOT(reloadMainWindow()));
}

void Recipe_Planner::reloadMainWindow(){

    read_file();
    display_data(this->Names, this->Recipes);
}

void Recipe_Planner::on_btn_edit_clicked(){

    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();

    //if no row is selected
    if(selectedRows.size() == 0) {

        QMessageBox::critical(this,"Error","Please select the row you want to edit");
        return;
    }

    QModelIndex selectedRow = selectedRows.takeLast();


    Add_Recipe *dlg2 = new Add_Recipe(this);
    dlg2->setWindowTitle("Edit Recipe");
    dlg2->show();

    dlg2->Names = this->Names;
    dlg2->Recipes = this->Recipes;

    //condition to check add/edit
    dlg2->add = false;

    connect(dlg2, SIGNAL(Add_RecipeCloses()), this, SLOT(reloadMainWindow()));

    //storing id of row to edit
    dlg2->ID_edit = selectedRow.row();

    //storing data of the highlighted row
    dlg2->newObject = this->Names.takeAt(selectedRow.row());
    dlg2->newObjectName = this->Recipes.takeAt(selectedRow.row());
    dlg2->recipe_count = dlg2->newObject.keys().size()-1;

    //filling it in description
    dlg2->load_data();
    dlg2->reloadIngredients();
}

void Recipe_Planner::delete_object(int row_index) {

    //removing the object
    this->Names.removeAt(row_index);
    this->Recipes.removeAt(row_index);

    //creating new object
    Add_Recipe *new_object = new Add_Recipe(this);

    new_object->Names = this->Names;
    new_object->Recipes = this->Recipes;

    //saving to file
    new_object->save();

    //refreshing the table
    display_data(this->Names,this->Recipes);
}

void Recipe_Planner::on_btn_delete_clicked(){


    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();

    //if no row is selected
    if(selectedRows.size() == 0) {

        QMessageBox::critical(this,"Error","Please select the row you want to delete");
        return;
    }

    //Deleting the selected row
    QModelIndex selectedRow = selectedRows.takeLast();
    delete_object(selectedRow.row());

}


void Recipe_Planner::on_ing_calc_btn_clicked(){

    Ingredient_Calculator* dlg3 = new Ingredient_Calculator(this);
    dlg3->setWindowTitle("Ingredient Calculator");
    dlg3->show();

}
