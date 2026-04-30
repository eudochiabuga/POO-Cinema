#pragma once
#include "Film.h"
#include "Sala.h"

class Rezervare
{
	static int GenerareID;
	int idRezervare;
	Client* client;
	Film* film;
	Sala* sala;
	int rand, coloana;
	double pret;
};