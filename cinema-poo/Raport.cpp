#include "Raport.h"
#include "Cinematograf.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <map>

double Raport::TotalIncasari() const {
    double total = 0;
    for (auto r : cinema->GetRezervari())
        total += r->GetPretFinal();
    return total;
}

int Raport::TotalRezervari() const { return (int)cinema->GetRezervari().Marime(); }
int Raport::TotalFilme()     const { return (int)cinema->GetFilme().Marime(); }
int Raport::TotalSali()      const { return (int)cinema->GetSali().Marime(); }
int Raport::TotalSpectacole() const { return (int)cinema->GetSpectacole().Marime(); }

vector<StatisticaFilm> Raport::TopFilme(int n) const {
    map<int, StatisticaFilm> stats;
    for (auto f : cinema->GetFilme()) {
        stats[f->GetID()] = { f->GetTitlu(), 0, 0.0, 0.0 };
    }
    for (auto r : cinema->GetRezervari()) {
        Film* f = r->GetSpectacol() ? r->GetSpectacol()->GetFilm() : nullptr;
        if (!f) continue;
        auto it = stats.find(f->GetID());
        if (it != stats.end()) {
            it->second.numarRezervari++;
            it->second.incasariTotale += r->GetPretFinal();
        }
    }
    vector<StatisticaFilm> rezultat;
    for (auto& kv : stats) rezultat.push_back(kv.second);
    sort(rezultat.begin(), rezultat.end(),
        [](const StatisticaFilm& a, const StatisticaFilm& b) {
            return a.numarRezervari > b.numarRezervari;
        });
    if ((int)rezultat.size() > n) rezultat.resize(n);
    return rezultat;
}

vector<StatisticaSala> Raport::StatisticiSali() const {
    map<int, StatisticaSala> stats;
    for (auto s : cinema->GetSali()) {
        stats[s->GetID()] = { s->GetNumarSala(), 0, s->GetCapacitate(), 0.0, 0.0 };
    }
    for (auto r : cinema->GetRezervari()) {
        Sala* s = r->GetSpectacol() ? r->GetSpectacol()->GetSala() : nullptr;
        if (!s) continue;
        auto it = stats.find(s->GetID());
        if (it != stats.end()) {
            it->second.rezervari++;
            it->second.incasari += r->GetPretFinal();
        }
    }
    vector<StatisticaSala> rezultat;
    for (auto& kv : stats) {
        if (kv.second.locuriTotale > 0)
            kv.second.procentOcupare =
            100.0 * kv.second.rezervari / kv.second.locuriTotale;
        rezultat.push_back(kv.second);
    }
    sort(rezultat.begin(), rezultat.end(),
        [](const StatisticaSala& a, const StatisticaSala& b) {
            return a.incasari > b.incasari;
        });
    return rezultat;
}

vector<StatisticaCategorie> Raport::StatisticiCategorii() const {
    map<string, StatisticaCategorie> stats;
    for (auto f : cinema->GetFilme()) {
        auto& s = stats[f->GetCategorie()];
        s.categorie = f->GetCategorie();
        s.numarFilme++;
    }
    for (auto r : cinema->GetRezervari()) {
        Film* f = r->GetSpectacol() ? r->GetSpectacol()->GetFilm() : nullptr;
        if (!f) continue;
        auto& s = stats[f->GetCategorie()];
        s.numarRezervari++;
        s.incasari += r->GetPretFinal();
    }
    vector<StatisticaCategorie> rezultat;
    for (auto& kv : stats) rezultat.push_back(kv.second);
    sort(rezultat.begin(), rezultat.end(),
        [](const StatisticaCategorie& a, const StatisticaCategorie& b) {
            return a.incasari > b.incasari;
        });
    return rezultat;
}

vector<StatisticaZilnica> Raport::StatisticiZilnice() const {
    map<string, StatisticaZilnica> stats;
    for (auto r : cinema->GetRezervari()) {
        string d = r->GetData();
        if (d.empty()) continue;
        auto& s = stats[d];
        s.data = d;
        s.rezervari++;
        s.incasari += r->GetPretFinal();
    }
    vector<StatisticaZilnica> rezultat;
    for (auto& kv : stats) rezultat.push_back(kv.second);
    sort(rezultat.begin(), rezultat.end(),
        [](const StatisticaZilnica& a, const StatisticaZilnica& b) {
            return a.data < b.data;
        });
    return rezultat;
}

double Raport::PretMediuBilet() const {
    int n = TotalRezervari();
    if (n == 0) return 0;
    return TotalIncasari() / n;
}

map<string, int> Raport::RezervariPerTip() const {
    map<string, int> stats;
    for (auto r : cinema->GetRezervari()) {
        if (r->GetClient())
            stats[r->GetClient()->GetTip()]++;
    }
    return stats;
}

string Raport::RaportComplet() const {
    ostringstream os;
    os << fixed << setprecision(2);
    os << "===========================================\n";
    os << "  RAPORT COMPLET - " << cinema->GetNume() << "\n";
    os << "===========================================\n\n";
    os << "Sumar general:\n";
    os << "  Numar filme:       " << TotalFilme() << "\n";
    os << "  Numar sali:        " << TotalSali() << "\n";
    os << "  Numar spectacole:  " << TotalSpectacole() << "\n";
    os << "  Numar rezervari:   " << TotalRezervari() << "\n";
    os << "  Total incasari:    " << TotalIncasari() << " RON\n";
    os << "  Pret mediu bilet:  " << PretMediuBilet() << " RON\n\n";

    os << "Top filme:\n";
    auto topF = TopFilme(5);
    for (auto& s : topF) {
        if (s.numarRezervari == 0) continue;
        os << "  " << s.titlu << ": " << s.numarRezervari
            << " bilete, " << s.incasariTotale << " RON\n";
    }
    os << "\n";

    os << "Statistici sali:\n";
    for (auto& s : StatisticiSali()) {
        os << "  Sala " << s.numarSala
            << ": " << s.rezervari << " bilete, "
            << s.incasari << " RON\n";
    }
    os << "\n";

    os << "Statistici categorii:\n";
    for (auto& c : StatisticiCategorii()) {
        os << "  " << c.categorie << ": "
            << c.numarFilme << " filme, "
            << c.numarRezervari << " bilete, "
            << c.incasari << " RON\n";
    }
    os << "\n";

    os << "Rezervari pe tip persoana:\n";
    for (auto& kv : RezervariPerTip()) {
        os << "  " << kv.first << ": " << kv.second << "\n";
    }
    return os.str();
}
