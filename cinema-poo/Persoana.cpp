#include "Persoana.h"

int Persoana::GenerareID = 1;

Persoana::Persoana(string n, int v, string email, string telefon)
    : nume(n), varsta(v), email(email), telefon(telefon) {
    id = GenerareID++;
}

Persoana::~Persoana() {}

void Persoana::Afisare() const {
    cout << "Persoana #" << id
        << " | " << nume
        << " | " << GetTip()
        << " | varsta " << varsta;
    if (!email.empty())   cout << " | " << email;
    if (!telefon.empty()) cout << " | " << telefon;
    cout << endl;
}

ostream& operator<<(ostream& out, const Persoana& p) {
    out << p.GetTip() << " " << p.nume << " (" << p.varsta << " ani)";
    return out;
}
