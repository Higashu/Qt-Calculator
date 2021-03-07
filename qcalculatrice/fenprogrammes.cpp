#include "fenprogrammes.h"
#include <QLabel>

FenProgrammes::FenProgrammes(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Gestionnaire de programmes");
    resize(500, 500);

    couche = new QGridLayout;
    setLayout(couche);

    int nb_prog_stocke = 0;
    int compteurGrid = 1;
    for (auto it = TABEXPR.cbegin(); it != TABEXPR.cend(); it++)
    {
        if (static_cast<LitteraleExpression *>(*it)->getTarget() != nullptr && static_cast<LitteraleExpression *>(*it)->getTarget()->getType() == programme)
        {
            QString text = (*it)->toString();
            QWidget *widg = creerWidget(text);
            widg->setMinimumWidth(500);
            int row = ((9 - compteurGrid) / 3);
            int column = ((compteurGrid - 1) % 3);
            couche->addWidget(widg, row, column);
            nb_prog_stocke++;
            compteurGrid++;
        }
    }

    if (nb_prog_stocke == 0)
    {
        QLabel *lab = new QLabel("Aucun programme stocké", this);
        lab->setAlignment(Qt::AlignCenter);
        lab->setFont(QFont("Arial", 14, QFont::Medium, false));
        couche->addWidget(lab);
    }
}

QWidget *FenProgrammes::creerWidget(const QString texte)
{
    QString nom_var = texte;
    QWidget *widget = new QWidget(this);
    widget->setStyleSheet("background-color : #fcf8e8;"
                          "border : 3px solid black;");
    QVBoxLayout *layout = new QVBoxLayout;
    widget->setLayout(layout);

    QLineEdit *name = new QLineEdit(this);
    layout->addWidget(name);
    name->setText("Nom de la variable :  " + texte);
    name->setReadOnly(true);
    name->setAlignment(Qt::AlignCenter);
    name->setFont(QFont("Arial", 14, QFont::Medium, false));
    name->setStyleSheet("background-color: #fcf8e8;"
                        "border : 0px solid black;");

    QLineEdit *aff_value = new QLineEdit(this);
    TableauValeurExpression.push_back(aff_value);

    QString valeur = static_cast<LitteraleExpression *>(TABEXPR.getExpression(texte))->getTarget()->toString();
    TableauLitterale.push_back(static_cast<LitteraleExpression *>(TABEXPR.getExpression(texte)));
    aff_value->setText("Valeur actuelle = " + valeur);
    aff_value->setReadOnly(true);
    aff_value->setAlignment(Qt::AlignCenter);
    aff_value->setFont(QFont("Arial", 12, QFont::Medium, true));
    aff_value->setStyleSheet("background-color: #fcf8e8;"
                             "border : 0px solid black;");
    layout->addWidget(aff_value);

    /*QLabel* lab_modif = new QLabel("Modifier la valeur :",this);
    lab_modif->setAlignment(Qt::AlignLeft);
    layout->addWidget(lab_modif);*/

    QLineEdit *modif_value = new QLineEdit(this);
    modif_value->setFont(QFont("Arial", 12, QFont::Medium, true));
    QString messagePrefixe = "Modifiez le programme ";
    QString messageSuffixe = " et appuyez sur entrée";
    modif_value->setPlaceholderText(messagePrefixe + texte + messageSuffixe);
    modif_value->setStyleSheet("background-color : #70af85");
    layout->addWidget(modif_value);

    boutton *supprimeVariable = new boutton("Supprimer " + texte, this);
    supprimeVariable->setFont(QFont("Arial", 12, QFont::Medium, true));
    supprimeVariable->setMaximumHeight(50);
    supprimeVariable->setStyleSheet("background-color :#df7861 ");
    layout->addWidget(supprimeVariable);

    connect(modif_value, SIGNAL(returnPressed()), this, SLOT(modifProg()));
    connect(modif_value, SIGNAL(returnPressed()), modif_value, SLOT(clear()));
    connect(supprimeVariable, SIGNAL(released()), this, SLOT(suppProg()));

    return widget;
}

void FenProgrammes::modifProg()
{

    QLineEdit *tmp = (QLineEdit *)sender();
    //22 = Nombre de caractere avant le texte
    //44 = 22 + 22 Nombre de caractere après le texte
    QString commande = tmp->placeholderText().mid(22, tmp->placeholderText().size() - 44) + " " + tmp->text() + " STO";
    USERINPUT.execute(commande);
    for (int i = 0; i < TableauLitterale.size(); i++)
    {
        if (TableauLitterale[i]->getTarget() != nullptr)
            TableauValeurExpression[i]->setText(TableauLitterale[i]->getTarget()->toString());
        else
            TableauValeurExpression[i]->setText("Cette expression ne contient rien");
    }
}

void FenProgrammes::suppProg()
{
    boutton *tmp = (boutton *)sender();
    USERINPUT.execute(tmp->text().right(tmp->text().size() - 10) + " FORGET");
    for (int i = 0; i < TableauLitterale.size(); i++)
    {
        if (TableauLitterale[i]->getTarget() != nullptr)
            TableauValeurExpression[i]->setText(TableauLitterale[i]->getTarget()->toString());
        else
            TableauValeurExpression[i]->setText("Cette expression ne contient rien");
    }
}
