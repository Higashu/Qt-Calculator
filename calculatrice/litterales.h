#pragma once

#include <string> //A retirer sur Qt
//#include <QString>
#include <vector> //A retirer sur Qt
//#include <QVector>
#include "compception.h"

enum typeLitterale
{
    entiere,
    rationnelle,
    reelle,
    atome,
    expression,
    programme
};

/* --------------------------------------------------------------------------*/
/*----- Interface Litterale ----- */
/* --------------------------------------------------------------------------*/

class Litterale
{
    friend class LitteraleFactory;
    friend class LitteraleManager;

protected:
    typeLitterale typeLit;
    Litterale(typeLitterale t) : typeLit(t) {}
    virtual ~Litterale() = default;

public:
    typeLitterale getType() const { return typeLit; }
    virtual const std::string toString() const = 0;
    //Renvoie la commande pour créer la litterale en question, exception faite pour les programmes
    virtual const std::string rawInput() const = 0;
    virtual Litterale *getCopie() const = 0;
    virtual Litterale *NEG() = 0;
    virtual Litterale *operator+(Litterale *) = 0;
    virtual Litterale *operator-(Litterale *) = 0;
    virtual Litterale *operator*(Litterale *) = 0;
    virtual Litterale *operator/(Litterale *) = 0;
    //Par défaut renvoie une erreur, car l'operateur marche qu'avec les litterales entieres
    virtual Litterale *DIV(Litterale *) { throw CompCeption("Erreur : Operateur DIV uniquement valide avec une litterale entiere."); }
    virtual Litterale *MOD(Litterale *) { throw CompCeption("Erreur : Operateur MOD uniquement valide avec une litterale entiere."); }
    virtual Litterale *operator==(Litterale *) = 0;
    virtual Litterale *operator!=(Litterale *) = 0;
    virtual Litterale *operator<=(Litterale *) = 0;
    virtual Litterale *operator>=(Litterale *) = 0;
    virtual Litterale *operator<(Litterale *) = 0;
    virtual Litterale *operator>(Litterale *) = 0;
    virtual Litterale *AND(Litterale *lit);
    virtual Litterale *OR(Litterale *lit);
    virtual Litterale *NOT();
    virtual Litterale *EVAL() { throw CompCeption("Erreur : Operateur EVAL uniquement valide avec une expression ou un programme."); }
    virtual Litterale *IFT(Litterale *) { throw CompCeption("Erreur : Operateur IFT uniquement valide avec une expression ou un programme."); }
    virtual Litterale *IFTE(Litterale *, Litterale *) { throw CompCeption("Erreur : Operateur IFTE uniquement valide avec une expression ou un programme."); }
    virtual void STO(Litterale *) { throw CompCeption("Erreur : Operateur STO uniquement valide avec une expression."); }
    virtual Litterale *NUM() { throw CompCeption("Erreur : Operateur NUM uniquement valide avec une litterale rationnelle."); }
    virtual Litterale *DEN() { throw CompCeption("Erreur : Operateur DEN uniquement valide avec une litterale rationnelle."); }
    virtual Litterale *POW(Litterale *) = 0;
    virtual Litterale *SIN() = 0;
    virtual Litterale *COS() = 0;
    virtual Litterale *TAN() = 0;
    virtual Litterale *ARCSIN() = 0;
    virtual Litterale *ARCCOS() = 0;
    virtual Litterale *ARCTAN() = 0;
    virtual Litterale *SQRT() = 0;
    virtual Litterale *EXP() = 0;
    virtual Litterale *LN() = 0;
    virtual void FORGET() { throw CompCeption("Erreur : Operateur FORGET uniquement valide avec une expression."); }
};

/* --------------------------------------------------------------------------*/
/*----- Litterale Entiere ----- */
/* --------------------------------------------------------------------------*/

class LitteraleEntiere : public Litterale
{
    friend class LitteraleFactory;
    friend class LitteraleManager;

private:
    int value;

private:
    LitteraleEntiere(int v) : Litterale(entiere), value(v){};
    ~LitteraleEntiere() = default;

public:
    const std::string toString() const override { return std::to_string(value); } //QString::number(value)
    const std::string rawInput() const override; //QString::number(value)
    int getValue() const { return value; }
    Litterale *getCopie() const override;
    Litterale *NEG() override
    {
        value = -value;
        return this;
    }
    Litterale *operator+(Litterale *) override;
    Litterale *operator-(Litterale *) override;
    Litterale *operator*(Litterale *) override;
    Litterale *operator/(Litterale *) override;
    Litterale *DIV(Litterale *) override;
    Litterale *MOD(Litterale *) override;
    Litterale *operator==(Litterale *) override;
    Litterale *operator!=(Litterale *) override;
    Litterale *operator<=(Litterale *) override;
    Litterale *operator>=(Litterale *) override;
    Litterale *operator<(Litterale *) override;
    Litterale *operator>(Litterale *) override;
    Litterale *AND(Litterale *) override;
    Litterale *OR(Litterale *) override;
    Litterale *NOT() override;
    Litterale *POW(Litterale *) override;
    Litterale *SIN() override;
    Litterale *COS() override;
    Litterale *TAN() override;
    Litterale *ARCSIN() override;
    Litterale *ARCCOS() override;
    Litterale *ARCTAN() override;
    Litterale *SQRT() override;
    Litterale *EXP() override;
    Litterale *LN() override;
};

/* --------------------------------------------------------------------------*/
/*----- Litterale Reelle ----- */
/* --------------------------------------------------------------------------*/

class LitteraleReelle : public Litterale
{
    friend class LitteraleFactory;
    friend class LitteraleManager;

private:
    double value;

private:
    LitteraleReelle(double v) : Litterale(reelle), value(v){};
    ~LitteraleReelle() = default;

public:
    const std::string toString() const override { return std::to_string(value); } //QString::number(value)
    const std::string rawInput() const override;
    double getValue() const { return value; }
    Litterale *getCopie() const override;
    Litterale *NEG() override
    {
        value = -value;
        return this;
    }
    Litterale *operator+(Litterale *) override;
    Litterale *operator-(Litterale *) override;
    Litterale *operator*(Litterale *) override;
    Litterale *operator/(Litterale *) override;
    Litterale *operator==(Litterale *) override;
    Litterale *operator!=(Litterale *) override;
    Litterale *operator<=(Litterale *) override;
    Litterale *operator>=(Litterale *) override;
    Litterale *operator<(Litterale *) override;
    Litterale *operator>(Litterale *) override;
    Litterale *POW(Litterale *) override;
    Litterale *SIN() override;
    Litterale *COS() override;
    Litterale *TAN() override;
    Litterale *ARCSIN() override;
    Litterale *ARCCOS() override;
    Litterale *ARCTAN() override;
    Litterale *SQRT() override;
    Litterale *EXP() override;
    Litterale *LN() override;
};

/* --------------------------------------------------------------------------*/
/*----- Litterale Rationnelle ----- */
/* --------------------------------------------------------------------------*/

class LitteraleRationnelle : public Litterale
{
    friend class LitteraleFactory;
    friend class LitteraleManager;

private:
    int numerateur;
    int denominateur;

private:
    LitteraleRationnelle(int num, int denom);
    ~LitteraleRationnelle() = default;

public:
    const std::string toString() const override { return std::to_string(numerateur) + "/" + std::to_string(denominateur); }        //QString::number(denominateur)
    const std::string rawInput() const override;
    int getNumerateur() const { return numerateur; }
    int getDenominateur() const { return denominateur; }
    Litterale *getCopie() const override;
    Litterale *NEG() override
    {
        numerateur = -numerateur;
        return this;
    }
    Litterale *operator+(Litterale *) override;
    Litterale *operator-(Litterale *) override;
    Litterale *operator*(Litterale *) override;
    Litterale *operator/(Litterale *) override;
    Litterale *operator==(Litterale *) override;
    Litterale *operator!=(Litterale *) override;
    Litterale *operator<=(Litterale *) override;
    Litterale *operator>=(Litterale *) override;
    Litterale *operator<(Litterale *) override;
    Litterale *operator>(Litterale *) override;
    Litterale *NUM() override;
    Litterale *DEN() override;
    Litterale *POW(Litterale *) override;
    Litterale *SIN() override;
    Litterale *COS() override;
    Litterale *TAN() override;
    Litterale *ARCSIN() override;
    Litterale *ARCCOS() override;
    Litterale *ARCTAN() override;
    Litterale *SQRT() override;
    Litterale *EXP() override;
    Litterale *LN() override;
};

/* --------------------------------------------------------------------------*/
/*----- Litterale Programme ----- */
/* --------------------------------------------------------------------------*/

class LitteraleProgramme : public Litterale
{
    friend class LitteraleFactory;
    friend class LitteraleManager;

private:
    std::string program;

private:
    LitteraleProgramme(std::string prog) : Litterale(programme), program(prog){};
    ~LitteraleProgramme() = default;

public:
    const std::string toString() const override { return program; }
    const std::string rawInput() const override
    {
        if (program.size() > 3)
            return program.substr(2, program.size() - 4);
        else
            return "";
    }
    std::string getProgramme() const { return program; }
    Litterale *getCopie() const override;
    Litterale *NEG() override { throw(CompCeption("Erreur : Operateur NEG impossible avec un programme.")); }
    Litterale *operator+(Litterale *) override { throw(CompCeption("Erreur : Operateur + impossible avec un programme.")); }
    Litterale *operator-(Litterale *) override { throw(CompCeption("Erreur : Operateur - impossible avec un programme.")); }
    Litterale *operator*(Litterale *) override { throw(CompCeption("Erreur : Operateur * impossible avec un programme.")); }
    Litterale *operator/(Litterale *) override { throw(CompCeption("Erreur : Operateur / impossible avec un programme.")); }
    Litterale *operator==(Litterale *) override { throw(CompCeption("Erreur : Operateur == impossible avec un programme.")); }
    Litterale *operator!=(Litterale *) override { throw(CompCeption("Erreur : Operateur != impossible avec un programme.")); }
    Litterale *operator<=(Litterale *) override { throw(CompCeption("Erreur : Operateur <= impossible avec un programme.")); }
    Litterale *operator>=(Litterale *) override { throw(CompCeption("Erreur : Operateur >= impossible avec un programme.")); }
    Litterale *operator<(Litterale *) override { throw(CompCeption("Erreur : Operateur < impossible avec un programme.")); }
    Litterale *operator>(Litterale *) override { throw(CompCeption("Erreur : Operateur > impossible avec un programme.")); }
    Litterale *EVAL() override;
    Litterale *IFT(Litterale *) override;
    Litterale *IFTE(Litterale *, Litterale *) override;
    Litterale *POW(Litterale *) override { throw(CompCeption("Erreur : Operateur POW impossible avec un programme.")); }
    Litterale *SIN() override { throw(CompCeption("Erreur : Operateur SIN impossible avec un programme.")); }
    Litterale *COS() override { throw(CompCeption("Erreur : Operateur COS impossible avec un programme.")); }
    Litterale *TAN() override { throw(CompCeption("Erreur : Operateur TAN impossible avec un programme.")); }
    Litterale *ARCSIN() override { throw(CompCeption("Erreur : Operateur ARCSIN impossible avec un programme.")); }
    Litterale *ARCCOS() override { throw(CompCeption("Erreur : Operateur ARCCOS impossible avec un programme.")); }
    Litterale *ARCTAN() override { throw(CompCeption("Erreur : Operateur ARCTAN impossible avec un programme.")); }
    Litterale *SQRT() override { throw(CompCeption("Erreur : Operateur SQRT impossible avec un programme.")); }
    Litterale *EXP() override { throw(CompCeption("Erreur : Operateur EXP impossible avec un programme.")); }
    Litterale *LN() override { throw(CompCeption("Erreur : Operateur LN impossible avec un programme.")); }
};

/* --------------------------------------------------------------------------*/
/*----- Litterale Expression ----- */
/* --------------------------------------------------------------------------*/

class LitteraleExpression : public Litterale
{
    friend class LitteraleFactory;
    friend class LitteraleManager;

private:
    std::string express;
    Litterale *target = nullptr;

private:
    LitteraleExpression(std::string expr) : Litterale(expression), express(expr){};
    ~LitteraleExpression() = default;

public:
    const std::string toString() const override { return express; };
    const std::string getExpression() const { return express; }
    const std::string rawInput() const override { return express; }
    Litterale *getCopie() const override;
    Litterale *getTarget() { return target; }
    void setTarget(Litterale *lit) { target = lit; }
    Litterale *NEG() override { throw(CompCeption("Erreur : Operateur NEG impossible avec une expression.")); }
    Litterale *operator+(Litterale *) override { throw(CompCeption("Erreur : Operateur + impossible avec une expression.")); }
    Litterale *operator-(Litterale *) override { throw(CompCeption("Erreur : Operateur - impossible avec une expression.")); }
    Litterale *operator*(Litterale *) override { throw(CompCeption("Erreur : Operateur * impossible avec une expression.")); }
    Litterale *operator/(Litterale *) override { throw(CompCeption("Erreur : Operateur / impossible avec une expression.")); }
    Litterale *operator==(Litterale *) override { throw(CompCeption("Erreur : Operateur == impossible avec une expression.")); }
    Litterale *operator!=(Litterale *) override { throw(CompCeption("Erreur : Operateur != impossible avec une expression.")); }
    Litterale *operator<=(Litterale *) override { throw(CompCeption("Erreur : Operateur <= impossible avec une expression.")); }
    Litterale *operator>=(Litterale *) override { throw(CompCeption("Erreur : Operateur >= impossible avec une expression.")); }
    Litterale *operator<(Litterale *) override { throw(CompCeption("Erreur : Operateur < impossible avec une expression.")); }
    Litterale *operator>(Litterale *) override { throw(CompCeption("Erreur : Operateur > impossible avec une expression.")); }
    Litterale *EVAL() override;
    Litterale *IFT(Litterale *) override;
    Litterale *IFTE(Litterale *lit1, Litterale *lit2) override;
    void STO(Litterale *lit) override;
    Litterale *POW(Litterale *) override { throw(CompCeption("Erreur : Operateur POW impossible avec une expression.")); }
    Litterale *SIN() override { throw(CompCeption("Erreur : Operateur SIN impossible avec une expression.")); }
    Litterale *COS() override { throw(CompCeption("Erreur : Operateur COS impossible avec une expression.")); }
    Litterale *TAN() override { throw(CompCeption("Erreur : Operateur TAN impossible avec une expression.")); }
    Litterale *ARCSIN() override { throw(CompCeption("Erreur : Operateur ARCSIN impossible avec une expression.")); }
    Litterale *ARCCOS() override { throw(CompCeption("Erreur : Operateur ARCCOS impossible avec une expression.")); }
    Litterale *ARCTAN() override { throw(CompCeption("Erreur : Operateur ARCTAN impossible avec une expression.")); }
    Litterale *SQRT() override { throw(CompCeption("Erreur : Operateur SQRT impossible avec une expression.")); }
    Litterale *EXP() override { throw(CompCeption("Erreur : Operateur EXP impossible avec une expression.")); }
    Litterale *LN() override { throw(CompCeption("Erreur : Operateur LN impossible avec une expression.")); }
    void FORGET() override;
};

/* --------------------------------------------------------------------------*/
/*----- Litterale Factory ----- */
/* --------------------------------------------------------------------------*/

class LitteraleFactory
{

public:
    LitteraleFactory() = delete;
    static Litterale *createLitterale(int, typeLitterale);
    static Litterale *createLitterale(double, typeLitterale);
    static Litterale *createLitterale(int, int, typeLitterale);
    static Litterale *createLitterale(std::string, typeLitterale);
    static void deleteLitterale(Litterale *);
};

/* --------------------------------------------------------------------------*/
/*----- Litterale Manager ----- */
/* --------------------------------------------------------------------------*/

class LitteraleManager
{
    friend class LitteraleFactory;

private:
    std::vector<Litterale *> listeLitterales;

private:
    LitteraleManager() = default;
    LitteraleManager(const LitteraleManager &) = delete;
    LitteraleManager &operator=(const LitteraleManager &) = delete;
    ~LitteraleManager();

private:
    Litterale *addLitterale(Litterale *);
    void removeLitterale(Litterale *);

public:
    static LitteraleManager &donneInstance();

public:
    //Const iterator de LitteraleManager
    using const_iterator = std::vector<Litterale *>::const_iterator;
    const_iterator cbegin() { return listeLitterales.cbegin(); }
    const_iterator cend() { return listeLitterales.cend(); }
    //Const reverse iterator de LitteraleManager
    using const_reverse_iterator = std::vector<Litterale *>::const_reverse_iterator;
    const_reverse_iterator crbegin() { return listeLitterales.crbegin(); }
    const_reverse_iterator crend() { return listeLitterales.crend(); }
};
