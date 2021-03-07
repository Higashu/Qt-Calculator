#pragma once

#include <string> //A retirer sur Qt
//#include <QString>
#include <vector> //A retirer sur Qt
//#include <QVector>
#include "litterales.h"

class UserInput
{

private:
	const std::vector<std::string> operateurBinaire = {"+", "-", "/", "*", "AND", "OR", "IFT", "STO", "DIV", "MOD", "<=", ">=", "<", ">", "==", "!=", "POW", "WHILE"};
	const std::vector<std::string> operateurUnaire = {"NOT", "NEG", "EVAL", "SIN", "COS", "TAN", "ARCSIN", "ARCCOS", "ARCTAN", "SQRT", "EXP", "LN", "DEN", "NUM", "FORGET"};
	const std::vector<std::string> operateurPile = {"CLEAR", "DUP", "DROP", "SWAP", "UNDO", "REDO"};
	const std::vector<std::string> operateurTernaire = {"IFTE"};

private:
	UserInput() = default;
	UserInput(const UserInput &) = delete;
	UserInput &operator=(const UserInput &) = delete;
	~UserInput() = default;

public:
	static UserInput &donneInstance();
	//Dans le UserInput car on utilise les fonctions commandeInterpreteur et stripCommande
	void loadExpression();
	void saveExpression();
	bool isOpUnaire(std::string);
	bool isOpBinaire(std::string);
	bool isOpPile(std::string);
	bool isOpTernaire(std::string);
	const std::vector<std::string> stripCommande(std::string, bool = true);
	void commandeInterpreteur(std::vector<std::string>, bool = false);
	void handleLitterale(std::pair<int, int>);
	void handleLitterale(std::pair<double, int>);
	void handleLitterale(std::pair<std::string, int>);
	void handleOperateurUnaire(std::string);
	void handleOperateurBinaire(std::string);
	void handleOperateurTernaire(std::string);
	void handleOperateurPile(std::string);
	//execute à modifier sur Qt
	void execute();
};

/*
À faire pour un fonctionnement normal :
1. USERINPUT.loadExpression() a l'ouverture du programme
2. une fois la commande de l'utilisateur recupere l'envoyer a execute ( modifier la fonction en conséquence commandeInterpreteur(stripcommande(maCommande)) )...
3. a la fermeture du programme TABEXPR.saveExpression()
*/