#include "RezervareOnline.h"
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

// Genereaza un cod de confirmare de forma "ONL-XXXX-ID"
string RezervareOnline::GenereazaCod(int id) {
    // Folosim id-ul rezervarii si un numar pseudo-aleator simplu
    int hash = (id * 7919 + 31337) % 9000 + 1000; // 4 cifre
    ostringstream os;
    os << "ONL-" << hash << "-" << id;
    return os.str();
}

RezervareOnline::RezervareOnline(Persoana* c, Spectacol* sp,
    int rand, int col,
    const string& email)
    : Rezervare(c, sp, rand, col),
    emailClient(email)
{
    codConfirmare = GenereazaCod(GetID());
}

void RezervareOnline::Afisare() const {
    // Afiseaza biletul standard, apoi adauga informatiile online
    Rezervare::Afisare();
    cout << "| [ONLINE]                        |\n";
    cout << "| Email:  " << left << setw(24) << emailClient << "|\n";
    cout << "| Cod:    " << left << setw(24) << codConfirmare << "|\n";
    cout << "+---------------------------------+\n";
}

string RezervareOnline::ToString() const {
    return Rezervare::ToString()
        + " | ONLINE | " + emailClient
        + " | Cod: " + codConfirmare;
}

ostream& operator<<(ostream& out, const RezervareOnline& r) {
    out << r.ToString();
    return out;
}