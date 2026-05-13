// MainApp.cpp - punct de intrare pentru aplicatia Windows Forms.
// Necesita compilare cu /clr si SubSystem:Windows (vezi .vcxproj).

#include "AppContext.h"
#include "Persistenta.h"
#include "TipPersoana.h"
#include "TipAngajat.h"
#include "SistemAutentificare.h"
#include "Exceptii.h"
#include "LoginForm.h"
#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

// Populeaza cu date demo daca nu exista fisierele
void IncarcaDateDemo() {
    Cinematograf* c = CinemaContext::cinema;

    // Daca exista deja date incarcate din fisier, nu suprascriem
    if (!c->GetFilme().Vid() || !c->GetSali().Vid()) return;

    // Sali
    c->AdaugaSala(new Sala("1", 48, 6, 8));
    c->AdaugaSala(new Sala("2", 30, 5, 6));
    c->AdaugaSala(new Sala("3", 60, 6, 10));
    SalaVIP* vip = new SalaVIP("VIP", 20, 4, 5, 0.5, "Recliner");
    vip->AdaugaProdusMeniu("Popcorn premium");
    vip->AdaugaProdusMeniu("Cocktail fara alcool");
    vip->AdaugaProdusMeniu("Platou de branza");
    vip->AdaugaProdusMeniu("Caramele de ciocolata");
    c->AdaugaSala(vip);

    // Filme
    c->AdaugaFilm(new Film("Inception",      "Sci-Fi",   148, 13, "Engleza",
        "Christopher Nolan", 2010, 8.8));
    c->AdaugaFilm(new Film("Minions",        "Animatie",  90,  0, "Romana",
        "Kyle Balda",        2015, 6.4));
    c->AdaugaFilm(new Film("John Wick 4",    "Actiune",  169, 16, "Engleza",
        "Chad Stahelski",    2023, 7.7));
    c->AdaugaFilm(new Film("Dune Part Two",  "Sci-Fi",   166, 12, "Engleza",
        "Denis Villeneuve",  2024, 8.6));
    c->AdaugaFilm(new Film("Oppenheimer",    "Biografie",180, 16, "Engleza",
        "Christopher Nolan", 2023, 8.4));
    c->AdaugaFilm(new Film("Barbie",         "Comedie",  114, 12, "Engleza",
        "Greta Gerwig",      2023, 7.0));
    c->AdaugaFilm(new Film("Avatar 2",       "Sci-Fi",   192, 12, "Engleza",
        "James Cameron",     2022, 7.6));

    // Spectacole demo
    auto filme = c->GetFilme().GetToate();
    auto sali = c->GetSali().GetToate();
    if (filme.size() >= 4 && sali.size() >= 3) {
        c->CreeazaSpectacol(filme[0], sali[0], "2026-05-15", "18:00", 35);
        c->CreeazaSpectacol(filme[0], sali[1], "2026-05-15", "21:00", 35);
        c->CreeazaSpectacol(filme[1], sali[1], "2026-05-15", "11:00", 25);
        c->CreeazaSpectacol(filme[2], sali[0], "2026-05-16", "19:30", 38);
        c->CreeazaSpectacol(filme[3], sali[3], "2026-05-16", "20:00", 55);
    }
}

[STAThreadAttribute]
int main(cli::array<System::String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Construim obiectele globale
    CinemaContext::cinema = new Cinematograf("CINEMA", 35.0, "Strada Mihai Eminescu nr. 25", "Iasi");
    CinemaContext::auth = new SistemAutentificare();
    CinemaContext::auth->IncarcaImpliciti();

    // Incarcam date persistate (daca exista)
    try {
        Persistenta::IncarcaTot(*CinemaContext::cinema, *CinemaContext::auth,
            CinemaContext::folderDate);
    }
    catch (...) { /* prima rulare */ }

    // Populeaza cu date demo daca e gol
    IncarcaDateDemo();

    // Login
    CinemaApp::LoginForm^ login = gcnew CinemaApp::LoginForm();
    if (login->ShowDialog() != System::Windows::Forms::DialogResult::OK) {
        delete CinemaContext::cinema;
        delete CinemaContext::auth;
        return 0;
    }

    // MainForm
    Application::Run(gcnew CinemaApp::MainForm());

    // Salvare automata la iesire
    try {
        Persistenta::SalveazaTot(*CinemaContext::cinema, *CinemaContext::auth,
            CinemaContext::folderDate);
    }
    catch (...) {}

    delete CinemaContext::cinema;
    delete CinemaContext::auth;
    return 0;
}
