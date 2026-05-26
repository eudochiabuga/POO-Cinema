#pragma once
#include "Angajat.h"
#include "TipAngajat.h"
#include "Repository.h"
#include <string>
#include <vector>
using namespace std;

// Sistemul de autentificare: gestioneaza angajatii si tine evidenta
// utilizatorului curent logat. Foloseste Repository<Angajat> intern -
// demonstreaza folosirea template-urilor.
class SistemAutentificare {
    Repository<Angajat> angajati;       // detine memoria
    Angajat* utilizatorCurent;          // pointer la unul din angajati

public:
    SistemAutentificare();
    ~SistemAutentificare() = default;   // Repository sterge angajatii

    // Adaugare/stergere
    void AdaugaAngajat(Angajat* a);
    bool StergeAngajat(int id);
    bool StergeAngajatDupaUsername(const string& username);

    // Autentificare. Arunca AutentificareException daca esueaza.
    Angajat* Login(const string& username, const string& parola);
    void Logout();

    Angajat* GetUtilizatorCurent() const { return utilizatorCurent; }
    bool EsteAutentificat() const { return utilizatorCurent != nullptr; }

    // Asigurare drepturi (arunca DrepturiInsuficienteException)
    void VerificaDrepturiManager() const;
    void VerificaDrepturiFilme() const;
    void VerificaDrepturiSali() const;
    void VerificaDrepturiSpectacole() const;
    void VerificaDrepturiRapoarte() const;
    void VerificaDrepturiAngajati() const;

    // Acces la lista
    const Repository<Angajat>& GetAngajati() const { return angajati; }
    Repository<Angajat>& GetAngajatiRef() { return angajati; }

    Angajat* GasesteDupaUsername(const string& username) const;
    Angajat* GasesteDupaID(int id) const;

    // Populeaza cu utilizatori impliciti (admin/admin, manager/manager, casier/casier)
    void IncarcaImpliciti();
};
