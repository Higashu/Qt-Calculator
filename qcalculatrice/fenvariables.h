#ifndef FENVARIABLES_H
#define FENVARIABLES_H

#include <QDialog>
#include <QLineEdit>
#include <QGridLayout>
#include <QVector>
#include "macro.h"
#include "pile.h"
#include "litterales.h"
#include "boutton.h"
#include "userinput.h"

class FenVariables : public QDialog
{
    Q_OBJECT
    QGridLayout *couche;
    QVector<QLineEdit *> TableauValeurExpression;
    QVector<LitteraleExpression *> TableauLitterale;

public:
    FenVariables(QWidget *parent = 0);
    QWidget *creerWidget(const QString texte);

public slots:
    void modifVal();
    void suppVal();
};

#endif // FENVARIABLES_H
