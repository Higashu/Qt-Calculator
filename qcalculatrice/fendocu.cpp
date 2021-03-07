#include "fendocu.h"

FenDocu::FenDocu(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Documentation");
    resize(400, 400);

    layout = new QHBoxLayout;
    setLayout(layout);

    documentation = new QTextEdit(this);
    documentation->setMinimumSize(700, 700);
    documentation->setFont(QFont("Arial", 12, QFont::Normal, false));
    layout->addWidget(documentation);

    documentation->setReadOnly(true);
    documentation->setText("<h1>Documentation de la calculatrice</h1>");
    documentation->append("<h2>Opérateurs numériques</h2>");
    documentation->append("DIV : division entière, MOD : modulo (binaire)");
    documentation->append("NEG : changer le signe de la dernière littéralle empilée (unaire)");
    documentation->append("NUM : renvoie le numérateur, DEN : renvoie le dénominateur (unaires)");
    documentation->append("POW : puissance (1er de la pile exposant 2eme de la pile nombre à multiplier binaire)");
    documentation->append("SIN, COS, TAN, ARCSIN, ARCCOS, ARCTAN");
    documentation->append("SQRT : racine carée");
    documentation->append("EXP : exponentielle");
    documentation->append("LN : logarithme népérien");
    documentation->append("=, !=, =<, >=, <, > : tests comparatifs (binaires) renvoie 1 si TRUE, 0 sinon");
    documentation->append("AND, OR, NOT : opérareurs logiquess (binaires)");

    documentation->append("<h2>Opérateurs de manipulation de la pile</h2>");
    documentation->append("DUP : dupliquer la dernière littérale empilée");
    documentation->append("DROP : supprime la dernière littérale empilée");
    documentation->append("SWAP : intervertit les deux derniers éléments empilés");
    documentation->append("CLEAR : vide tous les éléments de la pile");
    documentation->append("UNDO : revient en arrière");
    documentation->append("REDO : rétablit l’état du calculateur avant l’application du dernier UNDO");

    documentation->append("<h2>Opérateurs conditionnels de boucle</h2>");
    documentation->append("IFT : dépile 2 arguments. Le 1er est un test logique.  Si la valeur de ce test est vrai, le 2e argument est évalué sinon il est abandonné.");
    documentation->append("IFTE : dépile 3 arguments. Le 1er est un test logique. Si la valeur de ce test est vrai, le 2e argument est évalué et le 3e argument est abandonné sinon le 3e argument est évalué et le 2e argument est abandonné.");
    documentation->append("WHILE : dépile 2 arguments. Le 1er est un test logique. Tant que le test est vrai, le deuxième argument est évalué.");
}
