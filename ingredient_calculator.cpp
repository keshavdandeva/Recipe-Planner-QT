#include "ingredient_calculator.h"
#include "ui_ingredient_calculator.h"
#include "recipe_planner.h"
#include "add_recipe.h"
#include "globalvariables.h"
#include <string>
#include <QDebug>
#include <regex>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QMultiMap>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

Ingredient_Calculator::Ingredient_Calculator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ingredient_Calculator){
    ui->setupUi(this);
}

Ingredient_Calculator::~Ingredient_Calculator(){
    delete ui;
}

void Ingredient_Calculator::on_pushButton_clicked(){

    //fetching the names of the recipes and sorting them
    QString recipe_names = ui->plainTextEdit->toPlainText();
    QStringList recipe_list = recipe_names.split("\n");
    recipe_list.sort();

    //Using QMap to get recipe(s) count
    QMap<QString,int> qmap_recipe;
    for (int i = 0; i < recipe_list.count(); i++){
        int count_val = recipe_list.count(recipe_list[i]);
        qmap_recipe.insert(recipe_list[i],count_val);
    }

    //removing duplicates to fetch ingredients
    recipe_list.removeDuplicates();

    //Using QMap to get ingredients of the recipe(s)
    QMap<QString,float> qmap_ingredients;
    for (int i = 0; i < recipe_list.count(); i++ ){
        QJsonObject temp_ingredients = globalptr[recipe_list[i]].toObject();
        QStringList temp_ingredientsList = temp_ingredients.keys();

        int rep_count = qmap_recipe.value(recipe_list[i]); //stores the number of times a recipe is repeated
        int ingredient_count = temp_ingredientsList.count(); //stores the number of ingredients in recipe

        for(int i = 0; i < ingredient_count; i++){
            if(temp_ingredientsList[i]!="recipe"){

                //cleaning and storing the ingredients
                QStringList newStringList = temp_ingredients[temp_ingredientsList[i]].toString().split(" ");
                newStringList.removeAll("");
                QString newString = temp_ingredientsList[i]+"_"+newStringList[1];

                //storing the quantity of ingredients
                float quantity = newStringList[0].toFloat();
                quantity = quantity * rep_count;

                //inserting new quantity into qmap_ingredients
                if(qmap_ingredients.contains(newString)){
                    float newquantity = qmap_ingredients.value(newString)+quantity;
                    qmap_ingredients.insert(newString,newquantity);
                }else{
                    qmap_ingredients.insert(newString,quantity);
                }
            }
        }
    }

    QString final_ingredients;
    QStringList finalList = qmap_ingredients.keys();

    //traversing through qmap and getting final ingredients list
    for(int i = 0; i < finalList.count(); i++){
        QStringList temp_list = finalList[i].split("_");
        QString temp_string = QString::number(qmap_ingredients.value(finalList[i]));

        //if condition used to present ingredients in separate lines
        if(i==finalList.count()-1){
            final_ingredients += temp_list[0]+": "+temp_string+" "+temp_list[1];
        }else{
            final_ingredients += temp_list[0]+": "+temp_string+" "+temp_list[1]+"\n";
        }
    }

    ui->plainTextEdit_2->setPlainText(final_ingredients);
}
