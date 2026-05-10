#include <string>
#include <iostream>
#include "Film.h"
using namespace std;

Film::Film(string titlu, string categorie, int durata, int varstaMinima, string limba)
{
	Titlu = titlu;
	Categorie = categorie;
	Durata = durata;
	VarstaMinima = varstaMinima;
	Limba = limba;
}

Film::Film(const Film& f) = default;
Film& Film::operator=(const Film& f) = default;

string Film::GetTitlu()      const { return Titlu; }
string Film::GetCategorie()  const { return Categorie; }
int    Film::GetDurata()     const { return Durata; }
int    Film::GetVarstaMinima() const { return VarstaMinima; }
string Film::GetLimba()      const { return Limba; }

void Film::SetLimba(string limba) { Limba = limba; }

void Film::Afisare() const {
    cout << "Titlu:         " << Titlu << endl;
    cout << "Categorie:     " << Categorie << endl;
    cout << "Durata:        " << Durata << " minute" << endl;
    cout << "Varsta minima: " << VarstaMinima << "+" << endl;
    cout << "Limba:         " << Limba << endl;
}

ostream& operator<<(ostream& out, const Film& f) {
    out << f.Titlu << " [" << f.Categorie << ", "
        << f.Durata << " min, " << f.VarstaMinima << "+, "
        << f.Limba << "]";
    return out;
}
