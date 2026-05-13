#pragma once
#include "Sala.h"
#include "SalaVIP.h"
#include "Film.h"
#include "Rezervare.h"
#include "Persoana.h"
#include <string>
#include <vector>

class Cinematograf {
    string Nume;
    vector<Sala*> Sali;      
    vector<Film*> Filme;
    vector<Rezervare*> Rezervari;
    double PretBaza;
public:
    Cinematograf(string nume, double pretBaza = 30.0);
    ~Cinematograf();
    void AdaugaSala(Sala* s);
    void AdaugaFilm(Film* f);
    Rezervare* FaRezervare(Persoana* p, Film* f, Sala* s, int rand, int col);
    Sala* GasesteSala(string numar) const;
    Film* GasesteFilm(string titlu) const;
    void AfisareSali() const;
    void AfisareFilme() const;
    void AfisareRezervari() const;
    void AfisareHartaSala(string numarSala) const;
    void AfisareIncasari() const;
};
