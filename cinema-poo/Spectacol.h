#pragma once
#include "Film.h"
#include "Sala.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

// Spectacol = un Film, intr-o Sala, la o anumita data si ora.
// Are propria sa matrice de rezervari (independenta de matricea Salii) -
// pentru ca aceeasi Sala poate gazdui mai multe Spectacole in zile diferite.
class Spectacol {
    static int GenerareID;
    int id;
    Film* film;        // nu detine memoria
    Sala* sala;        // nu detine memoria
    string data;       // "YYYY-MM-DD"
    string ora;        // "HH:MM"
    double pretBaza;   // pretul de baza pentru acest spectacol
    bool** ocupat;     // matrice de rezervari (dimensiuni preluate din sala)
    int randuri, coloane;

    void AlocaOcupat();
    void ElibereazaOcupat();

public:
    Spectacol(Film* f, Sala* s, string data, string ora, double pretBaza);
    Spectacol(const Spectacol& s);
    Spectacol& operator=(const Spectacol& s);
    ~Spectacol();

    int GetID() const { return id; }
    Film* GetFilm() const { return film; }
    Sala* GetSala() const { return sala; }
    string GetData() const { return data; }
    string GetOra() const { return ora; }
    double GetPretBaza() const { return pretBaza; }
    int GetRanduri() const { return randuri; }
    int GetColoane() const { return coloane; }

    void SetPretBaza(double p) { pretBaza = p; }
    void SetData(const string& d) { data = d; }
    void SetOra(const string& o) { ora = o; }

    bool LocLiber(int rand, int col) const;
    bool RezervaLoc(int rand, int col);          // arunca exceptie daca e ocupat / invalid
    void ElibereazaLoc(int rand, int col);

    int LocuriOcupate() const;
    int LocuriDisponibile() const;
    double ProcentOcupare() const;

    bool EsteVIP(int rand) const;                 // ultimele 2 randuri (in sala normala)

    void Afisare() const;
    void AfisareHarta() const;

    // Operator pentru sortare dupa data+ora
    bool operator<(const Spectacol& s) const;

    friend ostream& operator<<(ostream& out, const Spectacol& s);

    // Setteri necesari pentru persistenta (la incarcare din fisier)
    void SetID(int newId) { id = newId; if (newId >= GenerareID) GenerareID = newId + 1; }
    static int GetNextID() { return GenerareID; }
    static void SetNextID(int v) { GenerareID = v; }

    // Acces controlat la matricea de rezervari (pentru persistenta)
    void MarcheazaOcupat(int rand, int col, bool valoare);
};
