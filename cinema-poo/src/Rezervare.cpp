#define _CRT_SECURE_NO_WARNINGS
#include "Rezervare.h"
#include "SalaVIP.h"
#include "Exceptii.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

int Rezervare::GenerareID = 1;

Rezervare::Rezervare(Persoana* c, Spectacol* sp, int r, int col)
    : client(c), spectacol(sp), rand(r), coloana(col)
{
    if (!c) throw DateInvalideException("Client null in rezervare.");
    if (!sp) throw DateInvalideException("Spectacol null in rezervare.");

    Film* f = sp->GetFilm();
    if (c->GetVarsta() < f->GetVarstaMinima())
        throw VarstaInsuficientaException(c->GetVarsta(), f->GetVarstaMinima());

    idRezervare = GenerareID++;
    pretBaza = sp->GetPretBaza();
    pretFinal = CalculeazaPret();

    // Data emiterii biletului (timpul curent)
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", now);
    dataEmiterii = buf;
}

Rezervare::Rezervare(const Rezervare& r)
    : idRezervare(r.idRezervare), client(r.client), spectacol(r.spectacol),
    rand(r.rand), coloana(r.coloana),
    pretBaza(r.pretBaza), pretFinal(r.pretFinal),
    dataEmiterii(r.dataEmiterii) {
}

Rezervare& Rezervare::operator=(const Rezervare& r) {
    if (this != &r) {
        idRezervare = r.idRezervare;
        client = r.client;
        spectacol = r.spectacol;
        rand = r.rand;
        coloana = r.coloana;
        pretBaza = r.pretBaza;
        pretFinal = r.pretFinal;
        dataEmiterii = r.dataEmiterii;
    }
    return *this;
}

// Zile speciale - in functie de DATA SPECTACOLULUI (nu data curenta),
// asa cum cere enuntul "calcul automat al pretului in functie de zi".
// Data are formatul "YYYY-MM-DD". Folosim algoritmul Zeller pentru a calcula
// ziua saptamanii (0=Duminica, 1=Luni, ..., 5=Vineri, 6=Sambata).
static int ZiSaptamana(const std::string& dataYMD) {
    if (dataYMD.size() < 10) return -1;
    int y, m, d;
    try {
        y = std::stoi(dataYMD.substr(0, 4));
        m = std::stoi(dataYMD.substr(5, 2));
        d = std::stoi(dataYMD.substr(8, 2));
    }
    catch (...) { return -1; }
    if (m < 3) { m += 12; y--; }
    int K = y % 100;
    int J = y / 100;
    int h = (d + 13 * (m + 1) / 5 + K + K / 4 + J / 4 + 5 * J) % 7;
    // Zeller: 0=Sambata, 1=Duminica, ..., 6=Vineri
    // Convertim la 0=Duminica, ..., 6=Sambata
    return (h + 6) % 7;
}

bool Rezervare::EsteVineri() const {
    if (!spectacol) return false;
    return ZiSaptamana(spectacol->GetData()) == 5;
}

bool Rezervare::EsteWeekend() const {
    if (!spectacol) return false;
    int zi = ZiSaptamana(spectacol->GetData());
    return zi == 0 || zi == 6;   // Duminica sau Sambata
}

double Rezervare::CalculeazaPret() const {
    double pret = pretBaza;

    // Discount in functie de tipul persoanei (Student 20%, Elev 15%, Adult 0%)
    pret *= (1.0 - client->GetDiscount());

    // Supliment sala VIP sau loc VIP
    Sala* sala = spectacol->GetSala();
    if (!sala->EsteVIP() && spectacol->EsteVIP(rand))
        pret *= 1.30;
    if (sala->EsteVIP())
        pret *= (1.0 + sala->GetSuplimentPret());

    // Supliment 3D (+20%)
    Film* f = spectacol->GetFilm();
    if (f && f->Este3D())
        pret *= 1.20;

    // Reducere vineri 50%, supliment weekend +10%
    if (EsteVineri())       pret *= 0.5;
    else if (EsteWeekend()) pret *= 1.10;

    return pret;
}

string Rezervare::GetTitluFilm() const {
    return (spectacol && spectacol->GetFilm()) ? spectacol->GetFilm()->GetTitlu() : "";
}
string Rezervare::GetNumarSala() const {
    return (spectacol && spectacol->GetSala()) ? spectacol->GetSala()->GetNumarSala() : "";
}
string Rezervare::GetData() const {
    return spectacol ? spectacol->GetData() : "";
}
string Rezervare::GetOra() const {
    return spectacol ? spectacol->GetOra() : "";
}

void Rezervare::Afisare() const {
    cout << "+---------------------------------+" << endl;
    cout << "|  BILET #" << left << setw(25) << idRezervare << "|" << endl;
    cout << "+---------------------------------+" << endl;
    cout << "| Client: " << left << setw(24) << client->GetNume() << "|" << endl;
    cout << "| Tip:    " << left << setw(24) << client->GetTip() << "|" << endl;
    cout << "| Film:   " << left << setw(24) << GetTitluFilm() << "|" << endl;
    cout << "| Sala:   " << left << setw(24) << GetNumarSala() << "|" << endl;
    cout << "| Data:   " << left << setw(24) << (GetData() + " " + GetOra()) << "|" << endl;

    string loc = "Rand " + to_string(rand + 1) + ", Col " + to_string(coloana + 1);
    if (!spectacol->GetSala()->EsteVIP() && spectacol->EsteVIP(rand)) loc += " (VIP)";
    cout << "| Loc:    " << left << setw(24) << loc << "|" << endl;

    cout << "+---------------------------------+" << endl;
    cout << fixed << setprecision(2);
    cout << "| Pret baza:  " << setw(7) << pretBaza << " RON          |" << endl;
    if (client->GetDiscount() > 0)
        cout << "| Discount:  -" << setw(3) << (int)(client->GetDiscount() * 100)
        << "%                   |" << endl;
    if (!spectacol->GetSala()->EsteVIP() && spectacol->EsteVIP(rand))
        cout << "| Loc VIP:   +30%                 |" << endl;
    if (spectacol->GetSala()->EsteVIP())
        cout << "| Sala VIP:  +" << setw(3) << (int)(spectacol->GetSala()->GetSuplimentPret() * 100)
        << "%                   |" << endl;
    if (EsteVineri())
        cout << "| Vineri:    -50%                 |" << endl;
    else if (EsteWeekend())
        cout << "| Weekend:   +10%                 |" << endl;
    cout << "| TOTAL:      " << setw(7) << pretFinal << " RON          |" << endl;
    cout << "+---------------------------------+" << endl;
}

string Rezervare::ToString() const {
    ostringstream os;
    os << fixed << setprecision(2);
    os << "Bilet #" << idRezervare
        << " | " << (client ? client->GetNume() : "?")
        << " | " << GetTitluFilm()
        << " | Sala " << GetNumarSala()
        << " | Rand " << rand + 1 << " Col " << coloana + 1
        << " | " << pretFinal << " RON";
    return os.str();
}

ostream& operator<<(ostream& out, const Rezervare& r) {
    out << r.ToString();
    return out;
}
