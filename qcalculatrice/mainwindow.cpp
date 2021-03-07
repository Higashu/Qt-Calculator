#include "mainwindow.h"
#include "ui_mainwindow.h"

FenPrincipale::FenPrincipale()
{

    setWindowTitle("Compuuuuuuut !");
    commande = new QLineEdit(this);
    commande->setStyleSheet("background-color: #70af85");
    commande->setFont(QFont("Arial", 12, QFont::Bold));
    commande->setMinimumHeight(60);
    commande->setFocus(Qt::OtherFocusReason);

    exception = new QLineEdit(this);
    exception->setReadOnly(true);
    exception->setStyleSheet("background-color:#ecb390");
    exception->setFont(QFont("Arial", 10, QFont::Bold, true));
    exception->setMinimumHeight(60);
    exception->setAlignment(Qt::AlignCenter);

    QWidget *zoneCentrale = new QWidget(this);
    setCentralWidget(zoneCentrale);
    zoneCentrale->setMaximumWidth(1200);
    zoneCentrale->setMinimumWidth(800);

    QVBoxLayout *couche = new QVBoxLayout;
    zoneCentrale->setLayout(couche);

    vuePile = new QTableWidget(PILE.getNbAffiche(), 1, this);
    vuePile->setEditTriggers(QAbstractItemView::NoEditTriggers);
    vuePile->horizontalHeader()->setVisible(false);
    vuePile->verticalHeader()->setVisible(false);
    vuePile->horizontalHeader()->setStretchLastSection(true);
    vuePile->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    vuePile->setStyleSheet("background-color:#fcf8e8");
    vuePile->setMaximumHeight(800);
    vuePile->setFont(QFont("Arial", 12, QFont::Bold, true));

    //labels du qtable widget
    QStringList labels;
    for (size_t i = PILE.getNbAffiche(); i > 0; i--)
    {
        labels << QString::number(i) + " :";
        //création d'un item de chaque ligne initialisé avec ""
        vuePile->setItem(i - 1, 0, new QTableWidgetItem(""));
        vuePile->item(i - 1, 0)->setTextAlignment(Qt::AlignCenter);
    }
    vuePile->setVerticalHeaderLabels(labels);

    couche->addWidget(exception);
    couche->addWidget(vuePile);
    couche->addWidget(commande);

    //boutons permettant l'ouverture des vues secondaires
    QDir dir(PROJECT_PATH);
    toolBarFichier = addToolBar("tools");
    QAction *actionVar = toolBarFichier->addAction("Gestionnaire de variables");
    actionVar->setIcon(QIcon(dir.absolutePath() + "/icons/x.png"));
    QAction *actionProg = toolBarFichier->addAction("Gestionnaire de programmes");
    actionProg->setIcon(QIcon(dir.absolutePath() + "/icons/prog.png"));
    QAction *actionDocu = toolBarFichier->addAction("Documentation");
    actionDocu->setIcon(QIcon(dir.absolutePath() + "/icons/documentation.png"));

    connect(actionVar, &QAction::triggered, this, &FenPrincipale::afficherGestVar);
    connect(actionProg, &QAction::triggered, this, &FenPrincipale::afficherGestProg);   
    connect(actionDocu, &QAction::triggered, this, &FenPrincipale::afficherDocu);

    //Nombres d'éléments affichés dans la pile
    toolBarFichier->addSeparator();
    QLabel *Lab_nbPile = new QLabel("Taille pile :", this);
    toolBarFichier->addWidget(Lab_nbPile);
    QSpinBox *nb_Affich_Pile = new QSpinBox(this);
    nb_Affich_Pile->setValue(PILE.getNbAffiche());

    connect(nb_Affich_Pile, SIGNAL(valueChanged(int)), &PILE, SLOT(setNbAffiche(int)));

    toolBarFichier->addWidget(nb_Affich_Pile);
    toolBarFichier->addSeparator();
    QLabel *Lab_Affichage_pave = new QLabel("Affichage des pavés : ", this);
    toolBarFichier->addWidget(Lab_Affichage_pave);

    // Créations des docks constants : numériques et opérateurs
    creerDockConstants();
    creerDockVar_Prog();

    //undo et redo
    toolBarFichier->addSeparator();
    QAction *action_undo = toolBarFichier->addAction("UNDO");
    action_undo->setIcon(QIcon(dir.absolutePath() + "/icons/undo.png"));
    QAction *action_redo = toolBarFichier->addAction("REDO");
    action_redo->setIcon(QIcon(dir.absolutePath() + "/icons/redo.png"));
    QAction *action_delete = toolBarFichier->addAction("DROP");
    action_delete->setIcon(QIcon(dir.absolutePath() + "/icons/delete.png"));
    QAction *action_dup = toolBarFichier->addAction("DUP");
    action_dup->setIcon(QIcon(dir.absolutePath() + "/icons/dup.png"));
    QAction *action_swap = toolBarFichier->addAction("SWAP");
    action_swap->setIcon(QIcon(dir.absolutePath() + "/icons/echange.png"));
    QAction *action_clear = toolBarFichier->addAction("CLEAR");
    action_clear->setIcon(QIcon(dir.absolutePath() + "/icons/clear.png"));

    connect(action_undo, SIGNAL(triggered()), this, SLOT(slotUndo()));
    connect(action_redo, SIGNAL(triggered()), this, SLOT(slotRedo()));
    connect(action_delete, SIGNAL(triggered()), this, SLOT(slotDrop()));
    connect(action_dup, SIGNAL(triggered()), this, SLOT(slotDup()));
    connect(action_swap, SIGNAL(triggered()), this, SLOT(slotSwap()));
    connect(action_clear, SIGNAL(triggered()), this, SLOT(slotClear()));

    connect(commande, SIGNAL(returnPressed()), this, SLOT(sendCommande()));
    connect(&PILE, SIGNAL(modificationEtat()), this, SLOT(refresh()));
}

FenPrincipale::~FenPrincipale()
{
    USERINPUT.saveExpression();
}

boutton *FenPrincipale::creerBouton(const QString texte, const char *member)
{
    boutton *button = new boutton(texte, this);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

void FenPrincipale::creerDockConstants()
{
    QDockWidget *dockConstant = new QDockWidget("Chiffres / Opérateur", this);
    dockConstant->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dockConstant);
    toolBarFichier->addAction(dockConstant->toggleViewAction());

    //pavé numérique (chiffres, virgule et négatif)
    QWidget *widget_pave_num = new QWidget(this);
    QGridLayout *pave_num = new QGridLayout;

    //création des boutons numériques grace à creerBouton()
    enum
    {
        NumDigitButtons = 9
    };
    boutton *digitButtons[NumDigitButtons];

    for (int i = 0; i < NumDigitButtons; ++i)
    {
        digitButtons[i] = creerBouton(QString::number(i + 1), SLOT(bouttonPaveClicked()));
        digitButtons[i]->setMinimumSize(90, 90);
    }

    for (int i = 1; i <= NumDigitButtons; ++i)
    {
        int row = ((9 - i) / 3);
        int column = ((i - 1) % 3);
        pave_num->addWidget(digitButtons[i - 1], row, column);
    }

    boutton *bt_virgule = creerBouton(".", SLOT(bouttonPaveClicked()));
    boutton *bt_zero = creerBouton("0", SLOT(bouttonPaveClicked()));
    boutton *bt_minus = creerBouton("NEG", SLOT(bouttonPaveClicked()));

    bt_virgule->setMinimumSize(90, 90);
    bt_zero->setMinimumSize(90, 90);
    bt_minus->setMinimumSize(90, 90);

    pave_num->addWidget(bt_virgule, 3, 0);
    pave_num->addWidget(bt_zero, 3, 1);
    pave_num->addWidget(bt_minus, 3, 2);

    widget_pave_num->setLayout(pave_num);

    //pavé opérateurs (plus, mois, fois, /, abs, div, neg, )
    QWidget *widget_pave_oper = new QWidget(this);
    QGridLayout *pave_oper = new QGridLayout;

    boutton *bt_space = creerBouton("ESPACE", SLOT(spaceClicked()));
    bt_space->setMinimumSize(40, 60);
    boutton *bt_return = creerBouton("EFFACER", SLOT(returnClicked()));
    bt_return->setMinimumSize(40, 60);
    boutton *bt_plus = creerBouton("+", SLOT(bouttonOperatorClicked()));
    bt_plus->setMinimumSize(40, 60);
    boutton *bt_moins = creerBouton("-", SLOT(bouttonOperatorClicked()));
    bt_moins->setMinimumSize(40, 60);
    boutton *bt_fois = creerBouton("*", SLOT(bouttonOperatorClicked()));
    bt_fois->setMinimumSize(40, 60);
    boutton *bt_div = creerBouton("/", SLOT(bouttonOperatorClicked()));
    bt_div->setMinimumSize(40, 60);
    boutton *bt_neg = creerBouton("MOD", SLOT(bouttonPaveClicked()));
    bt_neg->setMinimumSize(40, 60);
    boutton *bt_div_entiere = creerBouton("DIV", SLOT(bouttonPaveClicked()));
    bt_div_entiere->setMinimumSize(40, 60);
    boutton *bt_enter = creerBouton("ENTREE", SLOT(sendCommande()));
    bt_enter->setMinimumHeight(60);

    pave_oper->addWidget(bt_space, 0, 0);
    pave_oper->addWidget(bt_return, 0, 1);
    pave_oper->addWidget(bt_plus, 1, 0);
    pave_oper->addWidget(bt_moins, 1, 1);
    pave_oper->addWidget(bt_fois, 2, 0);
    pave_oper->addWidget(bt_div, 2, 1);
    pave_oper->addWidget(bt_neg, 3, 0);
    pave_oper->addWidget(bt_div_entiere, 3, 1);
    pave_oper->addWidget(bt_enter, 4, 0, 1, 0);
    widget_pave_oper->setLayout(pave_oper);

    QVBoxLayout *colonne_pave_const = new QVBoxLayout;
    colonne_pave_const->addWidget(widget_pave_num);
    colonne_pave_const->addWidget(widget_pave_oper);

    QWidget *widget_pave_constant = new QWidget(this);
    widget_pave_constant->setLayout(colonne_pave_const);

    dockConstant->setWidget(widget_pave_constant);
}

void FenPrincipale::creerDockVar_Prog()
{
    //création de l'action dans la toolbar permettant d'afficher et désafficher le pavé var et prog
    QDockWidget *dockVar = new QDockWidget("Variables / Programmes", this);
    dockVar->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, dockVar);
    toolBarFichier->addAction(dockVar->toggleViewAction());

    //création du widget principal englobant les layouts de pavé
    QWidget *widget_pave_var_prog = new QWidget(this);
    dockVar->setWidget(widget_pave_var_prog);

    //layout
    QVBoxLayout *couche = new QVBoxLayout;
    couche->setAlignment(Qt::AlignVCenter);
    couche->setSpacing(40);
    widget_pave_var_prog->setLayout(couche);

    //              VARIABLES           //
    //créer 10 tool buttons vides et dans refresh on modifie
    QLabel *label_var = new QLabel("Variables", this);
    label_var->setAlignment(Qt::AlignCenter);
    label_var->setFont(QFont("Arial", 18, QFont::Bold, false));
    couche->addWidget(label_var);

    QGridLayout *pave_var = new QGridLayout;
    pave_var->setSpacing(5);
    couche->addLayout(pave_var);

    //      PROGRAMMES      //
    QLabel *label_prog = new QLabel("Programmes", this);
    label_prog->setAlignment(Qt::AlignCenter);
    label_prog->setFont(QFont("Arial", 18, QFont::Bold, false));
    couche->addWidget(label_prog);

    QGridLayout *pave_prog = new QGridLayout;
    pave_prog->setSpacing(5);
    couche->addLayout(pave_prog);

    for (int i = 0; i < nombreBoutonVar_Prog; ++i)
    {

        //Pave variable
        tabPaveVar.push_back(creerBouton("", SLOT(bouttonPaveClicked())));
        tabPaveVar.back()->setMinimumSize(80, 60);
        int row = ((nombreBoutonVar_Prog - (i + 1)) / 3);
        int column = (((i + 1) - 1) % 3);
        pave_var->addWidget(tabPaveVar[i], row, column);

        //Pave programme
        tabPaveProg.push_back(creerBouton("", SLOT(bouttonPaveClicked())));
        tabPaveProg.back()->setMinimumSize(80, 60);
        row = ((nombreBoutonVar_Prog - (i + 1)) / 3);
        column = (((i + 1) - 1) % 3);
        pave_prog->addWidget(tabPaveProg[i], row, column);
    }
}

//slots

void FenPrincipale::refresh()
{

    //Si on a changé le nbre d'item a afficher, on modifie le nombre de ligne dans le tableau et on renumerote les labels
    if (vuePile->rowCount() != static_cast<int>(PILE.getNbAffiche()))
    {
        vuePile->setRowCount(PILE.getNbAffiche());
        QStringList labels;
        for (size_t i = PILE.getNbAffiche(); i > 0; i--)
        {
            labels << QString::number(i) + " :";
            //création d'un item de chaque ligne initialisé avec ""
            vuePile->setItem(i - 1, 0, new QTableWidgetItem(""));
            vuePile->item(i - 1, 0)->setTextAlignment(Qt::AlignCenter);
        }
        vuePile->setVerticalHeaderLabels(labels);
    }

    //On affiche le message de la PILE s'il y en a un
    exception->setText(PILE.getMessage());

    //On vide les cellules du tableau en remplacent le texte par rien
    for (size_t i = 0; i < PILE.getNbAffiche(); i++)
        vuePile->item(i, 0)->setText("");
    for (size_t i = 0; i < static_cast<unsigned int>(tabPaveVar.size()); i++)
    {
        tabPaveVar[i]->setText("");
        tabPaveProg[i]->setText("");
    }

    size_t nb = 0;

    //On remplie les cases du tableau avec le contenu de la pile
    for (auto it = PILE.crbegin(); it != PILE.crend() && nb < PILE.getNbAffiche(); ++it)
    {
        vuePile->item(PILE.getNbAffiche() - nb - 1, 0)->setText((*it)->toString());
        nb++;
    }

    nb = 0;

    //on modifie les pavés variables et programmes si changement
    for (auto it = TABEXPR.cbegin(); it != TABEXPR.cend() && nb < static_cast<unsigned int>(tabPaveVar.size()); it++)
    {
        if (static_cast<LitteraleExpression *>(*it)->getTarget() != nullptr && static_cast<LitteraleExpression *>(*it)->getTarget()->getType() != programme)
        {
            tabPaveVar[nb]->setText((*it)->toString().mid(1, (*it)->toString().size() - 2));
            nb++;
        }
    }

    nb = 0;

    for (auto it = TABEXPR.cbegin(); it != TABEXPR.cend() && nb < static_cast<unsigned int>(tabPaveVar.size()); it++)
    {
        if (static_cast<LitteraleExpression *>(*it)->getTarget() != nullptr && static_cast<LitteraleExpression *>(*it)->getTarget()->getType() == programme)
        {
            tabPaveProg[nb]->setText((*it)->toString().mid(1, (*it)->toString().size() - 2));
            nb++;
        }
    }
}

void FenPrincipale::sendCommande()
{
    //La commande a réussi
    if (USERINPUT.execute(commande->text()))
        commande->clear();
    exception->setText(PILE.getMessage());
}

void FenPrincipale::bouttonPaveClicked()
{
    boutton *clickedButton = (boutton *)sender();
    commande->setText(commande->text() + clickedButton->text());
}

void FenPrincipale::bouttonOperatorClicked()
{
    boutton *clickedButton = (boutton *)sender();

    //Si la commande est vide on envoie juste le signe divisé
    if (commande->text().isEmpty())
    {
        commande->setText(commande->text() + clickedButton->text());
        return;
    }
    //On vire les espaces s'il y en a en trop
    while (commande->text().back().isSpace())
        commande->setText(commande->text().left(commande->text().size() - 1));

    //On rajoute un espace avant le divisé
    commande->setText(commande->text() + " " + clickedButton->text());
    commande->returnPressed();
}

void FenPrincipale::spaceClicked()
{
    commande->setText(commande->text() + " ");
}

void FenPrincipale::returnClicked()
{
    commande->setText(commande->text().left(commande->text().size() - 1));
}

//pop up des gestionnaires
void FenPrincipale::afficherGestVar()
{
    FenVariables *fenetre2 = new FenVariables(this);
    fenetre2->exec();
}

void FenPrincipale::afficherGestProg()
{
    FenProgrammes *fenetre3 = new FenProgrammes(this);
    fenetre3->exec();
}

void FenPrincipale::afficherDocu()
{
    FenDocu *fenetreDocu = new FenDocu(this);
    fenetreDocu->exec();
}

void FenPrincipale::slotUndo()
{
    USERINPUT.execute("UNDO");
}

void FenPrincipale::slotRedo()
{
    USERINPUT.execute("REDO");
}

void FenPrincipale::slotClear()
{
    USERINPUT.execute("CLEAR");
}

void FenPrincipale::slotDrop()
{
    USERINPUT.execute("DROP");
}

void FenPrincipale::slotSwap()
{
    USERINPUT.execute("SWAP");
}

void FenPrincipale::slotDup()
{
    USERINPUT.execute("DUP");
}
