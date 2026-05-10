#pragma once
#include "Film.h"
#include "Sala.h"
#include "Persoana.h"
#include <ctime>
#include <iomanip>

class Rezervare
{
	static int GenerareID;
	int idRezervare;
	Persoana* client;
	Film* film;
	Sala* sala;
	int rand, coloana;
	double pretBaza;
	double pretFinal;
public:
	Rezervare(Persoana* c, Film* f, Sala* s, int r, int col, double baza);
	double CalculeazaPret() const;
	bool EsteVineri() const;
	void Afisare() const;
	int    GetID()          const;
	double GetPretFinal()   const;
	string GetNumeClient()  const;
	string GetTitluFilm()   const;

};