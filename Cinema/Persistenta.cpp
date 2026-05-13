#include "Persistenta.h"
#include "Cinematograf.h"
#include "SistemAutentificare.h"
#include "TipPersoana.h"
#include "TipAngajat.h"
#include "Exceptii.h"
#include <fstream>
#include <sstream>
#include <filesystem>
namespace fs = std::filesystem;

static string Trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    size_t b = s.find_last_not_of(" \t\r\n");
    if (a == string::npos) return "";
    return s.substr(a, b - a + 1);
}

// Split by '|' (delim) — handle trailing whitespace
static vector<string> Split(const string& s, char delim = '|') {
    vector<string> tokens;
    string buf;
    istringstream is(s);
    while (getline(is, buf, delim))
        tokens.push_back(Trim(buf));
    return tokens;
}

void Persistenta::AsiguraFolder(const string& folder) {
    try {
        if (!fs::exists(folder))
            fs::create_directories(folder);
    }
    catch (const exception& e) {
        throw FisierException(folder, e.what());
    }
}

void Persistenta::SalveazaTot(const Cinematograf& cinema,
    const SistemAutentificare& auth,
    const string& folder)
{
    AsiguraFolder(folder);
    SalveazaFilme(cinema, folder + "/filme.txt");
    SalveazaSali(cinema, folder + "/sali.txt");
    SalveazaSpectacole(cinema, folder + "/spectacole.txt");
    SalveazaRezervari(cinema, folder + "/rezervari.txt");
    SalveazaAngajati(auth, folder + "/angajati.txt");
}

void Persistenta::IncarcaTot(Cinematograf& cinema,
    SistemAutentificare& auth,
    const string& folder)
{
    // Tolerant la fisiere lipsa (prima rulare)
    try { IncarcaFilme(cinema, folder + "/filme.txt"); }      catch (...) {}
    try { IncarcaSali(cinema, folder + "/sali.txt"); }        catch (...) {}
    try { IncarcaSpectacole(cinema, folder + "/spectacole.txt"); } catch (...) {}
    try { IncarcaRezervari(cinema, folder + "/rezervari.txt"); }    catch (...) {}
    try { IncarcaAngajati(auth, folder + "/angajati.txt"); }   catch (...) {}
}

// ============================== FILME ==============================
void Persistenta::SalveazaFilme(const Cinematograf& cinema, const string& fisier) {
    ofstream f(fisier);
    if (!f.is_open()) throw FisierException(fisier, "nu se poate deschide pentru scriere");
    f << "# id|titlu|categorie|durata|varstaMin|limba|regizor|an|rating|poster\n";
    for (auto film : cinema.GetFilme()) {
        f << film->GetID() << "|"
            << film->GetTitlu() << "|"
            << film->GetCategorie() << "|"
            << film->GetDurata() << "|"
            << film->GetVarstaMinima() << "|"
            << film->GetLimba() << "|"
            << film->GetRegizor() << "|"
            << film->GetAn() << "|"
            << film->GetRating() << "|"
            << film->GetPoster() << "\n";
    }
}

void Persistenta::IncarcaFilme(Cinematograf& cinema, const string& fisier) {
    ifstream f(fisier);
    if (!f.is_open()) throw FisierException(fisier, "nu se poate deschide pentru citire");
    string linie;
    while (getline(f, linie)) {
        if (linie.empty() || linie[0] == '#') continue;
        auto t = Split(linie);
        if (t.size() < 10) continue;
        Film* film = new Film(t[1], t[2], stoi(t[3]), stoi(t[4]), t[5],
            t[6], stoi(t[7]), stod(t[8]), t[9]);
        film->SetID(stoi(t[0]));
        cinema.GetFilmeRef().Adauga(film);
    }
}

// ============================== SALI ==============================
void Persistenta::SalveazaSali(const Cinematograf& cinema, const string& fisier) {
    ofstream f(fisier);
    if (!f.is_open()) throw FisierException(fisier, "nu se poate deschide");
    f << "# tip|id|numar|capacitate|randuri|coloane|[VIP: supliment|tipScaun|meniu1;meniu2;...]\n";
    for (auto s : cinema.GetSali()) {
        if (s->EsteVIP()) {
            SalaVIP* sv = dynamic_cast<SalaVIP*>(s);
            f << "VIP|" << s->GetID() << "|" << s->GetNumarSala() << "|"
                << s->GetCapacitate() << "|" << s->GetRanduri() << "|" << s->GetColoane() << "|"
                << sv->GetSuplimentPret() << "|" << sv->GetTipScaun() << "|";
            const auto& m = sv->GetMeniu();
            for (size_t i = 0; i < m.size(); i++) {
                f << m[i];
                if (i + 1 < m.size()) f << ";";
            }
            f << "\n";
        }
        else {
            f << "NRM|" << s->GetID() << "|" << s->GetNumarSala() << "|"
                << s->GetCapacitate() << "|" << s->GetRanduri() << "|" << s->GetColoane()
                << "||||\n";
        }
    }
}

void Persistenta::IncarcaSali(Cinematograf& cinema, const string& fisier) {
    ifstream f(fisier);
    if (!f.is_open()) throw FisierException(fisier, "nu se poate deschide");
    string linie;
    while (getline(f, linie)) {
        if (linie.empty() || linie[0] == '#') continue;
        auto t = Split(linie);
        if (t.size() < 6) continue;
        if (t[0] == "VIP" && t.size() >= 9) {
            SalaVIP* sv = new SalaVIP(t[2], stoi(t[3]), stoi(t[4]), stoi(t[5]),
                stod(t[6]), t[7]);
            sv->SetID(stoi(t[1]));
            // produse meniu
            if (t.size() >= 9 && !t[8].empty()) {
                istringstream is(t[8]);
                string prod;
                while (getline(is, prod, ';'))
                    sv->AdaugaProdusMeniu(prod);
            }
            cinema.GetSaliRef().Adauga(sv);
        }
        else {
            Sala* s = new Sala(t[2], stoi(t[3]), stoi(t[4]), stoi(t[5]));
            s->SetID(stoi(t[1]));
            cinema.GetSaliRef().Adauga(s);
        }
    }
}

// ============================== SPECTACOLE ==============================
void Persistenta::SalveazaSpectacole(const Cinematograf& cinema, const string& fisier) {
    ofstream f(fisier);
    if (!f.is_open()) throw FisierException(fisier, "nu se poate deschide");
    f << "# id|filmID|salaID|data|ora|pretBaza\n";
    for (auto sp : cinema.GetSpectacole()) {
        f << sp->GetID() << "|"
            << (sp->GetFilm() ? sp->GetFilm()->GetID() : 0) << "|"
            << (sp->GetSala() ? sp->GetSala()->GetID() : 0) << "|"
            << sp->GetData() << "|"
            << sp->GetOra() << "|"
            << sp->GetPretBaza() << "\n";
    }
}

void Persistenta::IncarcaSpectacole(Cinematograf& cinema, const string& fisier) {
    ifstream f(fisier);
    if (!f.is_open()) throw FisierException(fisier, "nu se poate deschide");
    string linie;
    while (getline(f, linie)) {
        if (linie.empty() || linie[0] == '#') continue;
        auto t = Split(linie);
        if (t.size() < 6) continue;
        Film* film = cinema.GasesteFilmDupaID(stoi(t[1]));
        Sala* sala = cinema.GasesteSalaDupaID(stoi(t[2]));
        if (!film || !sala) continue;
        Spectacol* sp = new Spectacol(film, sala, t[3], t[4], stod(t[5]));
        sp->SetID(stoi(t[0]));
        cinema.GetSpectacoleRef().Adauga(sp);
    }
}

// ============================== REZERVARI ==============================
void Persistenta::SalveazaRezervari(const Cinematograf& cinema, const string& fisier) {
    ofstream f(fisier);
    if (!f.is_open()) throw FisierException(fisier, "nu se poate deschide");
    f << "# id|spectacolID|tipClient|numeClient|varsta|email|telefon|rand|coloana|dataEmiterii\n";
    for (auto r : cinema.GetRezervari()) {
        f << r->GetID() << "|"
            << (r->GetSpectacol() ? r->GetSpectacol()->GetID() : 0) << "|"
            << r->GetTipClient() << "|"
            << (r->GetClient() ? r->GetClient()->GetNume() : "") << "|"
            << (r->GetClient() ? r->GetClient()->GetVarsta() : 0) << "|"
            << (r->GetClient() ? r->GetClient()->GetEmail() : "") << "|"
            << (r->GetClient() ? r->GetClient()->GetTelefon() : "") << "|"
            << r->GetRand() << "|"
            << r->GetColoana() << "|"
            << r->GetDataEmiterii() << "\n";
    }
}

void Persistenta::IncarcaRezervari(Cinematograf& cinema, const string& fisier) {
    ifstream f(fisier);
    if (!f.is_open()) throw FisierException(fisier, "nu se poate deschide");
    string linie;
    while (getline(f, linie)) {
        if (linie.empty() || linie[0] == '#') continue;
        auto t = Split(linie);
        if (t.size() < 10) continue;

        Spectacol* sp = cinema.GasesteSpectacol(stoi(t[1]));
        if (!sp) continue;

        Persoana* client = nullptr;
        const string& tip = t[2];
        int varsta = stoi(t[4]);
        if (tip == "Adult")        client = new Adult(t[3], varsta, t[5], t[6]);
        else if (tip == "Student") client = new Student(t[3], varsta, t[5], t[6]);
        else if (tip == "Elev")    client = new Elev(t[3], varsta, t[5], t[6]);
        else if (tip == "Pensionar") client = new Pensionar(t[3], varsta, t[5], t[6]);
        else if (tip == "Copil")   client = new Copil(t[3], varsta, t[5], t[6]);
        else                        client = new Adult(t[3], varsta, t[5], t[6]);
        cinema.GetClientiRef().Adauga(client);

        int rand = stoi(t[7]);
        int col = stoi(t[8]);

        try {
            Rezervare* r = new Rezervare(client, sp, rand, col);
            r->SetID(stoi(t[0]));
            r->SetDataEmiterii(t[9]);
            sp->MarcheazaOcupat(rand, col, true);
            cinema.GetRezervariRef().Adauga(r);
        }
        catch (...) {
            // ignora rezervarile invalide
        }
    }
}

// ============================== ANGAJATI ==============================
void Persistenta::SalveazaAngajati(const SistemAutentificare& auth, const string& fisier) {
    ofstream f(fisier);
    if (!f.is_open()) throw FisierException(fisier, "nu se poate deschide");
    f << "# id|rol|nume|username|parola|salariu\n";
    for (auto a : auth.GetAngajati()) {
        f << a->GetID() << "|" << a->GetRol() << "|"
            << a->GetNume() << "|" << a->GetUsername() << "|"
            << a->GetParola() << "|" << a->GetSalariu() << "\n";
    }
}

void Persistenta::IncarcaAngajati(SistemAutentificare& auth, const string& fisier) {
    ifstream f(fisier);
    if (!f.is_open()) throw FisierException(fisier, "nu se poate deschide");
    // golim cei impliciti, daca au fost adaugati anterior
    string linie;
    bool primaLinie = true;
    while (getline(f, linie)) {
        if (linie.empty() || linie[0] == '#') continue;
        if (primaLinie) {
            // golim angajatii impliciti la prima linie valida
            auto& rep = auth.GetAngajatiRef();
            while (!rep.Vid()) rep.StergeIndex(0);
            primaLinie = false;
        }
        auto t = Split(linie);
        if (t.size() < 6) continue;

        Angajat* a = nullptr;
        const string& rol = t[1];
        if (rol == "Manager")        a = new Manager(t[2], t[3], t[4], stod(t[5]));
        else if (rol == "Casier")    a = new Casier(t[2], t[3], t[4], stod(t[5]));
        else if (rol == "Admin")     a = new Administrator(t[2], t[3], t[4], stod(t[5]));
        else                          a = new Casier(t[2], t[3], t[4], stod(t[5]));
        a->SetID(stoi(t[0]));
        auth.GetAngajatiRef().Adauga(a);
    }
}
