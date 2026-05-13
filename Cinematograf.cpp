#include "Cinematograf.h"
#include <iostream>
#include <iomanip>
using namespace std;

int Rezervare::GenerareID = 1;
Cinematograf::Cinematograf(string nume, double pretBaza)
    : Nume(nume), PretBaza(pretBaza) {
}

Cinematograf::~Cinematograf()
{
    for (auto r : Rezervari)
        delete r;
}

void Cinematograf::AdaugaSala(Sala* s) {
    if (!s) { cout << "[Eroare] Sala invalida.\n"; return; }
    Sali.push_back(s);
    cout << "[OK] Sala " << s->GetNumarSala()
        << " adaugata la " << Nume << ".\n";
}

void Cinematograf::AdaugaFilm(Film* f) {
    if (!f) { cout << "[Eroare] Film invalid.\n"; return; }
    Filme.push_back(f);
    cout << "[OK] Filmul \"" << f->GetTitlu() << "\" adaugat.\n";
}


Sala* Cinematograf::GasesteSala(string numar) const {
    for (auto s : Sali)
        if (s->GetNumarSala() == numar) return s;
    return nullptr;
}

Film* Cinematograf::GasesteFilm(string titlu) const {
    for (auto f : Filme)
        if (f->GetTitlu() == titlu) return f;
    return nullptr;
}

Rezervare* Cinematograf::FaRezervare(Persoana* p, Film* f,
    Sala* s, int rand, int col) {
    if (!p || !f || !s) {
        cout << "[Eroare] Date incomplete pentru rezervare.\n";
        return nullptr;
    }

    if (p->GetVarsta() < f->GetVarstaMinima()) {
        cout << "[Eroare] " << p->GetNume()
            << " nu are varsta minima pentru filmul \""
            << f->GetTitlu() << "\" (minim "
            << f->GetVarstaMinima() << " ani).\n";
        return nullptr;
    }

    if (!s->VerificaDisponibilitate(rand, col)) {
        cout << "[Eroare] Locul (rand " << rand + 1
            << ", col " << col + 1 << ") este deja ocupat.\n";
        return nullptr;
    }

    s->OcupaLoc(rand, col);

    Rezervare* rez = new Rezervare(p, f, s, rand, col, PretBaza);
    Rezervari.push_back(rez);

    cout << "[OK] Rezervare pentru " << p->GetNume()
        << " | \"" << f->GetTitlu() << "\""
        << " | Rand " << rand + 1 << ", Col " << col + 1
        << " | " << fixed << setprecision(2)
        << rez->GetPretFinal() << " RON\n";

    return rez;
}


void Cinematograf::AfisareSali() const {
    cout << "\n=== Sali — " << Nume << " ===\n";
    if (Sali.empty()) { cout << "  (nicio sala)\n"; return; }
    for (auto s : Sali) {
        s->Afisare();
        cout << "-----------------------------\n";
    }
}

void Cinematograf::AfisareFilme() const {
    cout << "\n=== Filme — " << Nume << " ===\n";
    if (Filme.empty()) { cout << "  (niciun film)\n"; return; }
    for (int i = 0; i < (int)Filme.size(); i++)
        cout << i + 1 << ". " << *Filme[i] << "\n";
    cout << "\n";
}

void Cinematograf::AfisareRezervari() const {
    cout << "\n=== Rezervari — " << Nume << " ===\n";
    if (Rezervari.empty()) { cout << "  (nicio rezervare)\n"; return; }
    for (auto r : Rezervari) {
        r->Afisare();
        cout << "\n";
    }
}

void Cinematograf::AfisareHartaSala(string numarSala) const {
    Sala* s = GasesteSala(numarSala);
    if (!s) { cout << "[Eroare] Sala " << numarSala << " nu exista.\n"; return; }
    s->AfisareHarta();
}

void Cinematograf::AfisareIncasari() const {
    cout << "\n=== Incasari — " << Nume << " ===\n";
    double total = 0;
    for (auto r : Rezervari) {
        cout << "  Bilet #" << r->GetID()
            << " | " << left << setw(20) << r->GetNumeClient()
            << " | " << setw(18) << r->GetTitluFilm()
            << " | " << fixed << setprecision(2)
            << r->GetPretFinal() << " RON\n";
        total += r->GetPretFinal();
    }
    cout << "  -----------------------------------------\n";
    cout << "  TOTAL: " << fixed << setprecision(2) << total << " RON\n\n";
}




