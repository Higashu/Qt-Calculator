#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FenPrincipale comput;
    try
    {
        USERINPUT.loadExpression();
        PILE.setMessage("Bienvenue les loulous !");
    }
    catch (CompCeption e)
    {
        PILE.setMessage(e.getMessage());
    }
    comput.refresh();
    comput.show();
    return a.exec();
}
