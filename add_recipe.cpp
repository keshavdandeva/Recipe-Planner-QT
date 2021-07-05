#include "add_recipe.h"
#include "ui_add_recipe.h"
#include "globalvariables.h"
#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QMessageBox>

Add_Recipe::Add_Recipe(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_Recipe){
    ui->setupUi(this);
}

Add_Recipe::~Add_Recipe(){
    delete ui;
}

void Add_Recipe::save() {

    //creates file if it doesn't exist
    QFile file("C:/Users/Keshav Dandeva/Desktop/kod/EGUI Lab 1/Recipes.js");

    QJsonObject product;

    //loop for every product
    for(int i = 0; i < this->Recipes.size() ; i++)
        product.insert(Recipes[i], Names[i]);

    QJsonDocument document(product);

    //checks if file is open
    if(!file.open(QIODevice::WriteOnly) ){
        qDebug("Error writing the file!");
        return;
    }

    file.write(document.toJson());
    file.close();
}


void Add_Recipe::deleteProduct(int row_index) {

    QString recipe_name = this->model->takeItem(row_index,0)->text();

    this->newObject.remove(recipe_name);
    this->recipe_count--;
    this->reloadIngredients();
}

void Add_Recipe::on_delete_ingredient_btn_clicked(){

    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();

    QModelIndexList selectedRows = selectionModel->selectedRows();

    //if no row is selected
    if(selectedRows.size() == 0) {

        QMessageBox::critical(this,"Error","Please select the row you want to delete");
        return;
    }

    QModelIndex selectedRow = selectedRows.takeLast();
    this->deleteProduct(selectedRow.row());
}

void Add_Recipe::on_add_ingredient_btn_clicked(){

    //fetching data from the user
    QString ingredient_name = this->ui->lineEdit_2->text();
    QString ingredient_quantity = this->ui->lineEdit_3->text();
    QString ingredient_unit = this->ui->lineEdit_4->text();

    //empty field conditions
    if(ingredient_name.trimmed() == "") {
        QMessageBox::critical(this,"Error","Ingredient name cannot be empty");
        return;
    }
    if(ingredient_quantity.trimmed() == "") {
        QMessageBox::critical(this,"Error","Ingredient quantity cannot be empty");
        return;
    }

    if(ingredient_unit.trimmed() == "") {
        QMessageBox::critical(this,"Error","Ingredient unit cannot be empty");
        return;
    }

    //condition to check if quantity is numerical value
    bool isNum;
    ingredient_quantity.toDouble(&isNum);
    if(!isNum) {
        QMessageBox::critical(this,"Error","Ingredient quantity should be a number!");
        return;
    }

    //inserting the data
    this->newObject.insert(ingredient_name,ingredient_quantity + " " + ingredient_unit);
    this->recipe_count++;
    this->reloadIngredients();

    //clearing the fields
    this->ui->lineEdit_2->setText("");
    this->ui->lineEdit_3->setText("");
    this->ui->lineEdit_4->setText("");

}

void Add_Recipe::on_save_btn_clicked(){

    QStringList new_recipe;

    //fetching name of the recipe
    this->newObjectName = ui->lineEdit->text();

    //fetching recipe description
    new_recipe = ui->textEdit->toPlainText().split('\n');

    QJsonArray new_recipeArray;
    QJsonValue value;

    //saving it in json
    for(int i = 0; i < new_recipe.count(); i++) {

        value = new_recipe.at(i);
        new_recipeArray.push_back(value);
    }

    //insert recipe into newObject
    this->newObject.insert("recipe",new_recipeArray);

    //checking if adding or editing
    if(this->add){

        this->Names.push_back(this->newObject);
        this->Recipes.push_back(this->newObjectName);

        this->save();
        this->hide();

        emit Add_RecipeCloses();

    } else {

        //remove object at given id
        this->Names.removeAt(this->ID_edit);
        this->Recipes.removeAt(this->ID_edit);

        //insert object at given id
        this->Names.insert(this->ID_edit,this->newObject);
        this->Recipes.insert(this->ID_edit,this->newObjectName);

        this->save();
        this->hide();

        emit Add_RecipeCloses();
    }
}

void Add_Recipe::reloadIngredients() {

    model = new QStandardItemModel(this->recipe_count,2,this);

    //fetching keys in object
    QStringList keyStrings = this->newObject.keys();

    bool recipe_desc = false;

    //traversing through the data
    for(int row = 0; row < recipe_count; ++row) {
        for(int col = 0; col < 2; ++col) {

            QModelIndex index =  model->index(row,col, QModelIndex());

            if(keyStrings[row] == "recipe" || recipe_desc == true){
                ++row;
                recipe_desc = true;
            }
            if(col==0) { //name of ingredient
                if(keyStrings[row] != "recipe") {
                    model->setData(index,keyStrings[row]);
                }
            } else { //quantity and unit
                if(keyStrings[row] != "recipe") {
                    //quantity and unit string
                    QString quantity_unit = this->newObject.value(keyStrings[row]).toString();
                    model->setData(index, quantity_unit);
                }
            }
            if(recipe_desc){
                --row;
            }
        }
    }

    model->setHeaderData(0,Qt::Horizontal,tr("Ingredient"));
    model->setHeaderData(1,Qt::Horizontal,tr("Quantity and unit"));

    this->ui->tableView->setModel(model);

    this->ui->tableView->setColumnWidth(0,300);
    this->ui->tableView->setColumnWidth(1,300);
}

void Add_Recipe::on_cancel_btn_clicked(){

    this->hide();
    emit Add_RecipeCloses();
}

void Add_Recipe::load_data() {

    this->ui->lineEdit->setText(this->newObjectName);

    QJsonArray recipeLines = this->newObject.value("recipe").toArray();
    QString string_recipeLines = "";

    //loop to go through ingredients
    for(const auto obj : recipeLines) {
        string_recipeLines += obj.toString() + "\n";
    }

    //displaying ingredients
    this->ui->textEdit->setPlainText(string_recipeLines);
}
