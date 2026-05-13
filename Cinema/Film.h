#pragma once
#include <string>
#include <iostream>
using namespace std;

class Film
{
    static int GenerareID;
    int id;
    string Titlu;
    string Categorie;
    int Durata;            // minute
    int VarstaMinima;
    string Limba;
    string Regizor;
    int An;                // anul de productie
    double Rating;         // 0..10 (IMDB-style)
    string Poster;         // path catre imagine sau URL (optional)

public:
    Film(string titlu = "", string categorie = "", int durata = 0,
        int varstaMinima = 0, string limba = "Romana",
        string regizor = "", int an = 2024, double rating = 0.0,
        string poster = "");
    Film(const Film& f);
    Film& operator=(const Film& f);
    ~Film();

    int    GetID()            const { return id; }
    string GetTitlu()         const { return Titlu; }
    string GetCategorie()     const { return Categorie; }
    int    GetDurata()        const { return Durata; }
    int    GetVarstaMinima()  const { return VarstaMinima; }
    string GetLimba()         const { return Limba; }
    string GetRegizor()       const { return Regizor; }
    int    GetAn()            const { return An; }
    double GetRating()        const { return Rating; }
    string GetPoster()        const { return Poster; }

    void SetTitlu(const string& t) { Titlu = t; }
    void SetCategorie(const string& c) { Categorie = c; }
    void SetDurata(int d) { Durata = d; }
    void SetVarstaMinima(int v) { VarstaMinima = v; }
    void SetLimba(const string& l) { Limba = l; }
    void SetRegizor(const string& r) { Regizor = r; }
    void SetAn(int a) { An = a; }
    void SetRating(double r) { Rating = r; }
    void SetPoster(const string& p) { Poster = p; }

    void Afisare() const;
    string ToString() const;
    string Descriere() const;     // mai detaliat, pentru UI

    bool operator==(const Film& f) const { return Titlu == f.Titlu && An == f.An; }
    bool operator<(const Film& f) const { return Titlu < f.Titlu; }

    friend ostream& operator<<(ostream& out, const Film& f);

    // Pentru persistenta
    void SetID(int newId) { id = newId; if (newId >= GenerareID) GenerareID = newId + 1; }
    static int GetNextID() { return GenerareID; }
    static void SetNextID(int v) { GenerareID = v; }
};
