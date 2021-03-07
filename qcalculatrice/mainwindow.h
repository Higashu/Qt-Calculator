#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QObject>
#include <QProcess>
#include <vector>
#include <string>
#include <QDebug>
#include "fenvariables.h"
#include "fenprogrammes.h"
#include "fendocu.h"
#include "boutton.h"
#include "userinput.h"
#include "macro.h"
#include "pile.h"
#include "litterales.h"

class FenPrincipale : public QMainWindow
{
    Q_OBJECT

    QLineEdit *commande;
    QLineEdit *exception;
    QToolBar *toolBarFichier;
    QTableWidget *vuePile;
    QVector<boutton *> tabPaveVar;
    QVector<boutton *> tabPaveProg;
    int nombreBoutonVar_Prog = 9;

public:
    FenPrincipale();
    ~FenPrincipale();
    boutton *creerBouton(const QString texte, const char *member);
    void creerDockConstants();
    void creerDockVar_Prog();

public slots:
    void sendCommande();
    void refresh();
    void afficherGestVar();
    void afficherGestProg();
    void afficherDocu();
    void bouttonPaveClicked();
    void bouttonOperatorClicked();
    void spaceClicked();
    void returnClicked();
    void slotUndo();
    void slotRedo();
    void slotDrop();
    void slotDup();
    void slotSwap();
    void slotClear();
};
#endif // MAINWINDOW_H
