#pragma once
#include "Angajat.h"

// Manager - are toate drepturile.
class Manager : public Angajat {
public:
    Manager(string nume, string username, string parola, double salariu);
    string GetRol() const override;
    bool PoateModificaFilme() const override { return true; }
    bool PoateModificaSali() const override { return true; }
    bool PoateAdaugaSpectacole() const override { return true; }
    bool PoateVedeRapoarte() const override { return true; }
    bool PoateGestionaAngajati() const override { return true; }
    Angajat* Clone() const override;
};

// Casier - poate face rezervari, vede spectacole; nu poate sterge filme/sali
// sau gestiona angajati.
class Casier : public Angajat {
public:
    Casier(string nume, string username, string parola, double salariu);
    string GetRol() const override;
    bool PoateModificaFilme() const override { return false; }
    bool PoateModificaSali() const override { return false; }
    bool PoateAdaugaSpectacole() const override { return false; }
    bool PoateVedeRapoarte() const override { return true; }   // poate vedea, dar simplificat
    bool PoateGestionaAngajati() const override { return false; }
    Angajat* Clone() const override;
};

// Administrator - similar cu Manager dar separat (in cazul in care vrei drepturi
// chiar si mai inalte, de exemplu pentru import/export tot)
class Administrator : public Angajat {
public:
    Administrator(string nume, string username, string parola, double salariu);
    string GetRol() const override;
    bool PoateModificaFilme() const override { return true; }
    bool PoateModificaSali() const override { return true; }
    bool PoateAdaugaSpectacole() const override { return true; }
    bool PoateVedeRapoarte() const override { return true; }
    bool PoateGestionaAngajati() const override { return true; }
    Angajat* Clone() const override;
};
