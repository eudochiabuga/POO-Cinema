#include "Spectacol.h"
#include "Exceptii.h"
#include <iomanip>

int Spectacol::GenerareID = 1;

void Spectacol::AlocaOcupat() {
    if (randuri > 0 && coloane > 0) {
        ocupat = new bool* [randuri];
        for (int i = 0; i < randuri; i++) {
            ocupat[i] = new bool[coloane];
            for (int j = 0; j < coloane; j++)
                ocupat[i][j] = false;
        }
    }
    else {
        ocupat = nullptr;
    }
}

void Spectacol::ElibereazaOcupat() {
    if (ocupat) {
        for (int i = 0; i < randuri; i++)
            delete[] ocupat[i];
        delete[] ocupat;
        ocupat = nullptr;
    }
}

Spectacol::Spectacol(Film* f, Sala* s, string data, string ora, double pretBaza)
    : film(f), sala(s), data(data), ora(ora), pretBaza(pretBaza)
{
    if (!f) throw DateInvalideException("Film null pentru spectacol.");
    if (!s) throw DateInvalideException("Sala null pentru spectacol.");
    if (pretBaza < 0) throw DateInvalideException("Pret negativ.");

    id = GenerareID++;
    randuri = s->GetRanduri();
    coloane = s->GetColoane();
    AlocaOcupat();
}

Spectacol::Spectacol(const Spectacol& s)
    : id(s.id), film(s.film), sala(s.sala),
    data(s.data), ora(s.ora), pretBaza(s.pretBaza),
    randuri(s.randuri), coloane(s.coloane)
{
    AlocaOcupat();
    for (int i = 0; i < randuri; i++)
        for (int j = 0; j < coloane; j++)
            ocupat[i][j] = s.ocupat[i][j];
}

Spectacol& Spectacol::operator=(const Spectacol& s) {
    if (this != &s) {
        ElibereazaOcupat();
        id = s.id;
        film = s.film;
        sala = s.sala;
        data = s.data;
        ora = s.ora;
        pretBaza = s.pretBaza;
        randuri = s.randuri;
        coloane = s.coloane;
        AlocaOcupat();
        for (int i = 0; i < randuri; i++)
            for (int j = 0; j < coloane; j++)
                ocupat[i][j] = s.ocupat[i][j];
    }
    return *this;
}

Spectacol::~Spectacol() {
    ElibereazaOcupat();
}

bool Spectacol::LocLiber(int rand, int col) const {
    if (rand < 0 || rand >= randuri) return false;
    if (col < 0 || col >= coloane) return false;
    return !ocupat[rand][col];
}

bool Spectacol::RezervaLoc(int rand, int col) {
    if (rand < 0 || rand >= randuri || col < 0 || col >= coloane)
        throw LocInvalidException(rand, col, randuri, coloane);
    if (ocupat[rand][col])
        throw LocOcupatException(rand, col);
    ocupat[rand][col] = true;
    return true;
}

void Spectacol::ElibereazaLoc(int rand, int col) {
    if (rand >= 0 && rand < randuri && col >= 0 && col < coloane)
        ocupat[rand][col] = false;
}

int Spectacol::LocuriOcupate() const {
    int n = 0;
    for (int i = 0; i < randuri; i++)
        for (int j = 0; j < coloane; j++)
            if (ocupat[i][j]) n++;
    return n;
}

int Spectacol::LocuriDisponibile() const {
    return randuri * coloane - LocuriOcupate();
}

double Spectacol::ProcentOcupare() const {
    int total = randuri * coloane;
    if (total == 0) return 0;
    return 100.0 * LocuriOcupate() / total;
}

bool Spectacol::EsteVIP(int rand) const {
    if (sala->EsteVIP()) return true;     // sala intreaga e VIP
    return rand >= randuri - 2;            // ultimele 2 randuri sunt VIP
}

void Spectacol::Afisare() const {
    cout << "Spectacol #" << id << endl;
    cout << "  Film:  " << (film ? film->GetTitlu() : "(nedefinit)") << endl;
    cout << "  Sala:  " << (sala ? sala->GetNumarSala() : "(nedefinit)") << endl;
    cout << "  Data:  " << data << " " << ora << endl;
    cout << "  Pret:  " << fixed << setprecision(2) << pretBaza << " RON" << endl;
    cout << "  Locuri libere: " << LocuriDisponibile()
        << "/" << randuri * coloane
        << " (" << fixed << setprecision(1) << 100 - ProcentOcupare() << "%)" << endl;
}

void Spectacol::AfisareHarta() const {
    cout << "\n  Harta spectacolului #" << id << " (sala " << sala->GetNumarSala() << "):\n";
    cout << "     ";
    for (int j = 0; j < coloane; j++)
        cout << setw(3) << j + 1;
    cout << "\n";
    for (int i = 0; i < randuri; i++) {
        cout << setw(3) << i + 1 << " ";
        for (int j = 0; j < coloane; j++) {
            if (ocupat[i][j])
                cout << " [X]";
            else if (EsteVIP(i))
                cout << " [V]";
            else
                cout << " [ ]";
        }
        if (EsteVIP(i) && !sala->EsteVIP()) cout << "  <- VIP";
        cout << "\n";
    }
}

bool Spectacol::operator<(const Spectacol& s) const {
    if (data != s.data) return data < s.data;
    return ora < s.ora;
}

ostream& operator<<(ostream& out, const Spectacol& s) {
    out << "[" << s.data << " " << s.ora << "] "
        << (s.film ? s.film->GetTitlu() : "?")
        << " @ Sala " << (s.sala ? s.sala->GetNumarSala() : "?")
        << " - " << fixed << setprecision(2) << s.pretBaza << " RON";
    return out;
}

void Spectacol::MarcheazaOcupat(int rand, int col, bool valoare) {
    if (rand >= 0 && rand < randuri && col >= 0 && col < coloane)
        ocupat[rand][col] = valoare;
}
