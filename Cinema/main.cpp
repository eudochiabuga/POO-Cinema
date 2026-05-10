#include <iostream>
#include <limits>
#include "Cinematograf.h"
#include "TipPersoana.h"
using namespace std;
void Pauza() {
    cout << "\nApasa Enter pentru a continua...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
void AfisareMeniu() {
    cout << "\n╔══════════════════════════════════╗\n";
    cout << "║     SISTEM REZERVARI CINEMA      ║\n";
    cout << "╠══════════════════════════════════╣\n";
    cout << "║  1. Afiseaza filme               ║\n";
    cout << "║  2. Afiseaza sali                ║\n";
    cout << "║  3. Harta sala                   ║\n";
    cout << "║  4. Fa o rezervare               ║\n";
    cout << "║  5. Afiseaza toate rezervarile   ║\n";
    cout << "║  6. Afiseaza incasari            ║\n";
    cout << "║  7. Adauga film nou              ║\n";
    cout << "║  8. Adauga sala noua             ║\n";
    cout << "║  0. Iesire                       ║\n";
    cout << "╚══════════════════════════════════╝\n";
    cout << "  Optiune: ";
}

int CitesteInt(const string& mesaj, int minim, int maxim) {
    int val;
    while (true) {
        cout << mesaj;
        if (cin >> val && val >= minim && val <= maxim)
            return val;
        cout << "  [Eroare] Introduceti un numar intre "
            << minim << " si " << maxim << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string CitesteString(const string& mesaj) {
    string val;
    cout << mesaj;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, val);
    return val;
}


string CitesteStringDirect(const string& mesaj) {
    string val;
    cout << mesaj;
    getline(cin, val);
    return val;
}


void MeniuHartaSala(Cinematograf& cinema) {
    string nr = CitesteString("  Numarul salii (ex: 1, 2, VIP): ");
    cinema.AfisareHartaSala(nr);
}

void MeniuRezervare(Cinematograf& cinema,
    vector<Persoana*>& persoane,
    vector<Film*>& filme,
    vector<Sala*>& sali)
{
    cout << "\n--- Rezervare noua ---\n";

    string nume = CitesteString("  Numele clientului: ");

 
    cout << "  Tip persoana:\n";
    cout << "    1. Adult   (fara discount)\n";
    cout << "    2. Student (discount 20%)\n";
    cout << "    3. Elev    (discount 15%)\n";
    int tip = CitesteInt("  Optiune: ", 1, 3);

    int varsta = CitesteInt("  Varsta clientului: ", 1, 120);

    Persoana* client = nullptr;
    if (tip == 1) client = new Adult(nume, varsta);
    else if (tip == 2) client = new Student(nume, varsta);
    else               client = new Elev(nume, varsta);
    persoane.push_back(client);

    if (filme.empty()) { cout << "  [Eroare] Nu exista filme inregistrate.\n"; return; }
    cout << "\n  Filme disponibile:\n";
    for (int i = 0; i < (int)filme.size(); i++)
        cout << "    " << i + 1 << ". " << *filme[i] << "\n";
    int idxFilm = CitesteInt("  Alege filmul: ", 1, (int)filme.size()) - 1;
    Film* film = filme[idxFilm];

    if (sali.empty()) { cout << "  [Eroare] Nu exista sali inregistrate.\n"; return; }
    cout << "\n  Sali disponibile:\n";
    for (int i = 0; i < (int)sali.size(); i++)
        cout << "    " << i + 1 << ". Sala " << sali[i]->GetNumarSala()
        << " (" << sali[i]->GetRanduri() << " randuri x "
        << sali[i]->GetColoane() << " coloane"
        << (sali[i]->EsteVIP() ? ", VIP" : "") << ")\n";
    int idxSala = CitesteInt("  Alege sala: ", 1, (int)sali.size()) - 1;
    Sala* sala = sali[idxSala];

   
    sala->AfisareHarta();
    int rand = CitesteInt("  Rand    (1-" + to_string(sala->GetRanduri()) + "): ",
        1, sala->GetRanduri()) - 1;
    int coloana = CitesteInt("  Coloana (1-" + to_string(sala->GetColoane()) + "): ",
        1, sala->GetColoane()) - 1;

   
    Rezervare* rez = cinema.FaRezervare(client, film, sala, rand, coloana);
    if (rez) {
        cout << "\n  Biletul tau:\n";
        rez->Afisare();
    }
}

void MeniuAdaugaFilm(Cinematograf& cinema, vector<Film*>& filme) {
    cout << "\n--- Adauga film nou ---\n";

    string titlu = CitesteString("  Titlu: ");
    string categorie = CitesteStringDirect("  Categorie (ex: Actiune, Sci-Fi): ");
    int    durata = CitesteInt("  Durata (minute): ", 1, 500);
    int    varsta = CitesteInt("  Varsta minima: ", 0, 18);
    string limba = CitesteStringDirect("  Limba: ");

    Film* f = new Film(titlu, categorie, durata, varsta, limba);
    filme.push_back(f);
    cinema.AdaugaFilm(f);
}

void MeniuAdaugaSala(Cinematograf& cinema, vector<Sala*>& sali) {
    cout << "\n--- Adauga sala noua ---\n";

    string nr = CitesteString("  Numarul salii: ");
    int randuri = CitesteInt("  Numar randuri: ", 1, 30);
    int coloane = CitesteInt("  Numar coloane: ", 1, 30);
    int cap = randuri * coloane;

    cout << "  Tip sala:\n";
    cout << "    1. Sala normala\n";
    cout << "    2. Sala VIP\n";
    int tip = CitesteInt("  Optiune: ", 1, 2);

    if (tip == 1) {
        Sala* s = new Sala(nr, cap, randuri, coloane);
        sali.push_back(s);
        cinema.AdaugaSala(s);
    }
    else {
        cout << "  Supliment pret VIP (ex: 0.5 pentru +50%): ";
        double supliment;
        cin >> supliment;
        string tipScaun = CitesteString("  Tip scaun (ex: Recliner): ");

        SalaVIP* s = new SalaVIP(nr, cap, randuri, coloane, supliment, tipScaun);

        cout << "  Adauga produse la meniu (linie goala pentru a termina):\n";
        while (true) {
            string produs = CitesteStringDirect("    Produs: ");
            if (produs.empty()) break;
            s->AdaugaProdusMeniu(produs);
        }

        sali.push_back(s);
        cinema.AdaugaSala(s);
    }
}


int main()
{
    Cinematograf cinema("CinePlex", 35.0);
    vector<Persoana*> persoane;
    vector<Film*>     filme;
    vector<Sala*>     sali;

    Sala* sala1 = new Sala("1", 40, 5, 8);
    Sala* sala2 = new Sala("2", 30, 5, 6);
    SalaVIP* salaVIP = new SalaVIP("VIP", 20, 4, 5, 0.5, "Recliner");
    salaVIP->AdaugaProdusMeniu("Popcorn premium");
    salaVIP->AdaugaProdusMeniu("Cocktail fara alcool");
    salaVIP->AdaugaProdusMeniu("Platou de branza");
    sali.push_back(sala1);
    sali.push_back(sala2);
    sali.push_back(salaVIP);
    cinema.AdaugaSala(sala1);
    cinema.AdaugaSala(sala2);
    cinema.AdaugaSala(salaVIP);

    Film* f1 = new Film("Inception", "Sci-Fi", 148, 13, "Engleza");
    Film* f2 = new Film("Minions", "Animatie", 90, 0, "Romana");
    Film* f3 = new Film("John Wick 4", "Actiune", 169, 16, "Engleza");
    Film* f4 = new Film("Dune Part Two", "Sci-Fi", 166, 12, "Engleza");
    filme.push_back(f1); filme.push_back(f2);
    filme.push_back(f3); filme.push_back(f4);
    cinema.AdaugaFilm(f1); cinema.AdaugaFilm(f2);
    cinema.AdaugaFilm(f3); cinema.AdaugaFilm(f4);
    int optiune = -1;
    do {
        AfisareMeniu();
        if (!(cin >> optiune)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            optiune = -1;
        }

        switch (optiune) {
        case 1: cinema.AfisareFilme();     Pauza(); break;
        case 2: cinema.AfisareSali();      Pauza(); break;
        case 3: MeniuHartaSala(cinema);    Pauza(); break;
        case 4: MeniuRezervare(cinema, persoane, filme, sali); Pauza(); break;
        case 5: cinema.AfisareRezervari(); Pauza(); break;
        case 6: cinema.AfisareIncasari();  Pauza(); break;
        case 7: MeniuAdaugaFilm(cinema, filme); Pauza(); break;
        case 8: MeniuAdaugaSala(cinema, sali);  Pauza(); break;
        case 0: cout << "\nLa revedere!\n"; break;
        default:
            cout << "  [Eroare] Optiune invalida. Alege intre 0 si 8.\n";
        }
    } while (optiune != 0);
    for (auto p : persoane) delete p;
    for (auto f : filme)    delete f;
    for (auto s : sali)     delete s;

    return 0;
}
