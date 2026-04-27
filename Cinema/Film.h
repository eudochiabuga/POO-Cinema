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
	Film(const Film& film);
	Film& operator=(const Film& film);
	void Afisare();
	string GetTitlu() const
	{
		return Titlu;
	}
	string GetCategorie() const
	{
		return Categorie;
	}
	int GetDurata() const
	{
		return Durata;
	}
	int GetVarstaMinima() const
	{
		return VarstaMinima;
	}
	void SetLimba(string limba);
	friend ostream& operator<<(ostream& out, const Film& film);
};

