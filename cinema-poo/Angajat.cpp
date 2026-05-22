#include "Angajat.h"
#include <iomanip>

int Angajat::GenerareID = 1;

Angajat::Angajat(string nume, string username, string parola, double salariu)
    : nume(nume), username(username), parola(parola), salariu(salariu)
{
    id = GenerareID++;
}

Angajat::~Angajat() {}

void Angajat::Afisare() const {
    cout << "Angajat #" << id << " | "
        << left << setw(20) << nume
        << " | " << setw(15) << username
        << " | " << setw(10) << GetRol()
        << " | " << fixed << setprecision(2) << salariu << " RON";
}

ostream& operator<<(ostream& out, const Angajat& a) {
    out << a.GetRol() << " " << a.nume << " (@" << a.username << ")";
    return out;
}
