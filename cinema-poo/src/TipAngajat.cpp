#include "TipAngajat.h"

Manager::Manager(string nume, string username, string parola, double salariu)
    : Angajat(nume, username, parola, salariu) {
}
string Manager::GetRol() const { return "Manager"; }
Angajat* Manager::Clone() const { return new Manager(*this); }

Casier::Casier(string nume, string username, string parola, double salariu)
    : Angajat(nume, username, parola, salariu) {
}
string Casier::GetRol() const { return "Casier"; }
Angajat* Casier::Clone() const { return new Casier(*this); }

Administrator::Administrator(string nume, string username, string parola, double salariu)
    : Angajat(nume, username, parola, salariu) {
}
string Administrator::GetRol() const { return "Admin"; }
Angajat* Administrator::Clone() const { return new Administrator(*this); }
