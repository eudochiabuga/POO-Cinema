#include "Rezervare.h"
#include "SalaVIP.h"
#include <iostream>
#include <iomanip>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

int Rezervare::GenerareID = 1;

Rezervare::Rezervare(Persoana* c, Film* f, Sala* s,
    int r, int col, double baza)
    : client(c), film(f), sala(s),
    rand(r), coloana(col), pretBaza(baza)
{
    idRezervare = GenerareID++;
    pretFinal = CalculeazaPret();
}

bool Rezervare::EsteVineri() const {
    time_t t = time(nullptr);
    tm* zi = localtime(&t);
    return zi->tm_wday == 5;
}

double Rezervare::CalculeazaPret() const {
    double pret = pretBaza;
    pret *= (1.0 - client->GetDiscount());
    if (!sala->EsteVIP() && sala->EsteLocVIP(rand))
        pret *= 1.30;
    if (sala->EsteVIP())
        pret *= (1.0 + sala->GetSuplimentPret());
    if (EsteVineri())
        pret *= 0.5;

    return pret;
}

void Rezervare::Afisare() const {
    cout << "+---------------------------------+" << endl;
    cout << "|  BILET #" << left << setw(25) << idRezervare << "|" << endl;
    cout << "+---------------------------------+" << endl;
    cout << "| Client: " << left << setw(24) << client->GetNume() << "|" << endl;
    cout << "| Tip:    " << left << setw(24) << client->GetTip() << "|" << endl;
    cout << "| Film:   " << left << setw(24) << film->GetTitlu() << "|" << endl;
    cout << "| Sala:   " << left << setw(24) << sala->GetNumarSala() << "|" << endl;

    string loc = "Rand " + to_string(rand + 1) + ", Col " + to_string(coloana + 1);
    if (!sala->EsteVIP() && sala->EsteLocVIP(rand)) loc += " (VIP)";
    cout << "| Loc:    " << left << setw(24) << loc << "|" << endl;

    cout << "+---------------------------------+" << endl;
    cout << fixed << setprecision(2);
    cout << "| Pret baza:  " << setw(7) << pretBaza << " RON          |" << endl;
    if (client->GetDiscount() > 0)
        cout << "| Discount:  -" << setw(3) << (int)(client->GetDiscount() * 100)
        << "%                   |" << endl;
    if (!sala->EsteVIP() && sala->EsteLocVIP(rand))
        cout << "| Loc VIP:   +30%                 |" << endl;
    if (sala->EsteVIP())
        cout << "| Sala VIP:  +" << setw(3) << (int)(sala->GetSuplimentPret() * 100)
        << "%                   |" << endl;
    if (EsteVineri())
        cout << "| Vineri:    -50%                 |" << endl;
    cout << "| TOTAL:      " << setw(7) << pretFinal << " RON          |" << endl;
    cout << "+---------------------------------+" << endl;
}

int    Rezervare::GetID()         const { return idRezervare; }
double Rezervare::GetPretFinal()  const { return pretFinal; }
string Rezervare::GetNumeClient() const { return client->GetNume(); }
string Rezervare::GetTitluFilm()  const { return film->GetTitlu(); }

