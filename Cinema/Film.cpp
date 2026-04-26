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

void Film::Afisare()
{
	cout << "Titlu: " << Titlu << endl;
	cout << "Categorie: " << Categorie << endl;
	cout << "Durata filmului: " << Durata << "minute" << endl;
	cout << "Varsta minima: " << VarstaMinima << "ani" << endl;
	cout << "Limba: " << Limba << endl;
}

Film& Film::operator=(const Film& f)
{
	if (this != &f)
	{
		Titlu = f.Titlu;
		Categorie = f.Categorie;
		Durata = f.Durata;
		VarstaMinima = f.VarstaMinima;
		Limba = f.Limba;
	}
	return *this;
}

void Film::SetLimba(string limba)
{
	Limba = limba;
}
ostream& operator<<(ostream& out, const Film& f)
{
	out << f.Titlu << " (" << f.Categorie << ", "
		<< f.Durata << " min, " << f.VarstaMinima << "+, "
		<< "Limba: " << f.Limba << ")";
	return out;
}