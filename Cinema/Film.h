#pragma once
#include <string>
#include <iostream>
using namespace std;

class Film
{
	string Titlu;
	string Categorie;
	int Durata;
	int VarstaMinima;
	string Limba;
public:
	Film(string titlu, string categorie, int durata, int varstaMinima, string limba );
	Film(const Film& f);
	Film& operator=(const Film& f);
	string GetTitlu() const;
	string GetCategorie() const;
	int GetDurata() const;
	int GetVarstaMinima() const;
	string GetLimba() const;
	void SetLimba(string limba);
	void Afisare() const;

	friend ostream& operator<<(ostream& out, const Film& f);
};
