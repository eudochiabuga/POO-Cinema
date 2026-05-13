#include "SistemAutentificare.h"
#include "Exceptii.h"

SistemAutentificare::SistemAutentificare()
    : angajati(true), utilizatorCurent(nullptr) {
}

void SistemAutentificare::AdaugaAngajat(Angajat* a) {
    if (!a) throw DateInvalideException("Angajat null.");
    if (GasesteDupaUsername(a->GetUsername()))
        throw DateInvalideException("Username \"" + a->GetUsername() + "\" deja folosit.");
    angajati.Adauga(a);
}

bool SistemAutentificare::StergeAngajat(int id) {
    Angajat* a = GasesteDupaID(id);
    if (!a) return false;
    if (a == utilizatorCurent) utilizatorCurent = nullptr;
    return angajati.Sterge(a);
}

bool SistemAutentificare::StergeAngajatDupaUsername(const string& username) {
    Angajat* a = GasesteDupaUsername(username);
    if (!a) return false;
    if (a == utilizatorCurent) utilizatorCurent = nullptr;
    return angajati.Sterge(a);
}

Angajat* SistemAutentificare::Login(const string& username, const string& parola) {
    Angajat* a = GasesteDupaUsername(username);
    if (!a || !a->Autentifica(parola))
        throw AutentificareException();
    utilizatorCurent = a;
    return a;
}

void SistemAutentificare::Logout() {
    utilizatorCurent = nullptr;
}

void SistemAutentificare::VerificaDrepturiManager() const {
    if (!utilizatorCurent || !utilizatorCurent->PoateGestionaAngajati())
        throw DrepturiInsuficienteException("Doar managerul poate executa aceasta actiune.");
}
void SistemAutentificare::VerificaDrepturiFilme() const {
    if (!utilizatorCurent || !utilizatorCurent->PoateModificaFilme())
        throw DrepturiInsuficienteException("Nu aveti drepturi sa modificati filmele.");
}
void SistemAutentificare::VerificaDrepturiSali() const {
    if (!utilizatorCurent || !utilizatorCurent->PoateModificaSali())
        throw DrepturiInsuficienteException("Nu aveti drepturi sa modificati salile.");
}
void SistemAutentificare::VerificaDrepturiSpectacole() const {
    if (!utilizatorCurent || !utilizatorCurent->PoateAdaugaSpectacole())
        throw DrepturiInsuficienteException("Nu aveti drepturi sa adaugati spectacole.");
}
void SistemAutentificare::VerificaDrepturiRapoarte() const {
    if (!utilizatorCurent || !utilizatorCurent->PoateVedeRapoarte())
        throw DrepturiInsuficienteException("Nu aveti drepturi sa vedeti rapoartele.");
}
void SistemAutentificare::VerificaDrepturiAngajati() const {
    if (!utilizatorCurent || !utilizatorCurent->PoateGestionaAngajati())
        throw DrepturiInsuficienteException("Nu aveti drepturi sa gestionati angajatii.");
}

Angajat* SistemAutentificare::GasesteDupaUsername(const string& username) const {
    return angajati.GasestePrimul([&](const Angajat* a) {
        return a->GetUsername() == username;
        });
}

Angajat* SistemAutentificare::GasesteDupaID(int id) const {
    return angajati.GasestePrimul([&](const Angajat* a) {
        return a->GetID() == id;
        });
}

void SistemAutentificare::IncarcaImpliciti() {
    if (!angajati.Vid()) return;
    angajati.Adauga(new Administrator("Andrei Popescu", "admin", "admin", 8000));
    angajati.Adauga(new Manager("Maria Ionescu", "manager", "manager", 6500));
    angajati.Adauga(new Casier("Ion Vasile", "casier", "casier", 3500));
    angajati.Adauga(new Casier("Ana Dumitrescu", "ana", "ana123", 3200));
}
