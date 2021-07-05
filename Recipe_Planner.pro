QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    add_recipe.cpp \
    globalvariables.cpp \
    ingredient_calculator.cpp \
    main.cpp \
    recipe_planner.cpp

HEADERS += \
    add_recipe.h \
    globalvariables.h \
    ingredient_calculator.h \
    recipe_planner.h

FORMS += \
    add_recipe.ui \
    ingredient_calculator.ui \
    recipe_planner.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Recipes.js
