#pragma once
#include <string>
#include <iostream>
using namespace std;

// Clasa abstracta pentru orice angajat al cinematografului.
// Mosteneste indirect aceleasi principii ca Persoana, dar are nume,
// username, parola si rol. Lasata separat de Persoana (care e pentru clienti)
// pentru a evita amestecul de responsabilitati.
class Angajat {
protected:
    static int GenerareID;
    int id;
    string nume;
    string username;
    string parola;       // (in productie ar trebui hash-uita; aici simplificat)
    double salariu;

public:
    Angajat(string nume, string username, string parola, double salariu);
    virtual ~Angajat();

    int GetID() const { return id; }
    string GetNume() const { return nume; }
    string GetUsername() const { return username; }
    string GetParola() const { return parola; }     // pentru verificare
    double GetSalariu() const { return salariu; }

    void SetNume(const string& n) { nume = n; }
    void SetParola(const string& p) { parola = p; }
    void SetSalariu(double s) { salariu = s; }

    // Verifica daca parola data corespunde
    bool Autentifica(const string& parolaTest) const { return parolaTest == parola; }

    // Comportament polimorfic
    virtual string GetRol() const = 0;
    virtual bool PoateModificaFilme() const = 0;
    virtual bool PoateModificaSali() const = 0;
    virtual bool PoateAdaugaSpectacole() const = 0;
    virtual bool PoateVedeRapoarte() const = 0;
    virtual bool PoateGestionaAngajati() const = 0;

    virtual void Afisare() const;
    virtual Angajat* Clone() const = 0;            // pattern: clone polimorfic

    friend ostream& operator<<(ostream& out, const Angajat& a);

    // Pentru persistenta
    void SetID(int newId) { id = newId; if (newId >= GenerareID) GenerareID = newId + 1; }
    static int GetNextID() { return GenerareID; }
    static void SetNextID(int v) { GenerareID = v; }
};
