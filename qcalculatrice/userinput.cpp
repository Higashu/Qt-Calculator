#include <QRegExp>
#include <string>  //Compatibilité avec la bibliothèque JSON for Modern C++
#include <iomanip> // std::setw()
#include <fstream> // std::ifstream(), std::ofstream()
#include "userinput.h"
#include "compception.h"
#include "pile.h"
#include "litterales.h"
#include "json.hpp"
#include "macro.h"

/* ----- User Input ----- */

UserInput &UserInput::donneInstance()
{
    static UserInput singleton;
    return singleton;
}

bool UserInput::isOpUnaire(QString operateur)
{
    for (size_t i = 0; i < operateurUnaire.size(); i++)
    {
        if (operateurUnaire[i] == operateur)
            return true;
    }
    return false;
}

bool UserInput::isOpBinaire(QString operateur)
{
    for (size_t i = 0; i < operateurBinaire.size(); i++)
    {
        if (operateurBinaire[i] == operateur)
            return true;
    }
    return false;
}

bool UserInput::isOpPile(QString operateur)
{
    for (size_t i = 0; i < operateurPile.size(); i++)
    {
        if (operateurPile[i] == operateur)
            return true;
    }
    return false;
}

bool UserInput::isOpTernaire(QString operateur)
{
    for (size_t i = 0; i < operateurTernaire.size(); i++)
    {
        if (operateurTernaire[i] == operateur)
            return true;
    }
    return false;
}

void UserInput::loadExpression()
{

    std::ifstream file("expression.json");
    //Si le fichier n'existe pas encore ou l'ouverture a échoué on continue sans lancer d'exception.
    if (file.fail())
        return;
    try
    {
        nlohmann::json objet;
        std::string expression;
        std::string commande;
        file >> objet;

        for (auto it = objet.begin(); it != objet.end(); it++)
        {
            expression = (*it).at("nom");
            commande = (*it).at("commande");
            commandeInterpreteur(stripCommande(QString::fromStdString(expression + " " + commande + " STO"), false));
        }
        file.close();
    }
    catch (...)
    {
        throw CompCeption("Erreur : Chargement des expressions interrompu, la sauvegarde est corrompue.");
    }
}

void UserInput::saveExpression()
{
    //Cree des objets vide
    nlohmann::json objet;
    nlohmann::json objetFinal = nlohmann::json::object();
    unsigned int compteur = 0;
    for (auto it = TABEXPR.cbegin(); it != TABEXPR.cend(); it++)
    {
        LitteraleExpression *expr = static_cast<LitteraleExpression *>(*it);

        //On sauvegarde uniquement les expressions qui ont une target
        if (expr->getTarget() != nullptr)
        {
            compteur++;
            //Sinon il manque les crochets avec rawInput
            if (expr->getTarget()->getType() == programme)
                objet["commande"] = expr->getTarget()->toString().toStdString();
            else
                objet["commande"] = expr->getTarget()->rawInput().toStdString();
            objet["nom"] = expr->toString().toStdString();
            objetFinal["expression" + std::to_string(compteur)] = objet;
        }
    }

    std::ofstream file("expression.json");
    //Fixe les espaces et ecrit dans le fichier
    file << std::setw(4) << objetFinal << "\n";
    file.close();
    qInfo() << "Ciaaooo, amigo de la noche !!";
}

const std::vector<QString> UserInput::stripCommande(QString commande, bool pileToSave)
{
    std::vector<QString> userInputStripped;

    if (commande.isEmpty())
    {
        userInputStripped.push_back("");
        return userInputStripped;
    }

    QRegExp regExp;
    regExp.setPattern("(^([\\S]+\\s?)+[\\S]$)|(^[\\S]+$)|(^.{0}$)");

    if (!regExp.exactMatch(commande))
        throw CompCeption("Erreur : Espace inattendu.");

    QString temp;
    unsigned int crochetOuvrant = 0;

    for (int i = 0; i < commande.size(); i++)
    {
        //Si on entre dans un programme ou qu'on est déjà dans un programme
        if (commande[i] == '[' || crochetOuvrant != 0)
        {
            temp.push_back(commande[i]);

            if (commande[i] == '[')
                crochetOuvrant += 1;

            if (commande[i] == ']')
                crochetOuvrant -= 1;

            if (i == commande.size() - 1 && crochetOuvrant != 0)
                throw CompCeption("Erreur : Crochet non referme.");

            else if (i == commande.size() - 1)
                userInputStripped.push_back(temp);
        }
        //On est pas/plus dans un programme
        else
        {
            if (!commande[i].isSpace())
                temp.push_back(commande[i]);
            if (commande[i].isSpace() || i == commande.size() - 1)
            {
                userInputStripped.push_back(temp);
                temp.clear();
            }
        }
    }

    if (crochetOuvrant != 0)
        throw CompCeption("Erreur : Crochet non referme.");

    regExp.setPattern("(^[0-9]+$)|(^[0-9]*[.][0-9]+$)|(^[A-Z]{1}[A-Z0-9]*$)|(^'[A-Z]{1}[A-Z0-9]*'$)|(^\\[(\\s[\\S]+)*\\s\\]$)");
    for (size_t i = 0; i < userInputStripped.size(); i++)
    {
        QString element = userInputStripped[i];
        if (!regExp.exactMatch(element) && !isOpBinaire(element) && !isOpPile(element) && !isOpUnaire(element) && !isOpTernaire(element))
            throw CompCeption("Erreur : La commande : " + element + " est inconnue.");
    }

    //Si on fait un UNDO, REDO ou que la commande est vide, on ne sauvegarde pas la pile d'affichage
    if (userInputStripped.front() != "UNDO" && userInputStripped.front() != "REDO" && pileToSave)
        PILE.pileSave();

    return userInputStripped;
}

void UserInput::commandeInterpreteur(std::vector<QString> userInputStripped, bool programmeSanityCheck)
{

    QRegExp regExp;

    for (auto element : userInputStripped)
    {
        //Si la premiere commande est vide la seule possibilité c'est que ça soit la seul commande, on ne lance pas d'exception et on fait rien
        if (element == "")
            return;

        if (isOpUnaire(element))
        {
            //Si on ne fait pas un SanityCheck on appel HandleOperateur
            if (!programmeSanityCheck)
                USERINPUT.handleOperateurUnaire(element);

            continue;
        }

        else if (isOpBinaire(element))
        {
            //Si on ne fait pas un SanityCheck on appel HandleOperateur
            if (!programmeSanityCheck)
                USERINPUT.handleOperateurBinaire(element);

            continue;
        }

        else if (isOpTernaire(element))
        {
            //Si on ne fait pas un SanityCheck on appel HandleOperateur
            if (!programmeSanityCheck)
                USERINPUT.handleOperateurTernaire(element);

            continue;
        }

        else if (isOpPile(element))
        {
            //Si on ne fait pas un SanityCheck on appel HandleOperateur
            if (!programmeSanityCheck)
                USERINPUT.handleOperateurPile(element);

            continue;
        }

        regExp.setPattern("^[0-9]+$");
        if (regExp.exactMatch(element))
        {

            if (!programmeSanityCheck)
                USERINPUT.handleLitterale(std::pair<int, int>(element.toInt(), entiere));

            continue;
        }

        regExp.setPattern("^[0-9]*[.][0-9]+$");
        if (regExp.exactMatch(element))
        {

            if (!programmeSanityCheck)
                USERINPUT.handleLitterale(std::pair<double, int>(element.toDouble(), reelle));

            continue;
        }

        regExp.setPattern("^[A-Z]{1}[A-Z0-9]*$");
        if (regExp.exactMatch(element) && !isOpPile(element) && !isOpBinaire(element) && !isOpUnaire(element) && !isOpTernaire(element))
        {

            if (!programmeSanityCheck)
                USERINPUT.handleLitterale(std::pair<QString, int>(element, atome));

            //On est en train de vérifier la validité d'un programme
            else
            {

                //Si l'atome pointe vers une expression qui n'existe pas, si c'est le cas on averti l'utilisateur simplement
                if (!TABEXPR.existe("'" + element + "'"))
                    PILE.setMessage("Attention : Un programme avec un atome qui ne correspond a aucune expression a ete insere.");
            }

            continue;
        }

        regExp.setPattern("^'[A-Z]{1}[A-Z0-9]*'$");
        if (regExp.exactMatch(element))
        {

            if (!programmeSanityCheck)
                USERINPUT.handleLitterale(std::pair<QString, int>(element, expression));

            continue;
        }

        regExp.setPattern("^\\[(\\s[\\S]+)*\\s\\]$");
        if (regExp.exactMatch(element))
        {

            if (element.size() < 4)
                PILE.setMessage("Attention : Un programme vide a ete insere");
            else
                commandeInterpreteur(stripCommande(element.mid(2, element.size() - 4), false), true);

            if (!programmeSanityCheck)
                USERINPUT.handleLitterale(std::pair<QString, int>(element, programme));

            continue;
        }

        if (programmeSanityCheck)
            throw CompCeption("Erreur : Tentative d'insertion d'un programme avec une commande inconnue : " + element + ".");

        //Si aucune des conditions satisfaite
        throw CompCeption("Erreur : La commande : " + element + " est inconnue.");
    }
}

void UserInput::handleLitterale(std::pair<int, int> paire)
{
    PILE.push(FACTORY::createLitterale(paire.first, static_cast<typeLitterale>(paire.second)));
}

void UserInput::handleLitterale(std::pair<double, int> paire)
{
    PILE.push(FACTORY::createLitterale(paire.first, static_cast<typeLitterale>(paire.second)));
}

void UserInput::handleLitterale(std::pair<QString, int> paire)
{

    // L'utilisateur a rentre un atome
    if (paire.second == atome)
    {

        //Si l'expression n'existe pas, on lance une exception dans getExpression()
        Litterale *lit = TABEXPR.getExpression("'" + paire.first + "'")->EVAL();

        //La fonction rawInput est appelee pour empiler le resultat et on ne veut pas sauvegarder pas la pile donc pileToSave = false
        commandeInterpreteur(stripCommande(lit->rawInput(), false));
        return;
    }

    //L'utilisateur a rentré autre chose qu'un atome
    else
    {

        //Si c'est une expression et qu'on a deja cree l'expression, on l'empile simplement
        if (paire.second == expression && TABEXPR.existe(paire.first) == true)
        {
            PILE.push(TABEXPR.getExpression(paire.first));
            return;
        }

        //L'expression ou le programme sont nouveaux alors on les cree avec la factory
        Litterale *lit = FACTORY::createLitterale(paire.first, static_cast<typeLitterale>(paire.second));
        PILE.push(lit);

        //Si c'est une expression on la push dans tabexpr
        if (paire.second == expression)
            TABEXPR.push(lit);
    }
}

/* ----- Opérateur Unaire ----- */

void UserInput::handleOperateurUnaire(QString element)
{

    Litterale *lit1 = PILE.top();

    try
    {
        if (lit1 == nullptr)
            throw CompCeption("Erreur : Pas assez d'elements dans la pile pour l'operation : " + element);

        else if (element == "EVAL")
        {
            Litterale *lit = lit1->EVAL();
            commandeInterpreteur(stripCommande(lit->rawInput(), false));
        }

        else if (element == "NOT")
            PILE.push(lit1->NOT());

        else if (element == "NEG")
            PILE.push(lit1->NEG());

        else if (element == "SIN")
            PILE.push(lit1->SIN());

        else if (element == "COS")
            PILE.push(lit1->COS());

        else if (element == "TAN")
            PILE.push(lit1->TAN());

        else if (element == "ARCSIN")
            PILE.push(lit1->ARCSIN());

        else if (element == "ARCCOS")
            PILE.push(lit1->ARCCOS());

        else if (element == "ARCTAN")
            PILE.push(lit1->ARCTAN());

        else if (element == "SQRT")
            PILE.push(lit1->SQRT());

        else if (element == "LN")
            PILE.push(lit1->LN());

        else if (element == "EXP")
            PILE.push(lit1->EXP());

        else if (element == "NUM")
            PILE.push(lit1->NUM());

        else if (element == "DEN")
            PILE.push(lit1->DEN());

        else if (element == "FORGET")
            lit1->FORGET();

        else
            throw CompCeption("Erreur : L'operateur " + element + " a ete defini dans tabOpUnaire mais pas dans HandleOperateur");

        return;
    }
    catch (CompCeption e)
    {
        PILE.push(lit1);
        throw;
    }
}

/* ----- Opérateur Binaire ----- */

void UserInput::handleOperateurBinaire(QString element)
{

    Litterale *lit1 = PILE.top();
    Litterale *lit2 = PILE.top();

    try
    {
        if (lit2 == nullptr)
            throw CompCeption("Erreur : Pas assez d'elements dans la pile pour l'operation : " + element);

        else if (element == "+")
            PILE.push(lit2->operator+(lit1));

        else if (element == "-")
            PILE.push(lit2->operator-(lit1));

        else if (element == "/")
            PILE.push(lit2->operator/(lit1));

        else if (element == "*")
            PILE.push(lit2->operator*(lit1));

        else if (element == "STO")
            lit2->STO(lit1);

        else if (element == "AND")
            PILE.push(lit2->AND(lit1));

        else if (element == "OR")
            PILE.push(lit2->OR(lit1));

        else if (element == "IFT")
        {
            //J'ai inversé, c'est l'expression qui appelle IFT : Si ce que tu me donnes est TRUE alors je m'execute
            Litterale *res = lit1->IFT(lit2);
            //Si le test logique vaut 0 on renvoie nullptr
            if (res != nullptr)
            {
                commandeInterpreteur(stripCommande(res->rawInput(), false));
                return;
            }
        }

        else if (element == "DIV")
            PILE.push(lit2->DIV(lit1));

        else if (element == "MOD")
            PILE.push(lit2->MOD(lit1));

        else if (element == "<=")
            PILE.push(lit2->operator<=(lit1));

        else if (element == ">=")
            PILE.push(lit2->operator>=(lit1));

        else if (element == "<")
            PILE.push(lit2->operator<(lit1));

        else if (element == ">")
            PILE.push(lit2->operator>(lit1));

        else if (element == "==")
            PILE.push(lit2->operator==(lit1));

        else if (element == "!=")
            PILE.push(lit2->operator!=(lit1));

        else if (element == "POW")
            PILE.push(lit2->POW(lit1));

        else if (element == "WHILE")
            commandeInterpreteur(stripCommande(PILE.WHILE(lit2, lit1), false));

        else
            throw CompCeption("Erreur : L'operateur " + element + " a ete defini dans tabOpBinaire mais pas dans HandleOperateur");

        return;
    }
    catch (CompCeption e)
    {
        //Si lit1 ou lit2 = nullptr, la pile ne fait rien, donc OK
        PILE.push(lit2);
        PILE.push(lit1);
        //On fait remonter l'exception pour arrêter l'execution de la commande entiere
        throw;
    }
}

/* ----- Opérateur Ternaire -----*/

void UserInput::handleOperateurTernaire(QString element)
{

    Litterale *lit1 = PILE.top();
    Litterale *lit2 = PILE.top();
    Litterale *lit3 = PILE.top();

    try
    {
        if (lit3 == nullptr)
            throw CompCeption("Erreur : Pas assez d'elements dans la pile pour l'operation : " + element);

        else if (element == "IFTE")
        {
            Litterale *res = lit2->IFTE(lit1, lit3);
            commandeInterpreteur(stripCommande(res->rawInput(), false));
        }
        else
            throw CompCeption("Erreur : L'operateur " + element + " a ete defini dans tabOpTernaire mais pas dans HandleOperateur");

        return;
    }
    catch (CompCeption e)
    {
        PILE.push(lit3);
        PILE.push(lit2);
        PILE.push(lit1);
        throw;
    }
}

/* ----- Opérateur de pile ----- */

void UserInput::handleOperateurPile(QString element)
{
    try
    {
        if (element == "CLEAR")
            PILE.CLEAR();

        else if (element == "DUP")
            PILE.DUP();

        else if (element == "DROP")
            PILE.DROP();

        else if (element == "SWAP")
            PILE.SWAP();

        else if (element == "UNDO")
            PILE.UNDO();

        else if (element == "REDO")
            PILE.REDO();

        else
            throw CompCeption("Erreur : L'operateur " + element + " a ete defini dans tabOpPile mais pas dans HandleOperateur");

        return;
    }
    catch (CompCeption e)
    {
        throw;
    }
}

bool UserInput::execute(QString commande)
{
    try
    {
        PILE.setMessage("");
        USERINPUT.commandeInterpreteur(USERINPUT.stripCommande(commande));
        PILE.manageMemory();
        PILE.modificationEtat();
        PILE.afficher();
        return true;
    }
    catch (CompCeption e)
    {
        PILE.setMessage(e.getMessage());
        PILE.modificationEtat();
        return false;
        //A voir si on veut afficher une pop-up d'erreur ou non
        //throw;
    }
}
