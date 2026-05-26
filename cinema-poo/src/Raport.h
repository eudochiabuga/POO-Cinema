#pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;

class Cinematograf;       // forward declaration

// Structuri pentru rapoarte
struct StatisticaFilm {
    string titlu;
    int numarRezervari;
    double incasariTotale;
    double procentOcupare;       // optional
};

struct StatisticaSala {
    string numarSala;
    int rezervari;
    int locuriTotale;
    double procentOcupare;
    double incasari;
};

struct StatisticaCategorie {
    string categorie;
    int numarFilme;
    int numarRezervari;
    double incasari;
};

struct StatisticaZilnica {
    string data;             // "YYYY-MM-DD"
    int rezervari;
    double incasari;
};

// Raport = clasa "view" peste Cinematograf, calculeaza statistici.
class Raport {
    const Cinematograf* cinema;

public:
    Raport(const Cinematograf* c) : cinema(c) {}

    // Totaluri
    double TotalIncasari() const;
    int TotalRezervari() const;
    int TotalFilme() const;
    int TotalSali() const;
    int TotalSpectacole() const;

    // Top filme dupa numar rezervari
    vector<StatisticaFilm> TopFilme(int n = 10) const;

    // Statistici per sala
    vector<StatisticaSala> StatisticiSali() const;

    // Statistici per categorie
    vector<StatisticaCategorie> StatisticiCategorii() const;

    // Statistici zilnice (pe baza datelor spectacolelor)
    vector<StatisticaZilnica> StatisticiZilnice() const;

    // Media pret per bilet
    double PretMediuBilet() const;

    // Numar rezervari pe tip persoana (Adult/Student/Elev)
    map<string, int> RezervariPerTip() const;

    // Genereaza un raport text complet (pentru afisare/salvare)
    string RaportComplet() const;
};
