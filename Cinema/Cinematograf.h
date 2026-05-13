#pragma once
#include "Sala.h"
#include "SalaVIP.h"
#include "Film.h"
#include "Rezervare.h"
#include "Persoana.h"
#include "Spectacol.h"
#include "Repository.h"
#include <string>
#include <vector>

// Cinematograf - clasa centrala a aplicatiei. Gestioneaza:
//  - filmele
//  - salile (normale si VIP)
//  - spectacolele (programari film+sala+data+ora)
//  - rezervarile
//  - clientii (persoanele care au facut rezervari)
// Foloseste Repository<T> pentru toate colectiile interne.
class Cinematograf {
    string Nume;
    string Adresa;
    string Oras;
    double PretBaza;

    Repository<Film>      filme;        // detine memoria
    Repository<Sala>      sali;         // detine memoria
    Repository<Spectacol> spectacole;   // detine memoria
    Repository<Rezervare> rezervari;    // detine memoria
    Repository<Persoana>  clienti;      // detine memoria

public:
    Cinematograf(string nume = "CinePlex",
        double pretBaza = 30.0,
        string adresa = "Bd. Independentei nr. 1",
        string oras = "Bucuresti");
    ~Cinematograf() = default;       // toate repository-urile au detineMemoria=true

    // Filme
    void AdaugaFilm(Film* f);
    bool StergeFilm(int id);
    Film* GasesteFilm(const string& titlu) const;
    Film* GasesteFilmDupaID(int id) const;
    const Repository<Film>& GetFilme() const { return filme; }

    // Sali
    void AdaugaSala(Sala* s);
    bool StergeSala(int id);
    Sala* GasesteSala(const string& numar) const;
    Sala* GasesteSalaDupaID(int id) const;
    const Repository<Sala>& GetSali() const { return sali; }

    // Spectacole
    void AdaugaSpectacol(Spectacol* sp);
    Spectacol* CreeazaSpectacol(Film* f, Sala* s,
        const string& data, const string& ora,
        double pret = -1);              // -1 = foloseste PretBaza
    bool StergeSpectacol(int id);
    Spectacol* GasesteSpectacol(int id) const;
    vector<Spectacol*> SpectacolePentruFilm(int filmId) const;
    vector<Spectacol*> SpectacolePentruSala(int salaId) const;
    vector<Spectacol*> SpectacolePeData(const string& data) const;
    const Repository<Spectacol>& GetSpectacole() const { return spectacole; }

    // Rezervari
    Rezervare* FaRezervare(Persoana* p, Spectacol* sp, int rand, int col);
    Rezervare* FaRezervare(Persoana* p, Film* f, Sala* s, int rand, int col);
    bool StergeRezervare(int id);
    Rezervare* GasesteRezervare(int id) const;
    const Repository<Rezervare>& GetRezervari() const { return rezervari; }

    // Clienti
    void AdaugaClient(Persoana* p);
    const Repository<Persoana>& GetClienti() const { return clienti; }
    Persoana* GasesteClientDupaNume(const string& nume) const;

    // Afisari (consola)
    void AfisareSali() const;
    void AfisareFilme() const;
    void AfisareSpectacole() const;
    void AfisareRezervari() const;
    void AfisareHartaSala(const string& numarSala) const;
    void AfisareHartaSpectacol(int spectacolId) const;
    void AfisareIncasari() const;
    void AfisareAntet() const;

    // Acces
    string GetNume()    const { return Nume; }
    string GetAdresa()  const { return Adresa; }
    string GetOras()    const { return Oras; }
    double GetPretBaza() const { return PretBaza; }

    void SetNume(const string& n) { Nume = n; }
    void SetAdresa(const string& a) { Adresa = a; }
    void SetOras(const string& o) { Oras = o; }
    void SetPretBaza(double p) { PretBaza = p; }

    // Sterge totul (folosit la re-incarcarea din fisier)
    void Clear();

    // Acces public la repository-uri (pentru persistenta / raport)
    Repository<Film>&      GetFilmeRef()      { return filme; }
    Repository<Sala>&      GetSaliRef()       { return sali; }
    Repository<Spectacol>& GetSpectacoleRef() { return spectacole; }
    Repository<Rezervare>& GetRezervariRef()  { return rezervari; }
    Repository<Persoana>&  GetClientiRef()    { return clienti; }
};
