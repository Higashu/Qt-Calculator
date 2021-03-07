#include "pile.h"
#include "compception.h"
#include "litterales.h"
#include "macro.h"

/* ----- Pile Affichage ----- */

PileAffichage &PileAffichage::donneInstance()
{
    static PileAffichage singleton;
    return singleton;
}

void PileAffichage::DROP()
{
    if (pile.size() != 0)
        pile.pop_back();
    else
        throw(CompCeption("Erreur : Il n'y a rien a depiler."));
}

void PileAffichage::SWAP()
{
    if (pile.size() < 2)
        throw(CompCeption("Erreur : SWAP impossible, moins de deux elements dans la pile."));
    Litterale *temp = pile[pile.size() - 2];
    pile.erase(pile.end() - 2);
    pile.push_back(temp);
}

void PileAffichage::CLEAR()
{
    pile.clear();
}

void PileAffichage::DUP()
{
    if (pile.size() < 1)
        throw(CompCeption("Erreur : DUP impossible, aucun element dans la pile."));
    pile.push_back(pile.back()->getCopie());
}

void PileAffichage::afficher() const
{
    //A Modifier pour Qt
    qInfo() << "------ Ecran ------"
            << "\n";

    for (auto element : pile)
        qInfo() << element->toString();

    qInfo() << "\n"
            << "------  Fin  ------"
            << "\n";
}

Litterale *PileAffichage::top()
{
    //Si la pile est vide on renvoie nullptr
    if (pile.size() == 0)
        return nullptr;
    Litterale *top = pile.back();
    pile.pop_back();
    return top;
}

void PileAffichage::pileSave()
{
    pileUndo.clear();
    pileRedo.clear();
    for (auto litterale : pile)
        pileUndo.push_back(litterale);
}

void PileAffichage::UNDO()
{
    //Aucun UNDO n'a ete effectue avant
    if (pile != pileUndo)
    {
        pileRedo.clear();
        pileRedo = pile;
        pile.clear();
        pile = pileUndo;
    }
    else
        throw(CompCeption("Erreur : operation UNDO impossible."));
}

void PileAffichage::REDO()
{
    //Un UNDO a ete fait precedemment
    if (pile != pileRedo)
    {
        pile.clear();
        pile = pileRedo;
    }
    else
        throw(CompCeption("Erreur : operation REDO impossible."));
}

QString PileAffichage::WHILE(Litterale *lit1, Litterale *lit2)
{
    if (lit2->getType() != programme && lit2->getType() != expression )
        throw CompCeption("Erreur : Operateur WHILE uniquement valide avec un programme ou une expression.");

    QString commande;
    //Tant que la litterale a pour valeur logique true et que la pile n'est pas vide
    while (lit1 != nullptr && (lit1->getType() != entiere || static_cast<LitteraleEntiere *>(lit1)->getValue() != 0))
    {
        //On concatene le resultat de l'evaluatIon)        
        commande += lit2->EVAL()->rawInput();
        lit1 = PILE.top();
        //Espace entre chaque commande si le prochain argument est different de faux et la pile n'est pas vide
        if (lit1 != nullptr && (lit1->getType() != entiere || static_cast<LitteraleEntiere *>(lit1)->getValue() != 0))
            commande += " ";
    }
    return commande;
}

//Complexite : O(n^2)
void PileAffichage::manageMemory()
{
    std::vector<Litterale *> unusedLitterale;
    bool stillUsed;

    for (auto itManager = LitteraleManager::donneInstance().cbegin(); itManager != LitteraleManager::donneInstance().cend(); itManager++)
    {
        //On part du principe que la litterale n'est plus utilisee
        stillUsed = false;

        //On fouille dans tous les conteneurs de pile.h pour voir si la litterale est utilisée
        if (!stillUsed)
            for (auto itPileAffichage = PILE.cbegin(); itPileAffichage != PILE.cend(); itPileAffichage++)
            {
                //La litterale est utilisee
                if (*itManager == *itPileAffichage)
                {
                    stillUsed = true;
                    break;
                }
            }

        if (!stillUsed)
            for (auto itPileRedo = pileRedo.cbegin(); itPileRedo != pileRedo.cend(); itPileRedo++)
            {
                //La litterale est utilisee
                if (*itManager == *itPileRedo)
                {
                    stillUsed = true;
                    break;
                }
            }

        if (!stillUsed)
            for (auto itPileUndo = pileUndo.cbegin(); itPileUndo != pileUndo.cend(); itPileUndo++)
            {
                //La litterale est utilisee
                if (*itManager == *itPileUndo)
                {
                    stillUsed = true;
                    break;
                }
            }

        if (!stillUsed)
            for (auto itTabExpr = TABEXPR.cbegin(); itTabExpr != TABEXPR.cend(); itTabExpr++)
            {
                //La litterale est utilisee
                if (*itManager == *itTabExpr)
                {
                    stillUsed = true;
                    break;
                }
                if (*itManager == static_cast<LitteraleExpression *>(*itTabExpr)->getTarget())
                {
                    stillUsed = true;
                    break;
                }
            }

        if (!stillUsed)
            unusedLitterale.push_back(*itManager);
    }

    for (auto litterale : unusedLitterale)
        FACTORY::deleteLitterale(litterale);
}

/* ----- Tableau Expression ----- */

TableauExpression &TableauExpression::donneInstance()
{
    static TableauExpression singleton;
    return singleton;
}

void TableauExpression::push(Litterale *lit)
{
    tableauExpr.push_back(lit);
}

void TableauExpression::clear()
{
    tableauExpr.clear();
}

Litterale *TableauExpression::getExpression(const QString s) const
{
    for (auto element : tableauExpr)
    {
        if (element->toString() == s)
            return element;
    }
    throw(CompCeption("Erreur : Aucune expression ne correspond à l'atome " + s.mid(1, s.size() - 2) + "."));
}

bool TableauExpression::existe(QString s) const
{
    for (auto element : tableauExpr)
    {
        if (element->toString() == s)
            return true;
    }
    return false;
}
