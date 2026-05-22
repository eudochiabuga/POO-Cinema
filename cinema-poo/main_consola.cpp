// ============================================================
//  main_consola.cpp - Punct de intrare pentru versiunea consola
//
//  UI modern cu:
//   - culori ANSI (cross-platform Windows 10+/Linux/macOS)
//   - chenare Unicode (box drawing characters)
//   - harta sala colorata (verde=liber, rosu=ocupat, magenta=VIP)
//   - banner ASCII la pornire
//
//  Compilare cu g++:
//      g++ -std=c++17 -Wall -Wextra -o cinema main_consola.cpp \
//          Film.cpp Sala.cpp SalaVIP.cpp Spectacol.cpp \
//          Rezervare.cpp RezervareOnline.cpp \
//          Persoana.cpp TipPersoana.cpp \
//          Angajat.cpp TipAngajat.cpp \
//          SistemAutentificare.cpp Cinematograf.cpp \
//          Persistenta.cpp Raport.cpp AppContext.cpp
//
//  Sau pur si simplu:   make
//  Rulare:              ./cinema
// ============================================================

#include "Cinematograf.h"
#include "RezervareOnline.h"
#include "SalaVIP.h"
#include "TipPersoana.h"
#include "Exceptii.h"
#include "UIConsola.h"

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;
using namespace UI;

// ============================================================
//  Afisare colorata pentru filme / sali / spectacole
// ============================================================

void AfisareFilmeColorat(const Cinematograf& c) {
    Titlu("FILME DISPONIBILE", CYAN);
    const auto& filme = c.GetFilme();
    if (filme.Vid()) {
        Info("Nu exista filme inca.");
        return;
    }
    cout << "\n";
    cout << "  " << BOLD << GRAY
        << left << setw(4) << "ID"
        << setw(28) << "TITLU"
        << setw(12) << "GEN"
        << setw(8) << "DURATA"
        << setw(6) << "TIP"
        << setw(6) << "VAR+"
        << "RATING" << RESET << "\n";
    HR(76, "─", GRAY);
    for (auto f : filme) {
        // Culoare in functie de rating
        const char* col = f->GetRating() >= 8.0 ? GREEN :
            f->GetRating() >= 6.5 ? YELLOW : WHITE;
        cout << "  "
            << col
            << left << setw(4) << f->GetID()
            << setw(28) << (f->GetTitlu().size() > 25 ? f->GetTitlu().substr(0, 25) + "..." : f->GetTitlu())
            << setw(12) << f->GetCategorie()
            << setw(8) << (to_string(f->GetDurata()) + " min")
            << setw(6) << f->GetTipProiectie()
            << setw(6) << (to_string(f->GetVarstaMinima()) + "+")
            << fixed << setprecision(1) << f->GetRating() << "/10"
            << RESET << "\n";
    }
    cout << "\n";
}

void AfisareSaliColorat(const Cinematograf& c) {
    Titlu("SALI", CYAN);
    const auto& sali = c.GetSali();
    if (sali.Vid()) {
        Info("Nu exista sali configurate.");
        return;
    }
    cout << "\n";
    for (auto s : sali) {
        const char* col = s->EsteVIP() ? MAGENTA : CYAN;
        cout << "  " << col << BOLD << "Sala " << s->GetNumarSala() << RESET
            << " - " << s->GetRanduri() << "x" << s->GetColoane()
            << " (" << s->GetCapacitate() << " locuri)";
        if (s->EsteVIP())
            cout << MAGENTA << "  [VIP +" << (int)(s->GetSuplimentPret() * 100) << "%]" << RESET;
        cout << "\n";
    }
    cout << "\n";
}

void AfisareSpectacoleColorat(const Cinematograf& c) {
    Titlu("SPECTACOLE PROGRAMATE", CYAN);
    const auto& sp = c.GetSpectacole();
    if (sp.Vid()) {
        Info("Nu exista spectacole programate.");
        return;
    }
    cout << "\n";
    cout << "  " << BOLD << GRAY
        << left << setw(5) << "ID"
        << setw(22) << "FILM"
        << setw(8) << "SALA"
        << setw(12) << "DATA"
        << setw(8) << "ORA"
        << setw(10) << "PRET"
        << "LIBERE" << RESET << "\n";
    HR(75, "─", GRAY);
    for (auto x : sp) {
        int total = x->GetRanduri() * x->GetColoane();
        int libere = x->LocuriDisponibile();
        const char* col = (libere == 0) ? RED :
            (libere * 3 < total) ? YELLOW : GREEN;
        cout << "  "
            << col
            << left << setw(5) << x->GetID()
            << setw(22) << (x->GetFilm() ? x->GetFilm()->GetTitlu().substr(0, 19) : "?")
            << setw(8) << (x->GetSala() ? x->GetSala()->GetNumarSala() : "?")
            << setw(12) << x->GetData()
            << setw(8) << x->GetOra()
            << fixed << setprecision(2) << setw(10) << (to_string((int)x->GetPretBaza()) + " RON")
            << libere << "/" << total
            << RESET << "\n";
    }
    cout << "\n";
}

// ============================================================
//  Harta colorata
// ============================================================
void AfisareHartaColorat(const Spectacol* sp) {
    if (!sp) return;
    int R = sp->GetRanduri();
    int C = sp->GetColoane();

    cout << "\n  " << BOLD << CYAN << "Harta locuri - Spectacol #" << sp->GetID()
        << " (" << sp->GetFilm()->GetTitlu()
        << ", Sala " << sp->GetSala()->GetNumarSala() << ")" << RESET << "\n";
    cout << "\n";

    // ECRAN
    cout << "       " << BG_BLUE << BOLD;
    for (int i = 0; i < C * 3 + 2; i++) cout << " ";
    cout << RESET << "\n";
    cout << "       " << BG_BLUE << BOLD;
    int textPos = (C * 3 + 2 - 6) / 2;
    for (int i = 0; i < textPos; i++) cout << " ";
    cout << "ECRAN";
    for (int i = 0; i < C * 3 + 2 - textPos - 5; i++) cout << " ";
    cout << RESET << "\n";
    cout << "       " << BG_BLUE << BOLD;
    for (int i = 0; i < C * 3 + 2; i++) cout << " ";
    cout << RESET << "\n\n";

    // Header coloane
    cout << "       ";
    for (int j = 0; j < C; j++)
        cout << GRAY << setw(3) << j + 1 << RESET;
    cout << "\n";

    // Randuri
    for (int i = 0; i < R; i++) {
        cout << "   " << GRAY << "R" << setw(2) << i + 1 << RESET << " ";
        bool randVIP = sp->EsteVIP(i);
        for (int j = 0; j < C; j++) {
            if (!sp->LocLiber(i, j)) {
                cout << RED << " ■ " << RESET;          // ocupat
            }
            else if (randVIP) {
                cout << MAGENTA << " ♛ " << RESET;      // VIP liber
            }
            else {
                cout << GREEN << " □ " << RESET;        // liber
            }
        }
        if (randVIP) cout << "  " << MAGENTA << DIM << "VIP" << RESET;
        cout << "\n";
    }

    cout << "\n  " << BOLD << "Legenda:  " << RESET
        << GREEN << "□" << RESET << " liber   "
        << RED << "■" << RESET << " ocupat   "
        << MAGENTA << "♛" << RESET << " VIP   "
        << GRAY << "(intrare prin spate, randul 1 = cel mai aproape de ecran)" << RESET
        << "\n\n";
}

// ============================================================
//  Date demo
// ============================================================
void IncarcaDemo(Cinematograf& c) {
    c.AdaugaSala(new Sala("A1", 48, 6, 8));
    c.AdaugaSala(new Sala("A2", 30, 5, 6));
    SalaVIP* vip = new SalaVIP("VIP", 20, 4, 5, 0.50, "Recliner");
    vip->AdaugaProdusMeniu("Popcorn premium");
    vip->AdaugaProdusMeniu("Cocktail fara alcool");
    c.AdaugaSala(vip);

    c.AdaugaFilm(new Film("Inception", "Sci-Fi", 148, 13,
        "Engleza", "Christopher Nolan", 2010, 8.8, "", "2D"));
    c.AdaugaFilm(new Film("Avatar 2", "Sci-Fi", 192, 12,
        "Engleza", "James Cameron", 2022, 7.6, "", "3D"));
    c.AdaugaFilm(new Film("Minions", "Animatie", 90, 0,
        "Romana", "Kyle Balda", 2015, 6.4, "", "2D"));
    c.AdaugaFilm(new Film("Dune Part 2", "Sci-Fi", 166, 12,
        "Engleza", "Denis Villeneuve", 2024, 8.6, "", "3D"));
    c.AdaugaFilm(new Film("Oppenheimer", "Drama", 180, 15,
        "Engleza", "Christopher Nolan", 2023, 8.4, "", "2D"));

    auto& filme = c.GetFilmeRef();
    auto& sali = c.GetSaliRef();
    c.CreeazaSpectacol(filme[0], sali[0], "2026-05-22", "18:00", 35.0); // Vineri
    c.CreeazaSpectacol(filme[1], sali[2], "2026-05-22", "20:00", 55.0); // Vineri @ VIP
    c.CreeazaSpectacol(filme[2], sali[1], "2026-05-23", "11:00", 25.0); // Sambata
    c.CreeazaSpectacol(filme[3], sali[0], "2026-05-23", "21:00", 40.0); // Sambata
    c.CreeazaSpectacol(filme[4], sali[1], "2026-05-24", "19:30", 38.0); // Duminica
    c.CreeazaSpectacol(filme[0], sali[0], "2026-05-26", "20:00", 32.0); // Marti

    Succes("Date demo incarcate: " + to_string(filme.Marime()) + " filme, "
        + to_string(sali.Marime()) + " sali, "
        + to_string(c.GetSpectacole().Marime()) + " spectacole.");
}

// ============================================================
//  Optiuni de meniu
// ============================================================

Persoana* CitesteClient() {
    string nume = CitesteString("Nume client");
    int varsta = CitesteInt("Varsta", 1, 120);
    cout << "  " << GRAY << "Tip client: 1=Adult, 2=Student, 3=Elev, 4=Pensionar, 5=Copil"
        << RESET << "\n";
    int tip = CitesteInt("Optiune", 1, 5);
    switch (tip) {
    case 1: return new Adult(nume, varsta);
    case 2: return new Student(nume, varsta);
    case 3: return new Elev(nume, varsta);
    case 4: return new Pensionar(nume, varsta);
    case 5: return new Copil(nume, varsta);
    }
    return new Adult(nume, varsta);
}

void OptHartaSala(Cinematograf& c) {
    AfisareSpectacoleColorat(c);
    int id = CitesteInt("ID spectacol (0 = inapoi)", 0);
    if (id == 0) return;
    Spectacol* sp = c.GasesteSpectacol(id);
    if (!sp) { Eroare("Spectacol negasit."); return; }
    AfisareHartaColorat(sp);
}

void OptRezervare(Cinematograf& c, bool online) {
    Titlu(online ? "REZERVARE ONLINE" : "REZERVARE BILET", MAGENTA);

    AfisareSpectacoleColorat(c);
    int idSp = CitesteInt("ID spectacol", 1);
    Spectacol* sp = c.GasesteSpectacol(idSp);
    if (!sp) { Eroare("Spectacol negasit."); return; }
    AfisareHartaColorat(sp);

    Persoana* client = CitesteClient();
    string email;
    if (online) {
        email = CitesteString("Email");
        client->SetEmail(email);
    }
    c.AdaugaClient(client);

    int rand = CitesteInt("Rand (1-" + to_string(sp->GetRanduri()) + ")",
        1, sp->GetRanduri()) - 1;
    int col = CitesteInt("Coloana (1-" + to_string(sp->GetColoane()) + ")",
        1, sp->GetColoane()) - 1;

    try {
        if (online) {
            sp->RezervaLoc(rand, col);
            RezervareOnline* ro = new RezervareOnline(client, sp, rand, col, email);
            c.GetRezervariRef().Adauga(ro);
            cout << "\n";
            Succes("Rezervare ONLINE confirmata!");
            cout << "\n"; ro->Afisare(); cout << "\n";
            Info("Confirmare trimisa la: " + email);
            Info("Cod confirmare: " + ro->GetCodConfirmare());
        }
        else {
            Rezervare* r = c.FaRezervare(client, sp, rand, col);
            cout << "\n";
            Succes("Rezervare confirmata!");
            cout << "\n"; r->Afisare(); cout << "\n";
        }
    }
    catch (LocOcupatException& e) { Eroare(e.what()); }
    catch (LocInvalidException& e) { Eroare(e.what()); }
    catch (VarstaInsuficientaException& e) { Eroare(e.what()); }
    catch (CinemaException& e) { Eroare(e.what()); }
}

void OptIncasari(Cinematograf& c) {
    Titlu("INCASARI", YELLOW);
    const auto& rez = c.GetRezervari();
    if (rez.Vid()) { Info("Nicio rezervare emisa inca."); return; }
    cout << "\n";
    cout << "  " << BOLD << GRAY
        << left << setw(6) << "ID"
        << setw(20) << "CLIENT"
        << setw(8) << "TIP"
        << setw(22) << "FILM"
        << "PRET FINAL" << RESET << "\n";
    HR(70, "─", GRAY);
    double total = 0;
    for (auto r : rez) {
        cout << "  "
            << left << setw(6) << ("#" + to_string(r->GetID()))
            << setw(20) << r->GetNumeClient().substr(0, 19)
            << setw(8) << r->GetTipClient()
            << setw(22) << r->GetTitluFilm().substr(0, 19)
            << GREEN << fixed << setprecision(2) << r->GetPretFinal() << " RON" << RESET
            << "\n";
        total += r->GetPretFinal();
    }
    HR(70, "─", GRAY);
    cout << "  " << BOLD << "TOTAL: " << GREEN << fixed << setprecision(2)
        << total << " RON" << RESET << "\n\n";
}

void OptRezervari(Cinematograf& c) {
    Titlu("TOATE REZERVARILE", CYAN);
    const auto& rez = c.GetRezervari();
    if (rez.Vid()) { Info("Nicio rezervare."); return; }
    cout << "\n";
    for (auto r : rez)
        cout << "  " << *r << "\n";
    cout << "\n";
}

void OptDemoExceptii(Cinematograf& c) {
    Titlu("DEMO EXCEPTII", RED);
    const auto& spectacole = c.GetSpectacole().GetToate();
    if (spectacole.empty()) { Eroare("Nu exista spectacole."); return; }
    Spectacol* sp = spectacole[0];

    Adult* test = new Adult("Demo User", 30);
    c.AdaugaClient(test);

    cout << "\n" << BOLD << "  1. Rezervare valida (rand 1, col 1):" << RESET << "\n";
    try {
        Rezervare* r = c.FaRezervare(test, sp, 0, 0);
        Succes("Rezervare reusita: " + r->ToString());
    }
    catch (CinemaException& e) { Eroare(e.what()); }

    cout << "\n" << BOLD << "  2. Acelasi loc din nou (LocOcupatException):" << RESET << "\n";
    try { c.FaRezervare(test, sp, 0, 0); }
    catch (LocOcupatException& e) { Avertizare(string("EXCEPTIE prinsa: ") + e.what()); }

    cout << "\n" << BOLD << "  3. Loc in afara salii (LocInvalidException):" << RESET << "\n";
    try { c.FaRezervare(test, sp, 999, 999); }
    catch (LocInvalidException& e) { Avertizare(string("EXCEPTIE prinsa: ") + e.what()); }

    cout << "\n" << BOLD << "  4. Minor la film 13+ (VarstaInsuficientaException):" << RESET << "\n";
    Elev* copil = new Elev("Copil Test", 10);
    c.AdaugaClient(copil);
    try { c.FaRezervare(copil, sp, 1, 0); }
    catch (VarstaInsuficientaException& e) { Avertizare(string("EXCEPTIE prinsa: ") + e.what()); }

    cout << "\n";
    Succes("Toate exceptiile au fost capturate corect.");
}

// ============================================================
//  Meniu principal
// ============================================================
void AfisareMeniu(const Cinematograf& c) {
    cout << "\n";
    HRDouble(60);
    cout << "  " << BOLD << CYAN << c.GetNume() << RESET
        << GRAY << " - " << c.GetAdresa() << ", " << c.GetOras() << RESET << "\n";
    HRDouble(60);
    cout << "  " << GREEN << "1." << RESET << " Vezi filmele disponibile\n";
    cout << "  " << GREEN << "2." << RESET << " Vezi salile cinematografului\n";
    cout << "  " << GREEN << "3." << RESET << " Vezi spectacolele programate\n";
    cout << "  " << GREEN << "4." << RESET << " Harta locuri (libere / ocupate)\n";
    cout << "  " << YELLOW << "5." << RESET << " Rezerva bilet\n";
    cout << "  " << YELLOW << "6." << RESET << " Rezerva " << BOLD << "online" << RESET << " (cu email)\n";
    cout << "  " << CYAN << "7." << RESET << " Vezi toate rezervarile\n";
    cout << "  " << CYAN << "8." << RESET << " Vezi incasarile\n";
    cout << "  " << MAGENTA << "9." << RESET << " Demo: " << ITALIC << "exceptii in actiune" << RESET << "\n";
    cout << "  " << RED << "0." << RESET << " Iesire\n";
    HR(60, "─", GRAY);
}

int main() {
    EnableAnsi();
    Banner();

    Cinematograf cinema("CinePlex Iasi", 35.0, "Str. Mihai Eminescu nr. 25", "Iasi");

    cout << "  " << GRAY << "Se incarca date demo..." << RESET << "\n";
    IncarcaDemo(cinema);

    int optiune = -1;
    while (optiune != 0) {
        AfisareMeniu(cinema);
        optiune = CitesteInt("Optiune", 0, 9);

        switch (optiune) {
        case 1: AfisareFilmeColorat(cinema);       break;
        case 2: AfisareSaliColorat(cinema);        break;
        case 3: AfisareSpectacoleColorat(cinema);  break;
        case 4: OptHartaSala(cinema);              break;
        case 5: OptRezervare(cinema, false);       break;
        case 6: OptRezervare(cinema, true);        break;
        case 7: OptRezervari(cinema);              break;
        case 8: OptIncasari(cinema);               break;
        case 9: OptDemoExceptii(cinema);           break;
        case 0:
            cout << "\n  " << CYAN << BOLD
                << "Va multumim ca ati ales " << cinema.GetNume() << "!" << RESET
                << "\n\n";
            break;
        default:
            Eroare("Optiune invalida.");
        }
    }
    return 0;
}

  