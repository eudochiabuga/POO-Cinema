#include "Sala.h"
#include <iomanip>
#include <sstream>

int Sala::GenerareID = 1;

Sala::Sala(string sala, int capacitate, int numarRanduri, int numarColoane)
    : NumarSala(sala), Capacitate(capacitate),
    NumarRanduri(numarRanduri), NumarColoane(numarColoane) {
    id = GenerareID++;
    AlocaMatrice();
}

Sala::Sala(const Sala& s)
    : id(s.id), NumarSala(s.NumarSala), Capacitate(s.Capacitate),
    NumarRanduri(s.NumarRanduri), NumarColoane(s.NumarColoane) {
    AlocaMatrice();
    for (int i = 0; i < NumarRanduri; i++)
        for (int j = 0; j < NumarColoane; j++)
            Matrice[i][j] = s.Matrice[i][j];
}

Sala& Sala::operator=(const Sala& s) {
    if (this != &s) {
        ElibereazaMatrice();
        id = s.id;
        NumarSala = s.NumarSala;
        Capacitate = s.Capacitate;
        NumarRanduri = s.NumarRanduri;
        NumarColoane = s.NumarColoane;
        AlocaMatrice();
        for (int i = 0; i < NumarRanduri; i++)
            for (int j = 0; j < NumarColoane; j++)
                Matrice[i][j] = s.Matrice[i][j];
    }
    return *this;
}

Sala::~Sala() { ElibereazaMatrice(); }

void Sala::AlocaMatrice() {
    if (NumarRanduri > 0 && NumarColoane > 0) {
        Matrice = new int* [NumarRanduri];
        for (int i = 0; i < NumarRanduri; i++) {
            Matrice[i] = new int[NumarColoane];
            for (int j = 0; j < NumarColoane; j++)
                Matrice[i][j] = 0;
        }
    }
    else {
        Matrice = nullptr;
    }
}

void Sala::ElibereazaMatrice() {
    if (Matrice) {
        for (int i = 0; i < NumarRanduri; i++)
            delete[] Matrice[i];
        delete[] Matrice;
        Matrice = nullptr;
    }
}

bool Sala::OcupaLoc(int rand, int coloana) {
    if (!VerificaDisponibilitate(rand, coloana)) return false;
    Matrice[rand][coloana] = 1;
    return true;
}

bool Sala::VerificaDisponibilitate(int rand, int coloana) const {
    if (rand < 0 || rand >= NumarRanduri) return false;
    if (coloana < 0 || coloana >= NumarColoane) return false;
    return Matrice[rand][coloana] == 0;
}

bool Sala::EsteLocVIP(int rand) const {
    return rand >= NumarRanduri - 2;
}

void Sala::ReseteazaLocuri() {
    for (int i = 0; i < NumarRanduri; i++)
        for (int j = 0; j < NumarColoane; j++)
            Matrice[i][j] = 0;
}

void Sala::Afisare() const {
    cout << "Sala #" << id << "  " << NumarSala << endl;
    cout << "  Randuri:    " << NumarRanduri << endl;
    cout << "  Coloane:    " << NumarColoane << endl;
    cout << "  Capacitate: " << Capacitate << " locuri" << endl;
}

void Sala::AfisareHarta() const {
    cout << "\n  Harta salii " << NumarSala << ":\n";
    cout << "    ";
    for (int j = 0; j < NumarColoane; j++)
        cout << setw(3) << j + 1;
    cout << "\n";
    for (int i = 0; i < NumarRanduri; i++) {
        cout << setw(3) << i + 1 << " ";
        for (int j = 0; j < NumarColoane; j++) {
            if (Matrice[i][j] == 1)
                cout << " [X]";
            else if (EsteLocVIP(i))
                cout << " [V]";
            else
                cout << " [ ]";
        }
        if (EsteLocVIP(i)) cout << "  <- VIP";
        cout << "\n";
    }
    cout << "  Legenda: [ ]=liber  [X]=ocupat  [V]=VIP liber\n\n";
}

string Sala::Descriere() const {
    ostringstream os;
    os << "Sala " << NumarSala << " - " << NumarRanduri << "x" << NumarColoane
        << " (" << Capacitate << " locuri)";
    return os.str();
}

string Sala::GetNumarSala()       const { return NumarSala; }
int    Sala::GetRanduri()         const { return NumarRanduri; }
int    Sala::GetColoane()         const { return NumarColoane; }
double Sala::GetSuplimentPret()   const { return 0.0; }
bool   Sala::EsteVIP()            const { return false; }

ostream& operator<<(ostream& out, const Sala& s) {
    out << "Sala " << s.NumarSala
        << " (" << s.NumarRanduri << "x" << s.NumarColoane
        << ", " << s.Capacitate << " locuri)";
    return out;
}
