#pragma once

#include <QString>
#include <vector>
#include "litterales.h"

class UserInput
{

private:
    const std::vector<QString> operateurBinaire = {"+", "-", "/", "*", "AND", "OR", "IFT", "STO", "DIV", "MOD", "<=", ">=", "<", ">", "==", "!=", "POW", "WHILE"};
    const std::vector<QString> operateurUnaire = {"NOT", "NEG", "EVAL", "SIN", "COS", "TAN", "ARCSIN", "ARCCOS", "ARCTAN", "SQRT", "EXP", "LN", "DEN", "NUM", "FORGET"};
    const std::vector<QString> operateurPile = {"CLEAR", "DUP", "DROP", "SWAP", "UNDO", "REDO"};
    const std::vector<QString> operateurTernaire = {"IFTE"};

private:
    UserInput() = default;
    UserInput(const UserInput &) = delete;
    UserInput &operator=(const UserInput &) = delete;
    ~UserInput() = default;

public:
    static UserInput &donneInstance();
    void loadExpression();
    void saveExpression();
    bool isOpUnaire(QString);
    bool isOpBinaire(QString);
    bool isOpPile(QString);
    bool isOpTernaire(QString);
    const std::vector<QString> stripCommande(QString, bool = true);
    void commandeInterpreteur(std::vector<QString>, bool = false);
    void handleLitterale(std::pair<int, int>);
    void handleLitterale(std::pair<double, int>);
    void handleLitterale(std::pair<QString, int>);
    void handleOperateurUnaire(QString);
    void handleOperateurBinaire(QString);
    void handleOperateurTernaire(QString);
    void handleOperateurPile(QString);
    bool execute(QString);
};
