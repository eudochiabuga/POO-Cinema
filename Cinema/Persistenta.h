#pragma once
#include <string>
using namespace std;

class Cinematograf;
class SistemAutentificare;

// Clasa cu metode statice pentru salvare/incarcare in fisiere text simple.
// Format: "key=value" per linie, sectiuni separate prin "---".
class Persistenta {
public:
    static void SalveazaTot(const Cinematograf& cinema,
        const SistemAutentificare& auth,
        const string& folder);

    static void IncarcaTot(Cinematograf& cinema,
        SistemAutentificare& auth,
        const string& folder);

    // Helper-e individuale - arunca FisierException la eroare
    static void SalveazaFilme(const Cinematograf& cinema, const string& fisier);
    static void IncarcaFilme(Cinematograf& cinema, const string& fisier);

    static void SalveazaSali(const Cinematograf& cinema, const string& fisier);
    static void IncarcaSali(Cinematograf& cinema, const string& fisier);

    static void SalveazaSpectacole(const Cinematograf& cinema, const string& fisier);
    static void IncarcaSpectacole(Cinematograf& cinema, const string& fisier);

    static void SalveazaRezervari(const Cinematograf& cinema, const string& fisier);
    static void IncarcaRezervari(Cinematograf& cinema, const string& fisier);

    static void SalveazaAngajati(const SistemAutentificare& auth, const string& fisier);
    static void IncarcaAngajati(SistemAutentificare& auth, const string& fisier);

    // Verifica daca folder-ul exista; il creeaza daca nu.
    static void AsiguraFolder(const string& folder);
};
