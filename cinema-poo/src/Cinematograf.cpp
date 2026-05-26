#include "Cinematograf.h"
#include "Exceptii.h"
#include <iostream>
#include <iomanip>
using namespace std;

Cinematograf::Cinematograf(string nume, double pretBaza, string adresa, string oras)
    : Nume(nume), PretBaza(pretBaza), Adresa(adresa), Oras(oras),
    filme(true), sali(true), spectacole(true),
    rezervari(true), clienti(true) {
}

// ---------- Filme ----------
void Cinematograf::AdaugaFilm(Film* f) {
    if (!f) throw DateInvalideException("Film null.");
    filme.Adauga(f);
}

bool Cinematograf::StergeFilm(int id) {
    Film* f = GasesteFilmDupaID(id);
    if (!f) return false;
    // sterge si spectacolele si rezervarile legate
    for (auto sp : spectacole.Filtreaza([&](const Spectacol* s) { return s->GetFilm() == f; }))
        StergeSpectacol(sp->GetID());
    return filme.Sterge(f);
}

Film* Cinematograf::GasesteFilm(const string& titlu) const {
    return filme.GasestePrimul([&](const Film* f) { return f->GetTitlu() == titlu; });
}

Film* Cinematograf::GasesteFilmDupaID(int id) const {
    return filme.GasestePrimul([&](const Film* f) { return f->GetID() == id; });
}

// ---------- Sali ----------
void Cinematograf::AdaugaSala(Sala* s) {
    if (!s) throw DateInvalideException("Sala null.");
    sali.Adauga(s);
}

bool Cinematograf::StergeSala(int id) {
    Sala* s = GasesteSalaDupaID(id);
    if (!s) return false;
    for (auto sp : spectacole.Filtreaza([&](const Spectacol* x) { return x->GetSala() == s; }))
        StergeSpectacol(sp->GetID());
    return sali.Sterge(s);
}

Sala* Cinematograf::GasesteSala(const string& numar) const {
    return sali.GasestePrimul([&](const Sala* s) { return s->GetNumarSala() == numar; });
}

Sala* Cinematograf::GasesteSalaDupaID(int id) const {
    return sali.GasestePrimul([&](const Sala* s) { return s->GetID() == id; });
}

// ---------- Spectacole ----------
void Cinematograf::AdaugaSpectacol(Spectacol* sp) {
    if (!sp) throw DateInvalideException("Spectacol null.");
    spectacole.Adauga(sp);
}

Spectacol* Cinematograf::CreeazaSpectacol(Film* f, Sala* s,
    const string& data, const string& ora, double pret) {
    if (!f) throw ElementInexistentException("Film", "(null)");
    if (!s) throw ElementInexistentException("Sala", "(null)");
    double p = (pret < 0) ? PretBaza : pret;
    Spectacol* sp = new Spectacol(f, s, data, ora, p);
    spectacole.Adauga(sp);
    return sp;
}

bool Cinematograf::StergeSpectacol(int id) {
    Spectacol* sp = GasesteSpectacol(id);
    if (!sp) return false;
    // sterge rezervarile legate
    for (auto r : rezervari.Filtreaza([&](const Rezervare* x) { return x->GetSpectacol() == sp; }))
        rezervari.Sterge(r);
    return spectacole.Sterge(sp);
}

Spectacol* Cinematograf::GasesteSpectacol(int id) const {
    return spectacole.GasestePrimul([&](const Spectacol* sp) { return sp->GetID() == id; });
}

vector<Spectacol*> Cinematograf::SpectacolePentruFilm(int filmId) const {
    return spectacole.Filtreaza([&](const Spectacol* sp) {
        return sp->GetFilm() && sp->GetFilm()->GetID() == filmId;
        });
}
vector<Spectacol*> Cinematograf::SpectacolePentruSala(int salaId) const {
    return spectacole.Filtreaza([&](const Spectacol* sp) {
        return sp->GetSala() && sp->GetSala()->GetID() == salaId;
        });
}
vector<Spectacol*> Cinematograf::SpectacolePeData(const string& data) const {
    return spectacole.Filtreaza([&](const Spectacol* sp) {
        return sp->GetData() == data;
        });
}

// ---------- Rezervari ----------
Rezervare* Cinematograf::FaRezervare(Persoana* p, Spectacol* sp, int rand, int col) {
    if (!p) throw DateInvalideException("Client null.");
    if (!sp) throw DateInvalideException("Spectacol null.");

    // Verifica drepturile / disponibilitatea (arunca exceptie)
    sp->RezervaLoc(rand, col);

    try {
        Rezervare* r = new Rezervare(p, sp, rand, col);
        rezervari.Adauga(r);
        return r;
    }
    catch (...) {
        // daca rezervarea esueaza (de exemplu varsta), elibereaza locul
        sp->ElibereazaLoc(rand, col);
        throw;
    }
}

Rezervare* Cinematograf::FaRezervare(Persoana* p, Film* f, Sala* s, int rand, int col) {
    // Wrapper pentru fluxul "rapid" - creaza un Spectacol "implicit" si rezerva.
    if (!p || !f || !s) throw DateInvalideException("Date incomplete.");
    Spectacol* sp = CreeazaSpectacol(f, s, "rapid", "--:--", PretBaza);
    return FaRezervare(p, sp, rand, col);
}

bool Cinematograf::StergeRezervare(int id) {
    Rezervare* r = GasesteRezervare(id);
    if (!r) return false;
    // elibereaza locul in spectacol
    if (r->GetSpectacol())
        r->GetSpectacol()->ElibereazaLoc(r->GetRand(), r->GetColoana());
    return rezervari.Sterge(r);
}

Rezervare* Cinematograf::GasesteRezervare(int id) const {
    return rezervari.GasestePrimul([&](const Rezervare* r) { return r->GetID() == id; });
}

// ---------- Clienti ----------
void Cinematograf::AdaugaClient(Persoana* p) {
    if (!p) return;
    clienti.Adauga(p);
}

Persoana* Cinematograf::GasesteClientDupaNume(const string& nume) const {
    return clienti.GasestePrimul([&](const Persoana* p) { return p->GetNume() == nume; });
}

// ---------- Reset ----------
void Cinematograf::Clear() {
    while (!rezervari.Vid())  rezervari.StergeIndex(0);
    while (!spectacole.Vid()) spectacole.StergeIndex(0);
    while (!sali.Vid())       sali.StergeIndex(0);
    while (!filme.Vid())      filme.StergeIndex(0);
    while (!clienti.Vid())    clienti.StergeIndex(0);
}

// ---------- Afisari ----------
void Cinematograf::AfisareAntet() const {
    cout << "\n===========================================\n";
    cout << "  " << Nume << " - " << Adresa << ", " << Oras << "\n";
    cout << "  Pret baza: " << fixed << setprecision(2) << PretBaza << " RON\n";
    cout << "===========================================\n";
}

void Cinematograf::AfisareSali() const {
    cout << "\n=== Sali - " << Nume << " ===\n";
    if (sali.Vid()) { cout << "  (nicio sala)\n"; return; }
    for (auto s : sali) {
        s->Afisare();
        cout << "-----------------------------\n";
    }
}

void Cinematograf::AfisareFilme() const {
    cout << "\n=== Filme - " << Nume << " ===\n";
    if (filme.Vid()) { cout << "  (niciun film)\n"; return; }
    int i = 1;
    for (auto f : filme)
        cout << i++ << ". " << *f << "\n";
    cout << "\n";
}

void Cinematograf::AfisareSpectacole() const {
    cout << "\n=== Spectacole - " << Nume << " ===\n";
    if (spectacole.Vid()) { cout << "  (niciun spectacol)\n"; return; }
    for (auto sp : spectacole)
        cout << "  " << *sp << " | locuri libere: "
        << sp->LocuriDisponibile() << "/"
        << sp->GetRanduri() * sp->GetColoane() << "\n";
    cout << "\n";
}

void Cinematograf::AfisareRezervari() const {
    cout << "\n=== Rezervari - " << Nume << " ===\n";
    if (rezervari.Vid()) { cout << "  (nicio rezervare)\n"; return; }
    for (auto r : rezervari) {
        cout << "  " << *r << "\n";
    }
}

void Cinematograf::AfisareHartaSala(const string& numarSala) const {
    Sala* s = GasesteSala(numarSala);
    if (!s) throw ElementInexistentException("Sala", numarSala);
    s->AfisareHarta();
}

void Cinematograf::AfisareHartaSpectacol(int spectacolId) const {
    Spectacol* sp = GasesteSpectacol(spectacolId);
    if (!sp) throw ElementInexistentException("Spectacol", to_string(spectacolId));
    sp->AfisareHarta();
}

void Cinematograf::AfisareIncasari() const {
    cout << "\n=== Incasari - " << Nume << " ===\n";
    double total = 0;
    for (auto r : rezervari) {
        cout << "  Bilet #" << r->GetID()
            << " | " << left << setw(20) << r->GetNumeClient()
            << " | " << setw(20) << r->GetTitluFilm()
            << " | " << fixed << setprecision(2)
            << r->GetPretFinal() << " RON\n";
        total += r->GetPretFinal();
    }
    cout << "  -----------------------------------------\n";
    cout << "  TOTAL: " << fixed << setprecision(2) << total << " RON\n\n";
}
