#pragma once
#include <exception>
#include <string>
#include <stdexcept>
using namespace std;

// Clasa de baza pentru toate exceptiile aplicatiei
class CinemaException : public exception {
protected:
    string mesaj;
public:
    CinemaException(const string& m) : mesaj(m) {}
    const char* what() const noexcept override { return mesaj.c_str(); }
    virtual string GetMesaj() const { return mesaj; }
    virtual ~CinemaException() {}
};

// Locul a fost deja rezervat
class LocOcupatException : public CinemaException {
    int rand, coloana;
public:
    LocOcupatException(int r, int c)
        : CinemaException("Locul (rand " + to_string(r + 1)
            + ", coloana " + to_string(c + 1) + ") este deja ocupat."),
        rand(r), coloana(c) {
    }
    int GetRand() const { return rand; }
    int GetColoana() const { return coloana; }
};

// Clientul nu are varsta minima pentru film
class VarstaInsuficientaException : public CinemaException {
    int varstaClient, varstaMinima;
public:
    VarstaInsuficientaException(int vc, int vm)
        : CinemaException("Varsta clientului (" + to_string(vc)
            + ") este sub varsta minima a filmului ("
            + to_string(vm) + "+)."),
        varstaClient(vc), varstaMinima(vm) {
    }
    int GetVarstaClient() const { return varstaClient; }
    int GetVarstaMinima() const { return varstaMinima; }
};

// Login esuat
class AutentificareException : public CinemaException {
public:
    AutentificareException(const string& m = "Utilizator sau parola incorecta.")
        : CinemaException(m) {
    }
};

// Drepturi insuficiente (utilizator obisnuit incearca actiune de manager)
class DrepturiInsuficienteException : public CinemaException {
public:
    DrepturiInsuficienteException(const string& m = "Nu aveti drepturi pentru aceasta actiune.")
        : CinemaException(m) {
    }
};

// Spectacol / Film / Sala nu a fost gasit
class ElementInexistentException : public CinemaException {
public:
    ElementInexistentException(const string& tip, const string& id)
        : CinemaException(tip + " cu identificatorul \"" + id + "\" nu exista.") {
    }
};

// Eroare la salvare/incarcare fisier
class FisierException : public CinemaException {
public:
    FisierException(const string& numeFisier, const string& detalii)
        : CinemaException("Eroare fisier \"" + numeFisier + "\": " + detalii) {
    }
};

// Date invalide (de exemplu, durata negativa, capacitate zero, etc.)
class DateInvalideException : public CinemaException {
public:
    DateInvalideException(const string& m)
        : CinemaException("Date invalide: " + m) {
    }
};

// Loc in afara dimensiunilor salii
class LocInvalidException : public CinemaException {
public:
    LocInvalidException(int r, int c, int maxR, int maxC)
        : CinemaException("Locul (rand " + to_string(r + 1)
            + ", coloana " + to_string(c + 1)
            + ") este in afara salii (max "
            + to_string(maxR) + "x" + to_string(maxC) + ").") {
    }
};
