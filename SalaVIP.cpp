#include "SalaVIP.h"
#include <iomanip>

SalaVIP::SalaVIP(string sala, int capacitate, int randuri, int coloane,
    double suplimentPret, string tipScaun)
    : Sala(sala, capacitate, randuri, coloane),
    SuplimentPret(suplimentPret), TipScaun(tipScaun) {
}

SalaVIP::~SalaVIP() {}

void SalaVIP::Afisare() const {
    cout << "=== Sala VIP ===" << endl;
    Sala::Afisare();
    cout << "Tip scaun:  " << TipScaun << endl;
    cout << "Supliment:  +" << SuplimentPret * 100 << "%" << endl;
    AfisareMeniu();
}

void SalaVIP::AdaugaProdusMeniu(string produs) {
    MeniuDisponibil.push_back(produs);
}

void SalaVIP::AfisareMeniu() const {
    cout << "Meniu sala " << GetNumarSala() << ":" << endl;
    for (auto& p : MeniuDisponibil)
        cout << "  - " << p << endl;
}



void SalaVIP::AfisareHarta() const {
    cout << "\n  Harta salii VIP " << GetNumarSala() << ":\n";
    cout << "    ";
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

double SalaVIP::GetSuplimentPret() const { return SuplimentPret; }
bool   SalaVIP::EsteVIP()          const { return true; }

ostream& operator<<(ostream& out, const SalaVIP& s) {
    out << "Sala VIP " << s.GetNumarSala()
        << " (" << s.GetRanduri() << "x" << s.GetColoane()
        << "), supliment +" << s.SuplimentPret * 100
        << "%, scaune: " << s.TipScaun;
    return out;
}
