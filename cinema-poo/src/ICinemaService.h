#pragma once
#include <string>
#include <vector>

class Film;
class Sala;
class Spectacol;
class Rezervare;
class Persoana;

// ICinemaService - interfata abstracta pentru serviciile de baza ale unui cinema.
// Orice implementare (Cinematograf, CinemaOnline, etc.) trebuie sa ofere
// aceste operatii. Demonstreaza polimorfismul si principiul segregarii interfetelor.
class ICinemaService {
public:
    virtual ~ICinemaService() = default;

    // ---- Filme ----
    virtual void   AdaugaFilm(Film* f) = 0;
    virtual bool   StergeFilm(int id) = 0;
    virtual Film* GasesteFilmDupaID(int id) const = 0;

    // ---- Sali ----
    virtual void  AdaugaSala(Sala* s) = 0;
    virtual bool  StergeSala(int id) = 0;
    virtual Sala* GasesteSalaDupaID(int id) const = 0;

    // ---- Spectacole ----
    virtual Spectacol* CreeazaSpectacol(Film* f, Sala* s,
        const std::string& data, const std::string& ora,
        double pret = -1) = 0;
    virtual bool       StergeSpectacol(int id) = 0;
    virtual Spectacol* GasesteSpectacol(int id) const = 0;

    // ---- Rezervari ----
    virtual Rezervare* FaRezervare(Persoana* p, Spectacol* sp,
        int rand, int col) = 0;
    virtual bool       StergeRezervare(int id) = 0;
    virtual Rezervare* GasesteRezervare(int id) const = 0;

    // ---- Afisari ----
    virtual void AfisareFilme()      const = 0;
    virtual void AfisareSpectacole() const = 0;
    virtual void AfisareRezervari()  const = 0;
};
