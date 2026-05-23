// ============================================================
//  main_consola.cpp - Punct de intrare pentru versiunea consola
//
//  UI modern cu:
//   - culori ANSI (cross-platform Windows 10+/Linux/macOS)
//   - chenare Unicode (box drawing characters)
//   - harta sala colorata (verde=liber, rosu=ocupat, magenta=VIP)
//   - banner ASCII la pornire
//   - login cu user/parola (admin/manager/casier)
//   - salvare/incarcare automata din /date
//   - rezervare multipla (5 bilete consecutive)
//   - export web (genereaza web/data.js pentru pagina HTML)
//
//  Sau pur si simplu:   make
//  Rulare:              ./cinema
// ============================================================

#include "Cinematograf.h"
#include "RezervareOnline.h"
#include "SalaVIP.h"
#include "TipPersoana.h"
#include "TipAngajat.h"
#include "SistemAutentificare.h"
#include "Persistenta.h"
#include "Raport.h"
#include "Exceptii.h"
#include "UIConsola.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

using namespace std;
using namespace UI;

static const string DATA_FOLDER = "date";

// ============================================================
//  Afisari colorate
// ============================================================

void AfisareFilmeColorat(const Cinematograf& c) {
    Titlu("FILME DISPONIBILE", CYAN);
    const auto& filme = c.GetFilme();
    if (filme.Vid()) { Info("Nu exista filme."); return; }
    cout << "\n  " << BOLD << GRAY
        << left << setw(4) << "ID"
        << setw(34) << "TITLU"
        << setw(14) << "GEN"
        << setw(8) << "DURATA"
        << setw(6) << "TIP"
        << setw(6) << "VAR+"
        << "RATING" << RESET << "\n";
    HR(78, "-", GRAY);
    for (auto f : filme) {
        const char* col = f->GetRating() >= 8.0 ? GREEN : f->GetRating() >= 6.5 ? YELLOW : WHITE;
        cout << "  " << col
            << left << setw(4) << f->GetID()
            << setw(34) << (f->GetTitlu().size() > 31 ? f->GetTitlu().substr(0, 30) + "..." : f->GetTitlu())
            << setw(14) << f->GetCategorie()
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
    if (sali.Vid()) { Info("Nu exista sali."); return; }
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
    if (sp.Vid()) { Info("Nu exista spectacole."); return; }
    cout << "\n  " << BOLD << GRAY
        << left << setw(5) << "ID"
        << setw(28) << "FILM"
        << setw(8) << "SALA"
        << setw(12) << "DATA"
        << setw(8) << "ORA"
        << setw(10) << "PRET"
        << "LIBERE" << RESET << "\n";
    HR(80, "-", GRAY);
    for (auto x : sp) {
        int total = x->GetRanduri() * x->GetColoane();
        int libere = x->LocuriDisponibile();
        const char* col = (libere == 0) ? RED : (libere * 3 < total) ? YELLOW : GREEN;
        cout << "  " << col
            << left << setw(5) << x->GetID()
            << setw(28) << (x->GetFilm()->GetTitlu().size() > 25 ? x->GetFilm()->GetTitlu().substr(0, 24) + "..." : x->GetFilm()->GetTitlu())
            << setw(8) << x->GetSala()->GetNumarSala()
            << setw(12) << x->GetData()
            << setw(8) << x->GetOra()
            << fixed << setprecision(2) << setw(10) << (to_string((int)x->GetPretBaza()) + " RON")
            << libere << "/" << total
            << RESET << "\n";
    }
    cout << "\n";
}

void AfisareHartaColorat(const Spectacol* sp) {
    if (!sp) return;
    int R = sp->GetRanduri();
    int C = sp->GetColoane();
    cout << "\n  " << BOLD << CYAN << "Harta locuri - Spectacol #" << sp->GetID()
        << " (" << sp->GetFilm()->GetTitlu()
        << ", Sala " << sp->GetSala()->GetNumarSala() << ")" << RESET << "\n\n";
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
    cout << "       ";
    for (int j = 0; j < C; j++) cout << GRAY << setw(3) << j + 1 << RESET;
    cout << "\n";
    for (int i = 0; i < R; i++) {
        cout << "   " << GRAY << "R" << setw(2) << i + 1 << RESET << " ";
        bool randVIP = sp->EsteVIP(i);
        for (int j = 0; j < C; j++) {
            if (!sp->LocLiber(i, j))      cout << RED << " X " << RESET;
            else if (randVIP)              cout << MAGENTA << " V " << RESET;
            else                            cout << GREEN << " o " << RESET;
        }
        if (randVIP) cout << "  " << MAGENTA << DIM << "VIP" << RESET;
        cout << "\n";
    }
    cout << "\n  " << BOLD << "Legenda:  " << RESET
        << GREEN << "o" << RESET << " liber   "
        << RED << "X" << RESET << " ocupat   "
        << MAGENTA << "V" << RESET << " VIP\n\n";
}

// ============================================================
//  Date demo extinse (20 filme, 6 sali, 40+ spectacole)
// ============================================================
void IncarcaDemoComplet(Cinematograf& c, SistemAutentificare& auth) {
    // SALI
    c.AdaugaSala(new Sala("A1", 48, 6, 8));
    c.AdaugaSala(new Sala("A2", 30, 5, 6));
    c.AdaugaSala(new Sala("A3", 60, 6, 10));
    c.AdaugaSala(new Sala("A4", 40, 5, 8));
    auto* vip1 = new SalaVIP("VIP1", 20, 4, 5, 0.50, "Recliner");
    vip1->AdaugaProdusMeniu("Popcorn premium");
    vip1->AdaugaProdusMeniu("Cocktail fara alcool");
    vip1->AdaugaProdusMeniu("Nachos cu queso");
    c.AdaugaSala(vip1);
    auto* imax = new SalaVIP("IMAX", 50, 5, 10, 0.80, "Massage");
    imax->AdaugaProdusMeniu("Champagne");
    imax->AdaugaProdusMeniu("Sushi");
    c.AdaugaSala(imax);

    // FILME (20)
    c.AdaugaFilm(new Film("Inception", "Sci-Fi", 148, 13, "Engleza", "Christopher Nolan", 2010, 8.8, "", "2D"));
    c.AdaugaFilm(new Film("Avatar: The Way of Water", "Sci-Fi", 192, 12, "Engleza", "James Cameron", 2022, 7.6, "", "3D"));
    c.AdaugaFilm(new Film("Minions: The Rise of Gru", "Animatie", 90, 0, "Romana", "Kyle Balda", 2022, 6.4, "", "2D"));
    c.AdaugaFilm(new Film("Dune: Part Two", "Sci-Fi", 166, 12, "Engleza", "Denis Villeneuve", 2024, 8.6, "", "3D"));
    c.AdaugaFilm(new Film("Oppenheimer", "Biografic", 180, 15, "Engleza", "Christopher Nolan", 2023, 8.4, "", "2D"));
    c.AdaugaFilm(new Film("Barbie", "Comedie", 114, 12, "Engleza", "Greta Gerwig", 2023, 6.9, "", "2D"));
    c.AdaugaFilm(new Film("Spider-Verse", "Animatie", 140, 7, "Engleza", "Joaquim Dos Santos", 2023, 8.7, "", "3D"));
    c.AdaugaFilm(new Film("John Wick 4", "Actiune", 169, 15, "Engleza", "Chad Stahelski", 2023, 7.7, "", "2D"));
    c.AdaugaFilm(new Film("Guardians 3", "SF", 150, 12, "Engleza", "James Gunn", 2023, 7.9, "", "3D"));
    c.AdaugaFilm(new Film("Super Mario Bros", "Animatie", 92, 0, "Romana", "Aaron Horvath", 2023, 7.0, "", "3D"));
    c.AdaugaFilm(new Film("Mission Impossible 7", "Actiune", 163, 13, "Engleza", "Christopher McQuarrie", 2023, 7.7, "", "2D"));
    c.AdaugaFilm(new Film("Killers of the Flower Moon", "Crime", 206, 15, "Engleza", "Martin Scorsese", 2023, 7.6, "", "2D"));
    c.AdaugaFilm(new Film("Wonka", "Familie", 116, 7, "Engleza", "Paul King", 2023, 7.0, "", "2D"));
    c.AdaugaFilm(new Film("Aquaman 2", "SF", 124, 12, "Engleza", "James Wan", 2023, 5.5, "", "3D"));
    c.AdaugaFilm(new Film("Napoleon", "Istoric", 158, 15, "Engleza", "Ridley Scott", 2023, 6.4, "", "2D"));
    c.AdaugaFilm(new Film("Hunger Games: Songbirds", "Drama", 157, 13, "Engleza", "Francis Lawrence", 2023, 6.7, "", "2D"));
    c.AdaugaFilm(new Film("Frozen 3", "Animatie", 102, 0, "Romana", "Jennifer Lee", 2026, 7.5, "", "3D"));
    c.AdaugaFilm(new Film("The Batman 2", "Actiune", 175, 15, "Engleza", "Matt Reeves", 2026, 8.5, "", "2D"));
    c.AdaugaFilm(new Film("Joker: Folie a Deux", "Drama", 138, 18, "Engleza", "Todd Phillips", 2024, 7.0, "", "2D"));
    c.AdaugaFilm(new Film("Furiosa", "Actiune", 148, 15, "Engleza", "George Miller", 2024, 7.5, "", "2D"));

    // SPECTACOLE (40+ pe 7 zile)
    auto& filme = c.GetFilmeRef();
    auto& sali = c.GetSaliRef();
    const string zile[] = { "2026-05-22", "2026-05-23", "2026-05-24", "2026-05-25",
                            "2026-05-26", "2026-05-27", "2026-05-28" };
    const string ore[] = { "10:00", "13:00", "16:00", "19:00", "21:30" };
    int fIdx = 0;
    for (const auto& zi : zile) {
        for (size_t s = 0; s < sali.Marime(); s++) {
            for (int slot = 0; slot < 2; slot++) {
                Film* f = filme[fIdx % filme.Marime()];
                fIdx++;
                int oraIdx = (s + slot * 2) % 5;
                double pret = 30.0;
                if (f->Este3D()) pret += 10;
                if (sali[s]->EsteVIP()) pret += 15;
                try { c.CreeazaSpectacol(f, sali[s], zi, ore[oraIdx], pret); }
                catch (...) {}
            }
        }
    }

    // CLIENTI
    auto* c1 = new Adult("Andrei Popescu", 28, "andrei@gmail.com");
    auto* c2 = new Student("Maria Ionescu", 21, "maria.s@yahoo.com");
    auto* c3 = new Elev("Mihai Popa", 16, "mihai@elev.ro");
    auto* c4 = new Adult("Cristina Stoica", 32, "cris@outlook.com");
    auto* c5 = new Pensionar("Ion Vasile", 68, "ionv@gmail.com");
    auto* c6 = new Student("Ana Dumitrescu", 22, "ana.d@stud.ro");
    auto* c7 = new Adult("Razvan Marin", 35);
    auto* c8 = new Adult("Elena Tudor", 29, "elena@gmail.com");
    c.AdaugaClient(c1); c.AdaugaClient(c2); c.AdaugaClient(c3); c.AdaugaClient(c4);
    c.AdaugaClient(c5); c.AdaugaClient(c6); c.AdaugaClient(c7); c.AdaugaClient(c8);

    // REZERVARI demo (mix online/casa)
    Persoana* cls[] = { c1, c2, c3, c4, c5, c6, c7, c8 };
    bool onlines[] = { true, true, false, true, false, true, false, true,
                       true, false, false, true, true, false };
    int rezv[][3] = { {0,0,2},{0,0,3},{0,0,4},{1,1,1},{2,2,0},{3,0,0},
                      {4,1,2},{5,0,1},{7,2,3},{8,1,1},{10,0,0},{12,1,2},
                      {15,2,2},{18,0,5} };
    for (size_t i = 0; i < sizeof(rezv)/sizeof(rezv[0]); i++) {
        try {
            auto sp = c.GetSpectacoleRef()[rezv[i][0]];
            if (!sp) continue;
            Persoana* p = cls[i % 8];
            int r = rezv[i][1], col = rezv[i][2];
            sp->RezervaLoc(r, col);
            if (onlines[i]) {
                auto* ro = new RezervareOnline(p, sp, r, col, p->GetEmail().empty() ? "client@cinema.ro" : p->GetEmail());
                c.GetRezervariRef().Adauga(ro);
            } else {
                auto* rr = new Rezervare(p, sp, r, col);
                c.GetRezervariRef().Adauga(rr);
            }
        } catch (...) {}
    }

    auth.IncarcaImpliciti();
}

// ============================================================
//  Login console (cu inregistrare cont nou)
// ============================================================
bool InregistrareCont(SistemAutentificare& auth) {
    Titlu("INREGISTRARE CONT NOU", GREEN);
    cout << "  " << GRAY << "Iti creezi un cont propriu cu numele tau." << RESET << "\n\n";

    string nume = CitesteString("Nume complet (ex: Maria Popescu)");
    if (nume.empty()) { Eroare("Numele nu poate fi gol."); return false; }

    string user = CitesteString("Username dorit          ");
    if (user.empty()) { Eroare("Username-ul nu poate fi gol."); return false; }

    string pass = CitesteString("Parola                  ");
    if (pass.length() < 3) { Eroare("Parola trebuie sa aiba minim 3 caractere."); return false; }

    cout << "  " << GRAY << "Rol cont: 1=Casier  2=Manager  3=Administrator" << RESET << "\n";
    int rol = CitesteInt("Optiune rol             ", 1, 3);

    try {
        Angajat* nou = nullptr;
        switch (rol) {
            case 2: nou = new Manager(nume, user, pass, 6500); break;
            case 3: nou = new Administrator(nume, user, pass, 8000); break;
            default: nou = new Casier(nume, user, pass, 3500); break;
        }
        auth.AdaugaAngajat(nou);
        cout << "\n";
        Succes("Cont creat cu succes! Te loghez automat...");
        auth.Login(user, pass);
        Succes("Bun venit, " + nume + "!");
        return true;
    } catch (CinemaException& e) {
        Eroare(e.what());
        return false;
    }
}

bool LoginConsole(SistemAutentificare& auth) {
    while (true) {
        Titlu("AUTENTIFICARE", MAGENTA);
        cout << "  " << GREEN << "1." << RESET << " Login (cont existent)\n";
        cout << "  " << CYAN  << "2." << RESET << " " << BOLD << "Inregistrare cont nou" << RESET << " (cu numele tau)\n";
        cout << "  " << RED   << "0." << RESET << " Iesire\n\n";

        int opt = CitesteInt("Optiune", 0, 2);
        if (opt == 0) return false;

        if (opt == 2) {
            if (InregistrareCont(auth)) return true;
            cout << "\n";
            continue;
        }

        // Login
        cout << "\n  " << GRAY << "Conturi disponibile (default):" << RESET << "\n";
        cout << "    " << GREEN  << "admin   / admin   " << RESET << GRAY << "(Administrator - drepturi totale)" << RESET << "\n";
        cout << "    " << YELLOW << "manager / manager " << RESET << GRAY << "(Manager - rezervari + rapoarte)" << RESET << "\n";
        cout << "    " << CYAN   << "casier  / casier  " << RESET << GRAY << "(Casier - doar rezervari)" << RESET << "\n";
        cout << "    " << GRAY   << "...sau contul tau personal" << RESET << "\n\n";

        for (int incercari = 0; incercari < 3; incercari++) {
            string user = CitesteString("Username");
            string pass = CitesteString("Parola  ");
            try {
                auto a = auth.Login(user, pass);
                cout << "\n";
                Succes("Bun venit, " + a->GetNume() + " (" + a->GetRol() + ")!");
                return true;
            } catch (AutentificareException& e) {
                Eroare(e.what());
                if (incercari < 2)
                    Info("Mai ai " + to_string(2 - incercari) + " incercari. (Sau " + BOLD + "Ctrl+C" + RESET + " pentru meniu)");
            }
        }
        Avertizare("3 incercari esuate. Te intorc la meniu.");
        cout << "\n";
    }
}

// ============================================================
//  Optiuni meniu
// ============================================================
Persoana* CitesteClient() {
    string nume = CitesteString("Nume client");
    int varsta = CitesteInt("Varsta", 1, 120);
    cout << "  " << GRAY << "Tip: 1=Adult 2=Student 3=Elev 4=Pensionar 5=Copil" << RESET << "\n";
    int tip = CitesteInt("Optiune", 1, 5);
    switch (tip) {
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
    int rand = CitesteInt("Rand (1-" + to_string(sp->GetRanduri()) + ")", 1, sp->GetRanduri()) - 1;
    int col = CitesteInt("Coloana (1-" + to_string(sp->GetColoane()) + ")", 1, sp->GetColoane()) - 1;
    try {
        if (online) {
            sp->RezervaLoc(rand, col);
            auto* ro = new RezervareOnline(client, sp, rand, col, email);
            c.GetRezervariRef().Adauga(ro);
            cout << "\n"; Succes("Rezervare ONLINE confirmata!");
            cout << "\n"; ro->Afisare(); cout << "\n";
            Info("Cod confirmare: " + ro->GetCodConfirmare());
        } else {
            auto* r = c.FaRezervare(client, sp, rand, col);
            cout << "\n"; Succes("Rezervare confirmata!");
            cout << "\n"; r->Afisare(); cout << "\n";
        }
    } catch (CinemaException& e) { Eroare(e.what()); }
}

// REZERVARE MULTIPLA - cauta N locuri consecutive
void OptRezervareMultipla(Cinematograf& c) {
    Titlu("REZERVARE MULTIPLA (BILETE CONSECUTIVE)", MAGENTA);
    AfisareSpectacoleColorat(c);
    int idSp = CitesteInt("ID spectacol", 1);
    Spectacol* sp = c.GasesteSpectacol(idSp);
    if (!sp) { Eroare("Spectacol negasit."); return; }
    AfisareHartaColorat(sp);
    int N = CitesteInt("Cate bilete consecutive (1-10)", 1, 10);
    Persoana* client = CitesteClient();
    string email = CitesteString("Email (opt) ");
    if (!email.empty()) client->SetEmail(email);
    c.AdaugaClient(client);

    int randGasit = -1, colStart = -1;
    for (int r = 0; r < sp->GetRanduri() && randGasit < 0; r++) {
        int consec = 0;
        for (int col = 0; col < sp->GetColoane(); col++) {
            if (sp->LocLiber(r, col)) {
                consec++;
                if (consec == N) {
                    randGasit = r;
                    colStart = col - N + 1;
                    break;
                }
            } else consec = 0;
        }
    }
    if (randGasit < 0) {
        Eroare("Nu am gasit " + to_string(N) + " locuri consecutive pe niciun rand.");
        return;
    }
    Info("Locuri gasite: randul " + to_string(randGasit + 1) +
         ", coloanele " + to_string(colStart + 1) + " - " + to_string(colStart + N));

    vector<Rezervare*> rezervari;
    double total = 0;
    try {
        for (int i = 0; i < N; i++) {
            sp->RezervaLoc(randGasit, colStart + i);
            Rezervare* r;
            if (!email.empty())
                r = new RezervareOnline(client, sp, randGasit, colStart + i, email);
            else
                r = new Rezervare(client, sp, randGasit, colStart + i);
            c.GetRezervariRef().Adauga(r);
            rezervari.push_back(r);
            total += r->GetPretFinal();
        }
        cout << "\n"; Succes("S-au rezervat " + to_string(N) + " bilete pe acelasi rand!");
        cout << "\n";
        for (auto r : rezervari)
            cout << "  Bilet #" << r->GetID() << " - Rand " << r->GetRand() + 1
                 << ", Col " << r->GetColoana() + 1
                 << " - " << GREEN << fixed << setprecision(2) << r->GetPretFinal() << " RON" << RESET << "\n";
        cout << "  " << BOLD << "TOTAL: " << GREEN << fixed << setprecision(2)
             << total << " RON" << RESET << "\n";
    } catch (CinemaException& e) { Eroare(e.what()); }
}

void OptIncasari(Cinematograf& c) {
    Titlu("INCASARI", YELLOW);
    const auto& rez = c.GetRezervari();
    if (rez.Vid()) { Info("Nicio rezervare."); return; }
    double totOnline = 0, totCasa = 0;
    int nOnline = 0, nCasa = 0;
    cout << "\n  " << BOLD << GRAY
        << left << setw(6) << "ID" << setw(20) << "CLIENT" << setw(8) << "TIP"
        << setw(28) << "FILM" << setw(10) << "CANAL" << "PRET" << RESET << "\n";
    HR(78, "-", GRAY);
    for (auto r : rez) {
        bool online = r->EsteOnline();
        if (online) { totOnline += r->GetPretFinal(); nOnline++; }
        else { totCasa += r->GetPretFinal(); nCasa++; }
        cout << "  "
            << left << setw(6) << ("#" + to_string(r->GetID()))
            << setw(20) << r->GetNumeClient().substr(0, 19)
            << setw(8) << r->GetTipClient()
            << setw(28) << r->GetTitluFilm().substr(0, 26)
            << (online ? MAGENTA : CYAN) << setw(10) << (online ? "ONLINE" : "CASA") << RESET
            << GREEN << fixed << setprecision(2) << r->GetPretFinal() << " RON" << RESET << "\n";
    }
    HR(78, "-", GRAY);
    cout << "\n  " << BOLD << "Statistici:" << RESET << "\n";
    cout << "  " << MAGENTA << "Online: " << RESET << nOnline << " bilete = "
         << GREEN << fixed << setprecision(2) << totOnline << " RON" << RESET << "\n";
    cout << "  " << CYAN << "Casa:   " << RESET << nCasa << " bilete = "
         << GREEN << fixed << setprecision(2) << totCasa << " RON" << RESET << "\n";
    cout << "  " << BOLD << "TOTAL:  " << RESET << (nOnline + nCasa) << " bilete = "
         << GREEN << BOLD << fixed << setprecision(2) << (totOnline + totCasa) << " RON" << RESET << "\n\n";
}

void OptRezervari(Cinematograf& c) {
    Titlu("TOATE REZERVARILE", CYAN);
    const auto& rez = c.GetRezervari();
    if (rez.Vid()) { Info("Nicio rezervare."); return; }
    cout << "\n";
    for (auto r : rez) {
        cout << "  " << (r->EsteOnline() ? MAGENTA : CYAN)
             << (r->EsteOnline() ? "[ONLINE]" : "[CASA]  ") << RESET
             << " " << *r << "\n";
    }
    cout << "\n";
}

void OptDemoExceptii(Cinematograf& c) {
    Titlu("DEMO EXCEPTII", RED);
    auto spectacole = c.GetSpectacole().GetToate();
    if (spectacole.empty()) { Eroare("Nu exista spectacole."); return; }
    Spectacol* sp = nullptr;
    for (auto s : spectacole) {
        if (s->LocLiber(0, 0)) { sp = s; break; }
    }
    if (!sp) { Info("Toate locurile (0,0) sunt ocupate."); return; }

    Adult* test = new Adult("Demo User", 30);
    c.AdaugaClient(test);
    cout << "\n" << BOLD << "  1. Rezervare valida (rand 1, col 1):" << RESET << "\n";
    try { auto* r = c.FaRezervare(test, sp, 0, 0); Succes("OK: " + r->ToString()); }
    catch (CinemaException& e) { Eroare(e.what()); }

    cout << "\n" << BOLD << "  2. Acelasi loc → LocOcupatException:" << RESET << "\n";
    try { c.FaRezervare(test, sp, 0, 0); }
    catch (LocOcupatException& e) { Avertizare(string("EXCEPTIE: ") + e.what()); }

    cout << "\n" << BOLD << "  3. Loc invalid → LocInvalidException:" << RESET << "\n";
    try { c.FaRezervare(test, sp, 999, 999); }
    catch (LocInvalidException& e) { Avertizare(string("EXCEPTIE: ") + e.what()); }

    cout << "\n" << BOLD << "  4. Minor → VarstaInsuficientaException:" << RESET << "\n";
    Spectacol* sp13 = nullptr;
    for (auto s : spectacole)
        if (s->GetFilm()->GetVarstaMinima() >= 13) { sp13 = s; break; }
    if (sp13) {
        Elev* copil = new Elev("Copil Test", 10);
        c.AdaugaClient(copil);
        try { c.FaRezervare(copil, sp13, 3, 3); }
        catch (VarstaInsuficientaException& e) { Avertizare(string("EXCEPTIE: ") + e.what()); }
    }
    cout << "\n"; Succes("Toate exceptiile au fost capturate.");
}

// ============================================================
//  Export web (genereaza web/data.js pentru pagina HTML)
// ============================================================
static string EscapeJS(const string& s) {
    string out;
    for (char c : s) {
        if (c == '"' || c == '\\') { out += '\\'; out += c; }
        else if (c == '\n') out += "\\n";
        else out += c;
    }
    return out;
}

void ExportWebData(const Cinematograf& c) {
    fs::create_directories("web");
    ofstream f("web/data.js");
    if (!f.is_open()) { Eroare("Nu pot scrie web/data.js"); return; }

    f << "// AUTO-GENERAT de aplicatia C++ - nu edita manual.\n";
    f << "const CINEMA_DATA = {\n";
    f << "  nume: \"" << EscapeJS(c.GetNume()) << "\",\n";
    f << "  adresa: \"" << EscapeJS(c.GetAdresa()) << "\",\n";
    f << "  oras: \"" << EscapeJS(c.GetOras()) << "\",\n";
    f << "  filme: [\n";
    for (auto film : c.GetFilme()) {
        f << "    { id:" << film->GetID()
          << ", titlu:\"" << EscapeJS(film->GetTitlu()) << "\""
          << ", categorie:\"" << EscapeJS(film->GetCategorie()) << "\""
          << ", durata:" << film->GetDurata()
          << ", varsta:" << film->GetVarstaMinima()
          << ", tip:\"" << film->GetTipProiectie() << "\""
          << ", rating:" << film->GetRating()
          << ", regizor:\"" << EscapeJS(film->GetRegizor()) << "\""
          << ", an:" << film->GetAn() << " },\n";
    }
    f << "  ],\n  sali: [\n";
    for (auto s : c.GetSali()) {
        f << "    { id:" << s->GetID()
          << ", numar:\"" << EscapeJS(s->GetNumarSala()) << "\""
          << ", randuri:" << s->GetRanduri()
          << ", coloane:" << s->GetColoane()
          << ", vip:" << (s->EsteVIP() ? "true" : "false") << " },\n";
    }
    f << "  ],\n  spectacole: [\n";
    for (auto sp : c.GetSpectacole()) {
        f << "    { id:" << sp->GetID()
          << ", filmId:" << sp->GetFilm()->GetID()
          << ", salaId:" << sp->GetSala()->GetID()
          << ", data:\"" << sp->GetData() << "\""
          << ", ora:\"" << sp->GetOra() << "\""
          << ", pret:" << sp->GetPretBaza()
          << ", libere:" << sp->LocuriDisponibile()
          << ", total:" << sp->GetRanduri() * sp->GetColoane()
          << ", harta:[";
        for (int r = 0; r < sp->GetRanduri(); r++) {
            f << "\"";
            for (int col = 0; col < sp->GetColoane(); col++)
                f << (sp->LocLiber(r, col) ? "0" : "1");
            f << "\"";
            if (r < sp->GetRanduri() - 1) f << ",";
        }
        f << "] },\n";
    }
    f << "  ],\n  rezervari: [\n";
    for (auto r : c.GetRezervari()) {
        f << "    { id:" << r->GetID()
          << ", spectacolId:" << r->GetSpectacol()->GetID()
          << ", client:\"" << EscapeJS(r->GetNumeClient()) << "\""
          << ", tip:\"" << r->GetTipClient() << "\""
          << ", rand:" << r->GetRand()
          << ", col:" << r->GetColoana()
          << ", pret:" << r->GetPretFinal()
          << ", online:" << (r->EsteOnline() ? "true" : "false") << " },\n";
    }
    f << "  ]\n};\n";
    f.close();
}

void OptExportWeb(const Cinematograf& c) {
    Titlu("EXPORT WEB", CYAN);
    ExportWebData(c);
    Succes("web/data.js generat.");
    Info("Deschide web/index.html in browser ca sa vezi site-ul.");
}

// ============================================================
//  Meniu principal (adaptat la rol)
// ============================================================
void AfisareMeniu(const Cinematograf& c, const SistemAutentificare& auth) {
    cout << "\n";
    HRDouble(60);
    cout << "  " << BOLD << CYAN << c.GetNume() << RESET
        << GRAY << " - " << c.GetAdresa() << ", " << c.GetOras() << RESET << "\n";
    auto u = auth.GetUtilizatorCurent();
    if (u) cout << "  " << GRAY << "Logat: " << RESET << BOLD
                << u->GetNume() << " (" << u->GetRol() << ")" << RESET << "\n";
    HRDouble(60);
    cout << "  " << GREEN  << " 1." << RESET << " Vezi filmele\n";
    cout << "  " << GREEN  << " 2." << RESET << " Vezi salile\n";
    cout << "  " << GREEN  << " 3." << RESET << " Vezi spectacolele\n";
    cout << "  " << GREEN  << " 4." << RESET << " Harta locuri\n";
    cout << "  " << YELLOW << " 5." << RESET << " Rezerva bilet (casa)\n";
    cout << "  " << YELLOW << " 6." << RESET << " Rezerva ONLINE\n";
    cout << "  " << YELLOW << " 7." << RESET << " Rezerva " << BOLD << "MULTIPLE bilete consecutive" << RESET << "\n";
    cout << "  " << CYAN   << " 8." << RESET << " Vezi toate rezervarile\n";
    cout << "  " << CYAN   << " 9." << RESET << " Incasari (online/casa)\n";
    cout << "  " << MAGENTA<< "10." << RESET << " Demo exceptii\n";
    cout << "  " << MAGENTA<< "11." << RESET << " Export web (data.js)\n";
    cout << "  " << RED    << " 0." << RESET << " Iesire (cu salvare in date/)\n";
    HR(60, "-", GRAY);
}

// ============================================================
//  Main
// ============================================================
int main() {
    EnableAnsi();
    Banner();

    Cinematograf cinema("CinePlex Iasi", 35.0, "Str. Mihai Eminescu nr. 25", "Iasi");
    SistemAutentificare auth;

    cout << "  " << GRAY << "Initializare..." << RESET << "\n";

    fs::create_directories(DATA_FOLDER);
    bool hasData = fs::exists(DATA_FOLDER + "/filme.txt");
    if (hasData) {
        try {
            Persistenta::IncarcaTot(cinema, auth, DATA_FOLDER);
            // Daca fisierele sunt goale (eg. exit dupa register sau date corupte),
            // incarc demo ca sa nu ramana aplicatia goala
            if (cinema.GetFilme().Vid() || cinema.GetSpectacole().Vid()) {
                Avertizare("Date salvate incomplete - reincarc demo.");
                IncarcaDemoComplet(cinema, auth);
            } else {
                Succes("Date incarcate din " + DATA_FOLDER + "/ (" +
                    to_string(cinema.GetFilme().Marime()) + " filme, " +
                    to_string(cinema.GetSpectacole().Marime()) + " spectacole, " +
                    to_string(cinema.GetRezervari().Marime()) + " rezervari)");
            }
            // Daca sistemul de autentificare e gol, incarc impliciti
            if (auth.GetAngajati().Vid())
                auth.IncarcaImpliciti();
        } catch (...) {
            Avertizare("Probleme la incarcare. Incarc demo.");
            IncarcaDemoComplet(cinema, auth);
        }
    } else {
        Info("Prima rulare - incarc date demo extinse.");
        IncarcaDemoComplet(cinema, auth);
    }

    if (!LoginConsole(auth)) return 0;

    int optiune = -1;
    while (optiune != 0) {
        AfisareMeniu(cinema, auth);
        optiune = CitesteInt("Optiune", 0, 11);
        try {
            switch (optiune) {
                case 1:  AfisareFilmeColorat(cinema); break;
                case 2:  AfisareSaliColorat(cinema); break;
                case 3:  AfisareSpectacoleColorat(cinema); break;
                case 4:  OptHartaSala(cinema); break;
                case 5:  OptRezervare(cinema, false); break;
                case 6:  OptRezervare(cinema, true); break;
                case 7:  OptRezervareMultipla(cinema); break;
                case 8:  OptRezervari(cinema); break;
                case 9:  OptIncasari(cinema); break;
                case 10: OptDemoExceptii(cinema); break;
                case 11: OptExportWeb(cinema); break;
                case 0:
                    cout << "\n  " << GRAY << "Salvare date..." << RESET << "\n";
                    try {
                        Persistenta::SalveazaTot(cinema, auth, DATA_FOLDER);
                        ExportWebData(cinema);
                        Succes("Date salvate in " + DATA_FOLDER + "/ si web/data.js");
                    } catch (CinemaException& e) { Eroare(e.what()); }
                    cout << "\n  " << CYAN << BOLD
                        << "Va multumim ca ati ales " << cinema.GetNume() << "!" << RESET << "\n\n";
                    break;
                default: Eroare("Optiune invalida.");
            }
        } catch (CinemaException& e) { Eroare(e.what()); }
    }
    return 0;
}
