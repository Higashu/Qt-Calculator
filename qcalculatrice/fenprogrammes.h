#ifndef FENPROGRAMMES_H
#define FENPROGRAMMES_H

#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>
#include "macro.h"
#include "pile.h"
#include "litterales.h"
#include "boutton.h"
#include "userinput.h"

class FenProgrammes : public QDialog
{
    Q_OBJECT
    QGridLayout *couche;
    QVector<QLineEdit *> TableauValeurExpression;
    QVector<LitteraleExpression *> TableauLitterale;

public:
    FenProgrammes(QWidget *parent = 0);
    QWidget *creerWidget(const QString texte);

public slots:
    void modifProg();
    void suppProg();
};

#endif // FENPROGRAMMES_H
