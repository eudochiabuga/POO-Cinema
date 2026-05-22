#include "SalaVIP.h"
#include <iomanip>
#include <sstream>
#include <algorithm>

SalaVIP::SalaVIP(string sala, int capacitate, int randuri, int coloane,
    double suplimentPret, string tipScaun)
    : Sala(sala, capacitate, randuri, coloane),
    SuplimentPret(suplimentPret), TipScaun(tipScaun) {
}

SalaVIP::SalaVIP(const SalaVIP& s)
    : Sala(s), MeniuDisponibil(s.MeniuDisponibil),
    SuplimentPret(s.SuplimentPret), TipScaun(s.TipScaun) {
}

SalaVIP& SalaVIP::operator=(const SalaVIP& s) {
    if (this != &s) {
        Sala::operator=(s);
        MeniuDisponibil = s.MeniuDisponibil;
        SuplimentPret = s.SuplimentPret;
        TipScaun = s.TipScaun;
    }
    return *this;
}

SalaVIP::~SalaVIP() {}

void SalaVIP::Afisare() const {
    cout << "=== Sala VIP ===" << endl;
    Sala::Afisare();
    cout << "  Tip scaun:  " << TipScaun << endl;
    cout << "  Supliment:  +" << SuplimentPret * 100 << "%" << endl;
    AfisareMeniu();
}

void SalaVIP::AdaugaProdusMeniu(const string& produs) {
    if (!produs.empty()) MeniuDisponibil.push_back(produs);
}

void SalaVIP::StergeProdusMeniu(const string& produs) {
    MeniuDisponibil.erase(
        remove(MeniuDisponibil.begin(), MeniuDisponibil.end(), produs),
        MeniuDisponibil.end());
}

void SalaVIP::StergeMeniu() { MeniuDisponibil.clear(); }

void SalaVIP::AfisareMeniu() const {
    cout << "  Meniu sala " << GetNumarSala() << ":" << endl;
    for (auto& p : MeniuDisponibil)
        cout << "    - " << p << endl;
}

void SalaVIP::AfisareHarta() const {
    cout << "\n  Harta salii VIP " << GetNumarSala() << ":\n";
    cout << "     ";
    for (int j = 0; j < NumarColoane; j++)
        cout << setw(3) << j + 1;
    cout << "\n";
    for (int i = 0; i < NumarRanduri; i++) {
        cout << setw(3) << i + 1 << " ";
        for (int j = 0; j < NumarColoane; j++) {
            if (Matrice[i][j] == 1)
                cout << " [X]";
            else
                cout << " [R]";
        }
        cout << "\n";
    }
    cout << "  Legenda: [R]=recliner liber  [X]=ocupat\n\n";
}

string SalaVIP::Descriere() const {
    ostringstream os;
    os << "Sala VIP " << GetNumarSala() << " - "
        << NumarRanduri << "x" << NumarColoane
        << " " << TipScaun
        << " (+" << (int)(SuplimentPret * 100) << "%)";
    return os.str();
}

double SalaVIP::GetSuplimentPret() const { return SuplimentPret; }
bool   SalaVIP::EsteVIP()          const { return true; }

ostream& operator<<(ostream& out, const SalaVIP& s) {
    out << "Sala VIP " << s.GetNumarSala()
        << " (" << s.GetRanduri() << "x" << s.GetColoane()
        << "), supliment +" << s.SuplimentPret * 100
        << "%, scaune: " << s.TipScaun;
    return out;
}
