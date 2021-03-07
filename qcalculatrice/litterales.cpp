#include <cmath> // std::pow(), std::cos(), std::sin(), std::tan(), std::arccos(), std::arcsin(), std::arctan(), std::sqrt(), std::exp(), std::log()
#include "litterales.h"
#include "compception.h"
#include "macro.h"

/* --------------------------------------------------------------------------*/
/* ----- Litterale ----- */
/* --------------------------------------------------------------------------*/

//La litterale qui appelle ces methodes a pour valeur vrai car toutes litterales != litteraleEntiere(0) ==== true
Litterale *Litterale::AND(Litterale *lit)
{
    if (lit->getType() != entiere)
        return LitteraleFactory::createLitterale(1, entiere);
    else
        return lit->AND(this);
}
//C'est pas une litterale entiere qui appelle la methode donc c'est toujours vrai
Litterale *Litterale::OR(Litterale *) { return LitteraleFactory::createLitterale(1, entiere); }
//C'est pas une litterale entiere qui appelle la methode donc on renvoie toujours faux
Litterale *Litterale::NOT() { return FACTORY::createLitterale(0, entiere); }

/* --------------------------------------------------------------------------*/
/* ----- Litterale Rationnelle constructeur ----- */
/* --------------------------------------------------------------------------*/

LitteraleRationnelle::LitteraleRationnelle(int num, int denom) : Litterale(rationnelle), numerateur(num)
{
    if (denom != 0)
    {
        denominateur = denom;

        if (numerateur == 0)
        {
            denominateur = 1;
            return;
        }
        //Sert a rien je pense par construction c'est impossible
        if (denominateur == 0)
            throw CompCeption("Erreur : Le dénominateur est egale a 0.");

        //PGCD entre le numérateur et le dénominateur
        int a = numerateur, b = denominateur;
        //On ne travaille qu’avec des valeurs positives
        if (a < 0)
            a = -a;
        if (b < 0)
            b = -b;

        while (a != b)
        {
            if (a > b)
                a = a - b;
            else
                b = b - a;
        }
        //On divise le numerateur et le denominateur par le PGCD=a
        numerateur /= a;
        denominateur /= a;

        //Si le denominateur est négatif, on fait passer le signe - au denominateur
        if (denominateur < 0)
        {
            denominateur = -denominateur;
            numerateur = -numerateur;
        }
    }
    //Impossible d'arriver jusqu'ici car on doit diviser une litterale entiere par 0 avant. Si la construction échoue, il n'y a rien a nettoyer
    else
        throw CompCeption("Erreur : creation d'une litterale rationnelle avec un denominateur egale a 0.");
}

/* --------------------------------------------------------------------------*/
/* ----- Factory creation litterales ----- */
/* --------------------------------------------------------------------------*/

Litterale *LitteraleFactory::createLitterale(int v, typeLitterale t)
{
    if (t != entiere)
        throw CompCeption("Erreur : Type non coherent avec une litterale entiere.");
    return MANAGER.addLitterale(new LitteraleEntiere(v));
}

Litterale *LitteraleFactory::createLitterale(double v, typeLitterale t)
{
    if (t != reelle)
        throw CompCeption("Erreur : Type non coherent avec une litterale reelle.");
    return MANAGER.addLitterale(new LitteraleReelle(v));
}

Litterale *LitteraleFactory::createLitterale(int num, int denom, typeLitterale t)
{
    if (t != rationnelle)
        throw CompCeption("Erreur : Type non coherent avec une litterale rationelle.");
    Litterale *fraction = new LitteraleRationnelle(num, denom);
    if (static_cast<LitteraleRationnelle *>(fraction)->getDenominateur() == 1)
    {
        int entier = static_cast<LitteraleRationnelle *>(fraction)->getNumerateur();
        delete fraction;
        return MANAGER.addLitterale(new LitteraleEntiere(entier));
    }
    else
        return MANAGER.addLitterale(fraction);
}

Litterale *LitteraleFactory::createLitterale(QString s, typeLitterale t)
{
    if (t == expression)
        return MANAGER.addLitterale(new LitteraleExpression(s));
    if (t == programme)
        return MANAGER.addLitterale(new LitteraleProgramme(s));
    else
        throw CompCeption("Erreur : type non coherent avec une litterale expression ou programme.");
}

void LitteraleFactory::deleteLitterale(Litterale *lit)
{
    MANAGER.removeLitterale(lit);
}

/* --------------------------------------------------------------------------*/
/* ----- Litterale Manager  -----*/
/* --------------------------------------------------------------------------*/

LitteraleManager &LitteraleManager::donneInstance()
{
    static LitteraleManager singleton;
    return singleton;
}

LitteraleManager::~LitteraleManager()
{
    for (size_t i = 0; i < listeLitterales.size(); i++)
        delete listeLitterales[i];
}

Litterale *LitteraleManager::addLitterale(Litterale *lit)
{
    listeLitterales.push_back(lit);
    return listeLitterales.back();
}

void LitteraleManager::removeLitterale(Litterale *lit)
{
    for (size_t i = 0; i < listeLitterales.size(); i++)
    {
        if (listeLitterales[i] == lit)
        {
            delete listeLitterales[i];
            //static_cast en int car -Wsign-conversion renvoie une erreur !
            listeLitterales.erase(listeLitterales.begin() + i);
            return;
        }
    }
}

/* --- OPERATEURS --- */

/* --------------------------------------------------------------------------*/
/* ----- Litterale entiere ----- */
/* --------------------------------------------------------------------------*/

const QString LitteraleEntiere::rawInput() const {
    if (value < 0)
    {
        return QString::number(-value)+" NEG";
    }
    return QString::number(value);
}

Litterale *LitteraleEntiere::getCopie() const
{
    return FACTORY::createLitterale(value, entiere);
}

Litterale *LitteraleEntiere::operator+(Litterale *lit)
{
    if (lit->getType() == entiere)
        return FACTORY::createLitterale(static_cast<LitteraleEntiere *>(lit)->value + value, entiere);
    else
        return lit->operator+(this);
}

Litterale *LitteraleEntiere::operator-(Litterale *lit)
{
    lit->NEG();
    Litterale *res = lit->operator+(this);
    lit->NEG();
    return res;
}

Litterale *LitteraleEntiere::operator*(Litterale *lit)
{
    if (lit->getType() == entiere)
        return FACTORY::createLitterale(static_cast<LitteraleEntiere *>(lit)->value * value, entiere);
    else
        return lit->operator*(this);
}

Litterale *LitteraleEntiere::operator/(Litterale *lit)
{
    typeLitterale type = lit->getType();

    if (type == entiere)
    {
        if (static_cast<LitteraleEntiere *>(lit)->getValue() == 0)
            throw CompCeption("Erreur : Tentative de division par 0.");
        else if (value % static_cast<LitteraleEntiere *>(lit)->getValue() != 0)
        {
            return FACTORY::createLitterale(value, static_cast<LitteraleEntiere *>(lit)->getValue(), rationnelle);
        }
        else
        {
            return FACTORY::createLitterale(value / static_cast<LitteraleEntiere *>(lit)->getValue(), entiere);
        }
    }
    else if (type == reelle)
    {
        if (static_cast<LitteraleReelle *>(lit)->getValue() == 0)
            throw CompCeption("Erreur : Tentative de division par 0.");
        else
            return FACTORY::createLitterale(value / static_cast<LitteraleReelle *>(lit)->getValue(), reelle);
    }
    else if (type == rationnelle)
    {
        return FACTORY::createLitterale(value * static_cast<LitteraleRationnelle *>(lit)->getDenominateur(), static_cast<LitteraleRationnelle *>(lit)->getNumerateur(), rationnelle);
    }
    else if (type == atome || type == expression || type == programme)
        throw CompCeption("Erreur : Impossible de diviser par une expression ou un programme.");
    else
        throw CompCeption("Erreur : Division non defini entre une litterale entiere et le nouveau type créé.");
}

Litterale *LitteraleEntiere::DIV(Litterale *lit)
{

    if (lit->getType() != entiere)
        throw CompCeption("Erreur : Operateur DIV uniquement valide avec une litterale entiere.");

    else if (static_cast<LitteraleEntiere *>(lit)->getValue() == 0)
        throw CompCeption("Erreur : Division par zero.");

    else
    {
        int val = (this->getValue()) / (static_cast<LitteraleEntiere *>(lit)->getValue());
        return FACTORY::createLitterale(val, entiere);
    }
}

Litterale *LitteraleEntiere::MOD(Litterale *lit)
{
    if (lit->getType() != entiere)
        throw CompCeption("Erreur : Operateur MOD uniquement valide avec une litterale entiere.");
    if (static_cast<LitteraleEntiere *>(lit)->getValue() == 0)
        throw CompCeption("Erreur : Divison par zero.");
    return FACTORY::createLitterale(value % static_cast<LitteraleEntiere *>(lit)->value, entiere);
}

Litterale *LitteraleEntiere::operator==(Litterale *lit)
{
    if (lit->getType() == entiere)
        return FACTORY::createLitterale(value == static_cast<LitteraleEntiere *>(lit)->getValue(), entiere);
    else
        return lit->operator==(this);
}

Litterale *LitteraleEntiere::operator!=(Litterale *lit)
{
    if (lit->getType() == entiere)
        return FACTORY::createLitterale(value != static_cast<LitteraleEntiere *>(lit)->getValue(), entiere);
    else
        return lit->operator!=(this);
}

Litterale *LitteraleEntiere::operator<=(Litterale *lit)
{
    if (lit->getType() == entiere)
        return FACTORY::createLitterale(value <= static_cast<LitteraleEntiere *>(lit)->getValue(), entiere);
    else
        return lit->operator>=(this);
}

Litterale *LitteraleEntiere::operator>=(Litterale *lit)
{
    if (lit->getType() == entiere)
        return FACTORY::createLitterale(value >= static_cast<LitteraleEntiere *>(lit)->getValue(), entiere);
    else
        return lit->operator<=(this);
}

Litterale *LitteraleEntiere::operator<(Litterale *lit)
{
    if (lit->getType() == entiere)
        return FACTORY::createLitterale(value < static_cast<LitteraleEntiere *>(lit)->getValue(), entiere);
    else
        return lit->operator>(this);
}

Litterale *LitteraleEntiere::operator>(Litterale *lit)
{
    if (lit->getType() == entiere)
        return FACTORY::createLitterale(value > static_cast<LitteraleEntiere *>(lit)->getValue(), entiere);
    else
        return lit->operator<(this);
}

Litterale *LitteraleEntiere::AND(Litterale *lit)
{
    if ((this->value != 0 && static_cast<LitteraleEntiere *>(lit)->getValue() != 0) || (this->value != 0 && lit->getType() != entiere))
        return FACTORY::createLitterale(1, entiere);

    else if (this->value == 0 && static_cast<LitteraleEntiere *>(lit)->getValue() == 0)
        return FACTORY::createLitterale(0, entiere);

    else
        return FACTORY::createLitterale(0, entiere);
}

Litterale *LitteraleEntiere::OR(Litterale *lit)
{
    if (this->value != 0 || static_cast<LitteraleEntiere *>(lit)->getValue() != 0 || lit->getType() != entiere)
        return FACTORY::createLitterale(1, entiere);

    else
        return FACTORY::createLitterale(0, entiere);
}

Litterale *LitteraleEntiere::NOT()
{
    if (this->value == 1)
        return FACTORY::createLitterale(0, entiere);
    return FACTORY::createLitterale(1, entiere);
}

Litterale *LitteraleEntiere::POW(Litterale *lit)
{

    typeLitterale type = lit->getType();

    if (type == entiere)
    {
        LitteraleEntiere *temp = static_cast<LitteraleEntiere *>(lit);
        double res = std::pow(this->value, temp->value);
        if (res == static_cast<int>(res))
            return FACTORY::createLitterale(static_cast<int>(res), entiere);
        else
            return FACTORY::createLitterale(res, reelle);
    }
    else if (type == reelle)
    {
        LitteraleReelle *temp = static_cast<LitteraleReelle *>(lit);
        double res = std::pow(this->value, temp->getValue());
        if (res == static_cast<int>(res))
            return FACTORY::createLitterale(static_cast<int>(res), entiere);
        else
            return FACTORY::createLitterale(res, reelle);
    }
    else if (type == rationnelle)
    {
        LitteraleRationnelle *temp = static_cast<LitteraleRationnelle *>(lit);
        double res = std::pow(this->value, (static_cast<double>(temp->getNumerateur()) / static_cast<double>(temp->getDenominateur())));
        if (res == static_cast<int>(res))
            return FACTORY::createLitterale(static_cast<int>(res), entiere);
        else
            return FACTORY::createLitterale(res, reelle);
    }
    else if (type == atome || type == expression || type == programme)
        throw CompCeption("Erreur : Operation impossible, l'argument de POW ne peut pas être une expression ou un programme.");
    else
        throw CompCeption("Erreur : POW non defini entre une litterale entiere et le nouveau type créé.");
}

Litterale *LitteraleEntiere::SIN()
{
    double res = sin(this->value);
    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleEntiere::COS()
{
    double res = cos(this->value);
    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleEntiere::TAN()
{
    double res = tan(this->value);
    if (std::isnan(res))
        throw CompCeption("Erreur : Operateur TAN sur une valeur qui n'est pas dans le domaine de definition de la fonction");

    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleEntiere::ARCSIN()
{
    double res = asin(this->value);
    if (std::isnan(res))
        throw CompCeption("Erreur : Operateur ARCSIN sur une valeur qui n'est pas dans le domaine de definition de la fonction.");

    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleEntiere::ARCCOS()
{
    double res = acos(this->value);
    if (std::isnan(res))
        throw CompCeption("Erreur : Operateur ARCCOS sur une valeur qui n'est pas dans le domaine de definition de la fonction.");

    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleEntiere::ARCTAN()
{
    double res = atan(this->value);
    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleEntiere::SQRT()
{
    if (this->value < 0)
        throw CompCeption("Erreur : Operateur SQRT sur un nombre negatif.");

    double res = sqrt(this->value);
    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleEntiere::EXP()
{
    double res = exp(this->value);
    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleEntiere::LN()
{
    if (this->value <= 0)
        throw CompCeption("Erreur : Operateur LN sur un nombre negatif.");
    double res = log(this->value);
    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

/* --------------------------------------------------------------------------*/
/* ----- Litteralle Reelle ----- */
/* --------------------------------------------------------------------------*/

const QString LitteraleReelle::rawInput() const {
    if (value < 0)
    {
        return QString::number(-value)+" NEG";
    }
    return QString::number(value);
}

Litterale *LitteraleReelle::getCopie() const
{
    return FACTORY::createLitterale(value, reelle);
}

Litterale *LitteraleReelle::operator+(Litterale *lit)
{
    typeLitterale type = lit->getType();

    if (type == reelle)
        return FACTORY::createLitterale(static_cast<LitteraleReelle *>(lit)->value + value, reelle);
    else if (type == entiere)
        return FACTORY::createLitterale(static_cast<LitteraleEntiere *>(lit)->getValue() + value, reelle);
    return lit->operator+(this);
}

Litterale *LitteraleReelle::operator-(Litterale *lit)
{
    lit->NEG();
    Litterale *res = lit->operator+(this);
    lit->NEG();
    return res;
}

Litterale *LitteraleReelle::operator*(Litterale *lit)
{
    typeLitterale type = lit->getType();

    if (type == reelle)
        return FACTORY::createLitterale(static_cast<LitteraleReelle *>(lit)->value * value, reelle);
    else if (type == entiere)
        return FACTORY::createLitterale(static_cast<LitteraleEntiere *>(lit)->getValue() * value, reelle);
    else
        return lit->operator*(this);
}

Litterale *LitteraleReelle::operator/(Litterale *lit)
{
    typeLitterale type = lit->getType();

    if (type == entiere)
    {
        if (static_cast<LitteraleEntiere *>(lit)->getValue() == 0)
            throw CompCeption("Erreur : Tentative de division par 0.");
        else
            return FACTORY::createLitterale(value / static_cast<LitteraleEntiere *>(lit)->getValue(), reelle);
    }
    else if (type == reelle)
    {
        if (static_cast<LitteraleReelle *>(lit)->getValue() == 0)
            throw CompCeption("Erreur : Tentative de division par 0.");
        else
            return FACTORY::createLitterale(value / static_cast<LitteraleReelle *>(lit)->getValue(), reelle);
    }
    else if (type == rationnelle)
    {
        return FACTORY::createLitterale(static_cast<LitteraleRationnelle *>(lit)->getNumerateur() / (static_cast<LitteraleRationnelle *>(lit)->getDenominateur() * value), reelle);
    }
    else if (type == atome || type == expression || type == programme)
        throw CompCeption("Erreur : Impossible de diviser par une expression ou un programme.");
    else
        throw CompCeption("Erreur : Division non defini entre une litterale reelle et le nouveau type créé.");
}

Litterale *LitteraleReelle::operator==(Litterale *lit)
{
    typeLitterale type = lit->getType();

    if (type == entiere)
        return FACTORY::createLitterale(value == static_cast<LitteraleEntiere *>(lit)->getValue(), entiere);
    else if (type == reelle)
        return FACTORY::createLitterale(value == static_cast<LitteraleReelle *>(lit)->value, entiere);
    else
        return lit->operator==(this);
}

Litterale *LitteraleReelle::operator!=(Litterale *lit)
{
    typeLitterale type = lit->getType();

    if (type == entiere)
        return FACTORY::createLitterale(value != static_cast<LitteraleEntiere *>(lit)->getValue(), entiere);
    else if (type == reelle)
        return FACTORY::createLitterale(value == static_cast<LitteraleReelle *>(lit)->value, entiere);
    else
        return lit->operator!=(this);
}

Litterale *LitteraleReelle::operator<=(Litterale *lit)
{
    typeLitterale type = lit->getType();

    if (type == entiere)
        return FACTORY::createLitterale(value <= static_cast<LitteraleEntiere *>(lit)->getValue(), entiere);
    else if (type == reelle)
        return FACTORY::createLitterale(value <= static_cast<LitteraleReelle *>(lit)->getValue(), entiere);
    else
        return lit->operator>=(this);
}

Litterale *LitteraleReelle::operator>=(Litterale *lit)
{
    typeLitterale type = lit->getType();

    if (type == entiere)
        return FACTORY::createLitterale(value >= static_cast<LitteraleEntiere *>(lit)->getValue(), entiere);
    else if (type == reelle)
        return FACTORY::createLitterale(value >= static_cast<LitteraleReelle *>(lit)->getValue(), entiere);
    else
        return lit->operator<=(this);
}

Litterale *LitteraleReelle::operator<(Litterale *lit)
{
    typeLitterale type = lit->getType();

    if (type == entiere)
        return FACTORY::createLitterale(value < static_cast<LitteraleEntiere *>(lit)->getValue(), entiere);
    else if (type == reelle)
        return FACTORY::createLitterale(value < static_cast<LitteraleReelle *>(lit)->getValue(), entiere);
    else
        return lit->operator>(this);
}

Litterale *LitteraleReelle::operator>(Litterale *lit)
{
    typeLitterale type = lit->getType();

    if (type == entiere)
        return FACTORY::createLitterale(value > static_cast<LitteraleEntiere *>(lit)->getValue(), entiere);
    else if (type == reelle)
        return FACTORY::createLitterale(value > static_cast<LitteraleReelle *>(lit)->getValue(), entiere);
    else
        return lit->operator<(this);
}

Litterale *LitteraleReelle::POW(Litterale *lit)
{

    typeLitterale type = lit->getType();

    if (type == entiere)
    {
        LitteraleEntiere *temp = static_cast<LitteraleEntiere *>(lit);
        double res = std::pow(this->value, temp->getValue());
        if (res == static_cast<int>(res))
            return FACTORY::createLitterale(static_cast<int>(res), entiere);
        else
            return FACTORY::createLitterale(res, reelle);
    }
    else if (type == reelle)
    {
        LitteraleReelle *temp = static_cast<LitteraleReelle *>(lit);
        double res = std::pow(this->value, temp->value);
        if (res == static_cast<int>(res))
            return FACTORY::createLitterale(static_cast<int>(res), entiere);
        else
            return FACTORY::createLitterale(res, reelle);
    }
    else if (type == rationnelle)
    {
        LitteraleRationnelle *temp = static_cast<LitteraleRationnelle *>(lit);
        double res = std::pow(this->value, (static_cast<double>(temp->getNumerateur()) / static_cast<double>(temp->getDenominateur())));
        if (res == static_cast<int>(res))
            return FACTORY::createLitterale(static_cast<int>(res), entiere);
        else
            return FACTORY::createLitterale(res, reelle);
    }
    else if (type == atome || type == expression || type == programme)
        throw CompCeption("Erreur : Operation impossible, l'argument de POW ne peut pas être une expression ou un programme.");
    else
        throw CompCeption("Erreur : POW non defini entre une litterale entiere et le nouveau type créé.");
}

Litterale *LitteraleReelle::SIN()
{
    double res = sin(this->value);
    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleReelle::COS()
{
    double res = cos(this->value);
    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleReelle::TAN()
{
    double res = tan(this->value);
    if (std::isnan(res))
        throw CompCeption("Erreur : Operateur TAN sur une valeur qui n'est pas dans le domaine de definition de la fonction");

    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleReelle::ARCSIN()
{
    double res = asin(this->value);
    if (std::isnan(res))
        throw CompCeption("Erreur : Operateur ARCSIN sur une valeur qui n'est pas dans le domaine de definition de la fonction.");

    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleReelle::ARCCOS()
{
    double res = acos(this->value);
    if (std::isnan(res))
        throw CompCeption("Erreur : Operateur ARCCOS sur une valeur qui n'est pas dans le domaine de definition de la fonction.");

    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleReelle::ARCTAN()
{
    double res = atan(this->value);
    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleReelle::SQRT()
{
    if (this->value < 0)
        throw CompCeption("Erreur : Operateur SQRT sur un nombre negatif.");

    double res = sqrt(this->value);
    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleReelle::EXP()
{
    double res = exp(this->value);
    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleReelle::LN()
{
    if (this->value <= 0)
        throw CompCeption("Erreur : Operateur LN sur un nombre negatif.");
    double res = log(this->value);
    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

/* --------------------------------------------------------------------------*/
/* ----- Litterale Rationnelle ----- */
/* --------------------------------------------------------------------------*/

const QString LitteraleRationnelle::rawInput() const {
    //Quand la fraction est negative, seul le numérateur est négatif
    if (numerateur < 0)
    {
        return QString::number(-numerateur)+" "+QString::number(denominateur)+" / NEG";
    }
    return QString::number(numerateur)+" "+QString::number(denominateur)+" /";
}

Litterale *LitteraleRationnelle::getCopie() const
{
    return FACTORY::createLitterale(numerateur, denominateur, rationnelle);
}

Litterale *LitteraleRationnelle::operator+(Litterale *lit)
{
    typeLitterale type = lit->getType();
    if (type == entiere)
        return FACTORY::createLitterale(numerateur + (static_cast<LitteraleEntiere *>(lit)->getValue() * denominateur), denominateur, rationnelle);
    if (type == reelle)
        return FACTORY::createLitterale( ( static_cast<double>(numerateur) / static_cast<double>(denominateur) ) + static_cast<LitteraleReelle *>(lit)->getValue(), reelle );
    if (type == rationnelle)
    {
        LitteraleRationnelle *argument = static_cast<LitteraleRationnelle *>(lit);
        return FACTORY::createLitterale(numerateur * argument->denominateur + argument->numerateur * denominateur, denominateur * argument->denominateur, rationnelle);
    }
    return lit->operator+(this);
}

Litterale *LitteraleRationnelle::operator-(Litterale *lit)
{
    lit->NEG();
    Litterale *res = lit->operator+(this);
    lit->NEG();
    return res;
}

Litterale *LitteraleRationnelle::operator*(Litterale *lit)
{
    typeLitterale type = lit->getType();
    if (type == entiere)
        return FACTORY::createLitterale(numerateur * static_cast<LitteraleEntiere *>(lit)->getValue(), denominateur, rationnelle);
    if (type == reelle)
        return FACTORY::createLitterale(numerateur * static_cast<LitteraleReelle *>(lit)->getValue() / denominateur, reelle);
    if (type == rationnelle)
    {
        LitteraleRationnelle *argument = static_cast<LitteraleRationnelle *>(lit);
        return FACTORY::createLitterale(numerateur * argument->numerateur, denominateur * argument->denominateur, rationnelle);
    }

    return lit->operator*(this);
}

Litterale *LitteraleRationnelle::operator/(Litterale *lit)
{
    typeLitterale type = lit->getType();

    if (type == entiere)
    {
        if (static_cast<LitteraleEntiere *>(lit)->getValue() == 0)
            throw CompCeption("Erreur : Tentative de division par 0.");
        else
            return FACTORY::createLitterale(numerateur, static_cast<LitteraleEntiere *>(lit)->getValue() * denominateur, rationnelle);
    }
    else if (type == reelle)
    {
        if (static_cast<LitteraleReelle *>(lit)->getValue() == 0)
            throw CompCeption("Erreur : Tentative de division par 0.");
        else
            return FACTORY::createLitterale(numerateur / (denominateur * static_cast<LitteraleReelle *>(lit)->getValue()), reelle);
    }
    else if (type == rationnelle)
    {
        return FACTORY::createLitterale(numerateur * static_cast<LitteraleRationnelle *>(lit)->denominateur, denominateur * static_cast<LitteraleRationnelle *>(lit)->numerateur, rationnelle);
    }
    else if (type == atome || type == expression || type == programme)
        throw CompCeption("Erreur : Impossible de diviser par une expression ou un programme.");
    else
        throw CompCeption("Erreur : Division non defini entre une litterale rationnelle et le nouveau type créé");
}

Litterale *LitteraleRationnelle::operator==(Litterale *lit)
{
    typeLitterale type = lit->getType();

    if (type == entiere)
        return FACTORY::createLitterale(static_cast<LitteraleEntiere *>(lit)->getValue() * denominateur == numerateur, entiere);
    else if (type == reelle)
        return FACTORY::createLitterale(static_cast<LitteraleReelle *>(lit)->getValue() * denominateur == numerateur, entiere);
    else if (type == rationnelle)
        return FACTORY::createLitterale(static_cast<LitteraleRationnelle *>(lit)->numerateur == numerateur && static_cast<LitteraleRationnelle *>(lit)->denominateur == denominateur, entiere);
    else
        return lit->operator==(this);
}

Litterale *LitteraleRationnelle::operator!=(Litterale *lit)
{
    typeLitterale type = lit->getType();

    if (type == entiere)
        return FACTORY::createLitterale(static_cast<LitteraleEntiere *>(lit)->getValue() * denominateur != numerateur, entiere);
    else if (type == reelle)
        return FACTORY::createLitterale(static_cast<LitteraleReelle *>(lit)->getValue() * denominateur != numerateur, entiere);
    else if (type == rationnelle)
        return FACTORY::createLitterale(static_cast<LitteraleRationnelle *>(lit)->numerateur != numerateur && static_cast<LitteraleRationnelle *>(lit)->denominateur == denominateur, entiere);
    else
        return lit->operator!=(this);
}

Litterale *LitteraleRationnelle::operator<=(Litterale *lit)
{
    typeLitterale type = lit->getType();

    if (type == entiere)
        return FACTORY::createLitterale(static_cast<LitteraleEntiere *>(lit)->getValue() * denominateur <= numerateur, entiere);
    else if (type == reelle)
        return FACTORY::createLitterale(static_cast<LitteraleReelle *>(lit)->getValue() * denominateur <= numerateur, entiere);
    else if (type == rationnelle)
    {
        LitteraleRationnelle *argument = static_cast<LitteraleRationnelle *>(lit);
        return FACTORY::createLitterale(this->numerateur * argument->denominateur <= argument->numerateur * denominateur, entiere);
    }
    else
        return lit->operator>=(this);
}

Litterale *LitteraleRationnelle::operator>=(Litterale *lit)
{
    typeLitterale type = lit->getType();

    if (type == entiere)
        return FACTORY::createLitterale(static_cast<LitteraleEntiere *>(lit)->getValue() * denominateur >= numerateur, entiere);
    else if (type == reelle)
        return FACTORY::createLitterale(static_cast<LitteraleReelle *>(lit)->getValue() * denominateur >= numerateur, entiere);
    else if (type == rationnelle)
    {
        LitteraleRationnelle *argument = static_cast<LitteraleRationnelle *>(lit);
        return FACTORY::createLitterale(this->numerateur * argument->denominateur >= argument->numerateur * denominateur, entiere);
    }
    else
        return lit->operator<=(this);
}

Litterale *LitteraleRationnelle::operator<(Litterale *lit)
{
    typeLitterale type = lit->getType();

    if (type == entiere)
        return FACTORY::createLitterale(static_cast<LitteraleEntiere *>(lit)->getValue() * denominateur < numerateur, entiere);
    else if (type == reelle)
        return FACTORY::createLitterale(static_cast<LitteraleReelle *>(lit)->getValue() * denominateur < numerateur, entiere);
    else if (type == rationnelle)
    {
        LitteraleRationnelle *argument = static_cast<LitteraleRationnelle *>(lit);
        return FACTORY::createLitterale(this->numerateur * argument->denominateur < argument->numerateur * denominateur, entiere);
    }
    else
        return lit->operator>(this);
}

Litterale *LitteraleRationnelle::operator>(Litterale *lit)
{
    typeLitterale type = lit->getType();

    if (type == entiere)
        return FACTORY::createLitterale(static_cast<LitteraleEntiere *>(lit)->getValue() * denominateur > numerateur, entiere);
    else if (type == reelle)
        return FACTORY::createLitterale(static_cast<LitteraleReelle *>(lit)->getValue() * denominateur > numerateur, entiere);
    else if (type == rationnelle)
    {
        LitteraleRationnelle *argument = static_cast<LitteraleRationnelle *>(lit);
        return FACTORY::createLitterale(this->numerateur * argument->denominateur > argument->numerateur * denominateur, entiere);
    }
    else
        return lit->operator<(this);
}

Litterale *LitteraleRationnelle::NUM()
{
    return FACTORY::createLitterale(this->numerateur, entiere);
}

Litterale *LitteraleRationnelle::DEN()
{
    return FACTORY::createLitterale(this->denominateur, entiere);
}

Litterale *LitteraleRationnelle::POW(Litterale *lit)
{

    typeLitterale type = lit->getType();
    double argument = static_cast<double>(this->numerateur) / static_cast<double>(this->denominateur);

    if (type == entiere)
    {
        LitteraleEntiere *temp = static_cast<LitteraleEntiere *>(lit);
        double res = std::pow(argument, temp->getValue());
        if (res == static_cast<int>(res))
            return FACTORY::createLitterale(static_cast<int>(res), entiere);
        else
            return FACTORY::createLitterale(res, reelle);
    }
    else if (type == reelle)
    {
        LitteraleReelle *temp = static_cast<LitteraleReelle *>(lit);
        double res = std::pow(argument, temp->getValue());
        if (res == static_cast<int>(res))
            return FACTORY::createLitterale(static_cast<int>(res), entiere);
        else
            return FACTORY::createLitterale(res, reelle);
    }
    else if (type == rationnelle)
    {
        LitteraleRationnelle *temp = static_cast<LitteraleRationnelle *>(lit);
        double res = std::pow(argument, (static_cast<double>(temp->numerateur) / static_cast<double>(temp->numerateur)));
        if (res == static_cast<int>(res))
            return FACTORY::createLitterale(static_cast<int>(res), entiere);
        else
            return FACTORY::createLitterale(res, reelle);
    }
    else if (type == atome || type == expression || type == programme)
        throw CompCeption("Erreur : Operation impossible, l'argument de POW ne peut pas être une expression ou un programme.");
    else
        throw CompCeption("Erreur : POW non defini entre une reelle entiere et le nouveau type créé.");
}

Litterale *LitteraleRationnelle::SIN()
{
    double res = std::sin(static_cast<double>(this->numerateur) / static_cast<double>(this->denominateur));
    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleRationnelle::COS()
{
    double res = std::cos(static_cast<double>(this->numerateur) / static_cast<double>(this->denominateur));
    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleRationnelle::TAN()
{
    double res = tan(static_cast<double>(this->numerateur) / static_cast<double>(this->denominateur));
    if (std::isnan(res))
        throw CompCeption("Erreur : Operateur TAN sur une valeur qui n'est pas dans le domaine de definition de la fonction");

    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleRationnelle::ARCSIN()
{
    double res = asin(static_cast<double>(this->numerateur) / static_cast<double>(this->denominateur));
    if (std::isnan(res))
        throw CompCeption("Erreur : Operateur ARCSIN sur une valeur qui n'est pas dans le domaine de definition de la fonction.");

    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleRationnelle::ARCCOS()
{
    double res = acos(static_cast<double>(this->numerateur) / static_cast<double>(this->denominateur));
    if (std::isnan(res))
        throw CompCeption("Erreur : Operateur ARCCOS sur une valeur qui n'est pas dans le domaine de definition de la fonction.");

    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleRationnelle::ARCTAN()
{
    double res = atan(static_cast<double>(this->numerateur) / static_cast<double>(this->denominateur));

    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleRationnelle::SQRT()
{
    double argument = static_cast<double>(this->numerateur) / static_cast<double>(this->denominateur);
    if (argument < 0)
        throw CompCeption("Erreur : Operateur SQRT sur un nombre negatif.");

    double res = std::sqrt(argument);
    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleRationnelle::EXP()
{
    double res = std::exp(static_cast<double>(this->numerateur) / static_cast<double>(this->denominateur));
    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

Litterale *LitteraleRationnelle::LN()
{
    double argument = static_cast<double>(this->numerateur) / static_cast<double>(this->denominateur);
    if (argument <= 0)
        throw CompCeption("Erreur : Operateur LN sur un nombre negatif.");
    double res = std::log(argument);
    if (res == static_cast<int>(res))
        return FACTORY::createLitterale(static_cast<int>(res), entiere);
    else
        return FACTORY::createLitterale(res, reelle);
}

/* --------------------------------------------------------------------------*/
/* ----- Litterale Programme ----- */
/* --------------------------------------------------------------------------*/

Litterale *LitteraleProgramme::getCopie() const
{
    return FACTORY::createLitterale(program, programme);
}

Litterale *LitteraleProgramme::EVAL()
{
    return this;
}

Litterale *LitteraleProgramme::IFT(Litterale *lit)
{
    if (lit->getType() != entiere || static_cast<LitteraleEntiere *>(lit)->getValue() != 0)
        return this->EVAL();
    else
        return nullptr;
}

Litterale *LitteraleProgramme::IFTE(Litterale *lit1, Litterale *lit2)
{
    if (lit1->getType() != expression && lit1->getType() != programme)
        throw CompCeption("Erreur : IFTE prend en parametre deux programmes ou expressions et un test logique.");
    if (lit2->getType() != entiere || static_cast<LitteraleEntiere *>(lit2)->getValue() != 0)
        return this->EVAL();
    else
        return lit1->EVAL();
}

/* --------------------------------------------------------------------------*/
/* ----- Litterale Expression ----- */
/* --------------------------------------------------------------------------*/

Litterale *LitteraleExpression::getCopie() const
{
    return FACTORY::createLitterale(express, expression);
}

Litterale *LitteraleExpression::EVAL()
{
    if (this->target == nullptr)
        throw CompCeption("Erreur : Aucune litterale associee a cette expression.");
    return this->target;
}

Litterale *LitteraleExpression::IFT(Litterale *lit)
{
    if (lit->getType() != entiere || static_cast<LitteraleEntiere *>(lit)->getValue() != 0)
        return this->EVAL();
    else
        return nullptr;
}

Litterale *LitteraleExpression::IFTE(Litterale *lit1, Litterale *lit2)
{
    if (lit1->getType() != expression && lit1->getType() != programme)
        throw CompCeption("Erreur : IFTE prend en parametre deux programmes ou expressions et un test logique.");
    if (lit2->getType() != entiere || static_cast<LitteraleEntiere *>(lit2)->getValue() != 0)
        return this->EVAL();
    else
        return lit1->EVAL();
}

void LitteraleExpression::STO(Litterale *lit)
{
    if (lit->getType() == expression || lit->getType() == atome)
        throw CompCeption("Erreur : Impossible de stocker une litterale expression dans une expression.");
    this->target = lit;
}

void LitteraleExpression::FORGET()
{
    target = nullptr;
}
