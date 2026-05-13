#include "Film.h"
#include <iomanip>
#include <sstream>

int Film::GenerareID = 1;

Film::Film(string titlu, string categorie, int durata, int varstaMinima,
    string limba, string regizor, int an, double rating, string poster)
    : Titlu(titlu), Categorie(categorie), Durata(durata),
    VarstaMinima(varstaMinima), Limba(limba), Regizor(regizor),
    An(an), Rating(rating), Poster(poster)
{
    id = GenerareID++;
}

Film::Film(const Film& f)
    : id(f.id), Titlu(f.Titlu), Categorie(f.Categorie), Durata(f.Durata),
    VarstaMinima(f.VarstaMinima), Limba(f.Limba), Regizor(f.Regizor),
    An(f.An), Rating(f.Rating), Poster(f.Poster) {
}

Film& Film::operator=(const Film& f) {
    if (this != &f) {
        id = f.id;
        Titlu = f.Titlu;
        Categorie = f.Categorie;
        Durata = f.Durata;
        VarstaMinima = f.VarstaMinima;
        Limba = f.Limba;
        Regizor = f.Regizor;
        An = f.An;
        Rating = f.Rating;
        Poster = f.Poster;
    }
    return *this;
}

Film::~Film() {}

void Film::Afisare() const {
    cout << "Film #" << id << endl;
    cout << "  Titlu:         " << Titlu << endl;
    cout << "  Categorie:     " << Categorie << endl;
    cout << "  Regizor:       " << Regizor << endl;
    cout << "  An:            " << An << endl;
    cout << "  Durata:        " << Durata << " min" << endl;
    cout << "  Varsta minima: " << VarstaMinima << "+" << endl;
    cout << "  Limba:         " << Limba << endl;
    cout << "  Rating:        " << fixed << setprecision(1) << Rating << "/10" << endl;
}

string Film::ToString() const {
    ostringstream os;
    os << Titlu << " (" << An << ", " << Categorie << ", " << Durata << " min)";
    return os.str();
}

string Film::Descriere() const {
    ostringstream os;
    os << Titlu << " (" << An << ")\n"
        << "Regizor: " << Regizor << "\n"
        << "Gen: " << Categorie << " | Durata: " << Durata << " min | "
        << VarstaMinima << "+ | " << fixed << setprecision(1) << Rating << "/10";
    return os.str();
}

ostream& operator<<(ostream& out, const Film& f) {
    out << f.Titlu << " [" << f.Categorie << ", "
        << f.Durata << " min, " << f.VarstaMinima << "+, "
        << f.Limba;
    if (!f.Regizor.empty()) out << ", reg. " << f.Regizor;
    if (f.An > 0) out << ", " << f.An;
    out << ", " << fixed << setprecision(1) << f.Rating << "/10]";
    return out;
}
