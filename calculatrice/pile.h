#pragma once

#include <string> //A retirer sur Qt
//#include <QString>
#include <vector> //A retirer sur Qt
//#include <QVector>

class Litterale;

/* --------------------------------------------------------------------------*/
/*----- Pile Affichage ----- */
/* --------------------------------------------------------------------------*/

class PileAffichage
{

private:
    std::vector<Litterale *> pile;
    std::string message;
    unsigned int nbAffiche = 10;
    std::vector<Litterale *> pileUndo;
    std::vector<Litterale *> pileRedo;

private:
    PileAffichage() = default;
    PileAffichage(const PileAffichage &) = delete;
    PileAffichage &operator=(const PileAffichage &) = delete;
    ~PileAffichage() = default;

public:
    static PileAffichage &donneInstance();
    unsigned int size() { return pile.size(); }
    void push(Litterale *lit)
    {
        if (lit != nullptr)
            pile.push_back(lit);
    }
    void setNbAffiche(unsigned int n) { nbAffiche = n; }
    unsigned int getNbAffiche() { return nbAffiche; }
    void setMessage(std::string m) { message = m; }
    const std::string getMessage() const { return message; };
    Litterale *top();
    //afficher utile juste pour le debogage
    void afficher() const;
    void pileSave();
    void manageMemory();
    void DROP();
    void SWAP();
    void CLEAR();
    void DUP();
    void UNDO();
    void REDO();
    std::string WHILE(Litterale *, Litterale *);

public:
    //Const iterator de la pile
    using const_iterator = std::vector<Litterale *>::const_iterator;
    const_iterator cbegin() { return pile.cbegin(); }
    const_iterator cend() { return pile.cend(); }
    //Const reverse iterator de la pile
    using const_reverse_iterator = std::vector<Litterale *>::const_reverse_iterator;
    const_reverse_iterator crbegin() { return pile.crbegin(); }
    const_reverse_iterator crend() { return pile.crend(); }
};

/* --------------------------------------------------------------------------*/
/*----- Tableau Expression ----- */
/* --------------------------------------------------------------------------*/

class TableauExpression
{

private:
    std::vector<Litterale *> tableauExpr;

private:
    TableauExpression() = default;
    TableauExpression(const TableauExpression &) = delete;
    TableauExpression &operator=(const TableauExpression &) = delete;
    ~TableauExpression() = default;

public:
    static TableauExpression &donneInstance();
    void push(Litterale *);
    void clear();
    Litterale *getExpression(const std::string) const;
    bool existe(std::string) const;

public:
    //Const iterator du tableau d'expression
    using const_iterator = std::vector<Litterale *>::const_iterator;
    const_iterator cbegin() { return tableauExpr.cbegin(); }
    const_iterator cend() { return tableauExpr.cend(); }
    //Const reverse iterator du tableau d'expression
    using const_reverse_iterator = std::vector<Litterale *>::const_reverse_iterator;
    const_reverse_iterator crbegin() { return tableauExpr.crbegin(); }
    const_reverse_iterator crend() { return tableauExpr.crend(); }
};
