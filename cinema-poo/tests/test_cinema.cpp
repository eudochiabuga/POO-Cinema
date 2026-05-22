// ============================================================
//  test_cinema.cpp - Teste unitare pentru sistemul de cinema.
//
//  Framework de testare scris in casa - fara dependinte externe.
//  Foloseste macro-uri ASSERT_* pentru asertii cu mesaje frumoase.
//
//  Compilare (dintr-un parinte care contine tests/ si fisierele .cpp):
//      make test
//  sau direct:
//      g++ -std=c++17 -I.. tests/test_cinema.cpp Film.cpp Sala.cpp \
//          SalaVIP.cpp Spectacol.cpp Rezervare.cpp RezervareOnline.cpp \
//          Persoana.cpp TipPersoana.cpp Angajat.cpp TipAngajat.cpp \
//          SistemAutentificare.cpp Cinematograf.cpp Persistenta.cpp \
//          Raport.cpp AppContext.cpp -o test_cinema && ./test_cinema
// ============================================================

#include "../Cinematograf.h"
#include "../RezervareOnline.h"
#include "../SalaVIP.h"
#include "../TipPersoana.h"
#include "../Exceptii.h"
#include "../Repository.h"

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <stdexcept>

using namespace std;

// ============================================================
//  Mini test framework
// ============================================================
namespace TestFramework {
    static int testeTrecute = 0;
    static int testeEsuate = 0;
    static int totalAssertii = 0;
    static std::vector<std::string> erori;
    static std::string testCurent;

    // Coduri ANSI
    constexpr const char* RESET = "\033[0m";
    constexpr const char* GREEN = "\033[32m";
    constexpr const char* RED = "\033[31m";
    constexpr const char* YELLOW = "\033[33m";
    constexpr const char* CYAN = "\033[36m";
    constexpr const char* BOLD = "\033[1m";

    void inceputTest(const std::string& nume) {
        testCurent = nume;
        std::cout << "  " << CYAN << "[ TEST ]" << RESET << " " << nume << " ... ";
        std::cout.flush();
    }

    void sfarsitTest(bool succes) {
        if (succes) {
            std::cout << GREEN << BOLD << "OK" << RESET << "\n";
            testeTrecute++;
        }
        else {
            std::cout << RED << BOLD << "FAIL" << RESET << "\n";
            testeEsuate++;
        }
    }

    void sumar() {
        int total = testeTrecute + testeEsuate;
        std::cout << "\n";
        std::cout << "═══════════════════════════════════════════════════════════\n";
        std::cout << "  REZULTATE:  " << BOLD << total << RESET << " teste totale  |  "
            << GREEN << testeTrecute << " trecute" << RESET << "  |  "
            << (testeEsuate ? RED : GREEN) << testeEsuate << " esuate" << RESET
            << "  |  " << totalAssertii << " assertii\n";
        std::cout << "═══════════════════════════════════════════════════════════\n";

        if (!erori.empty()) {
            std::cout << RED << BOLD << "\nDETALII ERORI:\n" << RESET;
            for (const auto& e : erori) std::cout << "  - " << e << "\n";
        }

        if (testeEsuate == 0)
            std::cout << GREEN << BOLD << "\n  Toate testele au trecut!\n\n" << RESET;
        else
            std::cout << RED << BOLD << "\n  Atentie: exista teste esuate.\n\n" << RESET;
    }
}

// ---- Macro-uri ----
#define TEST(nume)                                            \
    do {                                                      \
        TestFramework::inceputTest(nume);                     \
        bool _testOk = true;                                  \
        try {

#define END_TEST                                              \
        } catch (const std::exception& e) {                   \
            _testOk = false;                                  \
            TestFramework::erori.push_back(                   \
                TestFramework::testCurent + ": exceptie neasteptata: " + e.what()); \
        } catch (...) {                                       \
            _testOk = false;                                  \
            TestFramework::erori.push_back(                   \
                TestFramework::testCurent + ": exceptie neasteptata necunoscuta"); \
        }                                                     \
        TestFramework::sfarsitTest(_testOk);                  \
    } while(0)

#define ASSERT_TRUE(cond)                                     \
    do {                                                      \
        TestFramework::totalAssertii++;                       \
        if (!(cond)) {                                        \
            _testOk = false;                                  \
            TestFramework::erori.push_back(                   \
                TestFramework::testCurent + ": ASSERT_TRUE(" #cond ") esuat la linia " \
                + std::to_string(__LINE__));                  \
        }                                                     \
    } while(0)

#define ASSERT_FALSE(cond) ASSERT_TRUE(!(cond))

#define ASSERT_EQ(a, b)                                       \
    do {                                                      \
        TestFramework::totalAssertii++;                       \
        if (!((a) == (b))) {                                  \
            _testOk = false;                                  \
            TestFramework::erori.push_back(                   \
                TestFramework::testCurent + ": ASSERT_EQ esuat la linia " \
                + std::to_string(__LINE__));                  \
        }                                                     \
    } while(0)

#define ASSERT_NEAR(a, b, eps)                                \
    do {                                                      \
        TestFramework::totalAssertii++;                       \
        if (std::fabs((a) - (b)) > (eps)) {                   \
            _testOk = false;                                  \
            TestFramework::erori.push_back(                   \
                TestFramework::testCurent + ": ASSERT_NEAR esuat la linia " \
                + std::to_string(__LINE__)                    \
                + " (got " + std::to_string(a)                \
                + ", expected " + std::to_string(b) + ")");   \
        }                                                     \
    } while(0)

#define ASSERT_THROWS(expr, ExceptionType)                    \
    do {                                                      \
        TestFramework::totalAssertii++;                       \
        bool _aruncat = false;                                \
        try { expr; }                                         \
        catch (const ExceptionType&) { _aruncat = true; }     \
        catch (...) {}                                        \
        if (!_aruncat) {                                      \
            _testOk = false;                                  \
            TestFramework::erori.push_back(                   \
                TestFramework::testCurent + ": ASSERT_THROWS(" #ExceptionType ") nu a aruncat la linia " \
                + std::to_string(__LINE__));                  \
        }                                                     \
    } while(0)

// ============================================================
//  Helper - cinema cu date demo (fara afisare)
// ============================================================
static void IncarcaDateTest(Cinematograf& c) {
    c.AdaugaSala(new Sala("A1", 24, 4, 6));
    c.AdaugaSala(new SalaVIP("VIP", 12, 3, 4, 0.50, "Recliner"));
    c.AdaugaFilm(new Film("Test2D", "Drama", 100, 0, "RO", "X", 2024, 8.0, "", "2D"));
    c.AdaugaFilm(new Film("Test3D", "SciFi", 120, 13, "EN", "Y", 2024, 7.5, "", "3D"));

    auto& filme = c.GetFilmeRef();
    auto& sali = c.GetSaliRef();
    // 2026-05-22 = Vineri (verificat cu Zeller)
    c.CreeazaSpectacol(filme[0], sali[0], "2026-05-22", "18:00", 30.0);
    // 2026-05-26 = Marti (zi obisnuita)
    c.CreeazaSpectacol(filme[1], sali[1], "2026-05-26", "20:00", 40.0);
    // 2026-05-23 = Sambata (weekend)
    c.CreeazaSpectacol(filme[0], sali[0], "2026-05-23", "19:00", 30.0);
}

// ============================================================
//  TESTE
// ============================================================

void test_Film_Creare() {
    TEST("Film: creare cu valori implicite si getter-i");
    Film f("Inception", "SciFi", 148, 13, "EN", "Nolan", 2010, 8.8, "", "2D");
    ASSERT_EQ(f.GetTitlu(), std::string("Inception"));
    ASSERT_EQ(f.GetDurata(), 148);
    ASSERT_EQ(f.GetVarstaMinima(), 13);
    ASSERT_FALSE(f.Este3D());

    Film f2("Avatar", "SciFi", 120, 12, "EN", "Cameron", 2022, 7.6, "", "3D");
    ASSERT_TRUE(f2.Este3D());
    END_TEST;
}

void test_Sala_AlocareSiMatrice() {
    TEST("Sala: alocare matrice + ocupare loc + verificare disponibilitate");
    Sala s("X", 12, 3, 4);
    ASSERT_EQ(s.GetRanduri(), 3);
    ASSERT_EQ(s.GetColoane(), 4);
    ASSERT_TRUE(s.VerificaDisponibilitate(0, 0));
    ASSERT_TRUE(s.OcupaLoc(0, 0));
    ASSERT_FALSE(s.VerificaDisponibilitate(0, 0));
    ASSERT_FALSE(s.OcupaLoc(0, 0));
    END_TEST;
}

void test_SalaVIP_Polimorfism() {
    TEST("SalaVIP: mostenire si polimorfism");
    SalaVIP sv("V1", 12, 3, 4, 0.5, "Recliner");
    Sala* s = &sv;
    ASSERT_TRUE(s->EsteVIP());                       // virtual override
    ASSERT_NEAR(s->GetSuplimentPret(), 0.5, 0.001);
    END_TEST;
}

void test_Repository_Template() {
    TEST("Repository<T>: template, adauga, gaseste, sterge, filtreaza");
    Repository<Film> rep(true);
    rep.Adauga(new Film("A", "Drama", 100, 0));
    rep.Adauga(new Film("B", "Comedie", 90, 0));
    rep.Adauga(new Film("C", "Drama", 110, 0));
    ASSERT_EQ(rep.Marime(), (size_t)3);

    Film* gasit = rep.GasestePrimul([](const Film* f) { return f->GetTitlu() == "B"; });
    ASSERT_TRUE(gasit != nullptr);
    ASSERT_EQ(gasit->GetTitlu(), std::string("B"));

    auto drame = rep.Filtreaza([](const Film* f) { return f->GetCategorie() == "Drama"; });
    ASSERT_EQ(drame.size(), (size_t)2);
    END_TEST;
}

void test_Rezervare_Valida() {
    TEST("Rezervare: creare valida + loc devine ocupat");
    Cinematograf c("Test", 30, "x", "y");
    IncarcaDateTest(c);
    auto sp = c.GetSpectacole()[0];
    Adult* a = new Adult("Ion", 30);
    c.AdaugaClient(a);

    Rezervare* r = c.FaRezervare(a, sp, 0, 0);
    ASSERT_TRUE(r != nullptr);
    ASSERT_FALSE(sp->LocLiber(0, 0));
    ASSERT_EQ(r->GetClient(), (Persoana*)a);
    END_TEST;
}

void test_Rezervare_LocOcupatException() {
    TEST("Rezervare: acelasi loc → LocOcupatException");
    Cinematograf c("Test", 30, "x", "y");
    IncarcaDateTest(c);
    auto sp = c.GetSpectacole()[0];
    Adult* a = new Adult("A", 30);
    c.AdaugaClient(a);
    c.FaRezervare(a, sp, 1, 1);
    ASSERT_THROWS(c.FaRezervare(a, sp, 1, 1), LocOcupatException);
    END_TEST;
}

void test_Rezervare_LocInvalidException() {
    TEST("Rezervare: loc in afara salii → LocInvalidException");
    Cinematograf c("Test", 30, "x", "y");
    IncarcaDateTest(c);
    auto sp = c.GetSpectacole()[0];
    Adult* a = new Adult("A", 30);
    c.AdaugaClient(a);
    ASSERT_THROWS(c.FaRezervare(a, sp, 100, 100), LocInvalidException);
    END_TEST;
}

void test_Rezervare_VarstaInsuficientaException() {
    TEST("Rezervare: minor la film 13+ → VarstaInsuficientaException");
    Cinematograf c("Test", 30, "x", "y");
    IncarcaDateTest(c);
    auto sp3D = c.GetSpectacole()[1];   // Test3D, 13+
    Copil* copil = new Copil("Copil", 10);
    c.AdaugaClient(copil);
    ASSERT_THROWS(c.FaRezervare(copil, sp3D, 0, 0), VarstaInsuficientaException);
    // Si locul trebuie sa ramana liber
    ASSERT_TRUE(sp3D->LocLiber(0, 0));
    END_TEST;
}

void test_Pret_2D_vs_3D() {
    TEST("Calcul pret: 3D = 1.20 x 2D (la aceeasi sala normala, zi obisnuita)");
    Cinematograf c("Test", 30, "x", "y");
    c.AdaugaSala(new Sala("A1", 24, 4, 6));
    c.AdaugaFilm(new Film("F2D", "Drama", 100, 0, "RO", "X", 2024, 8.0, "", "2D"));
    c.AdaugaFilm(new Film("F3D", "SciFi", 100, 0, "RO", "X", 2024, 8.0, "", "3D"));
    auto& filme = c.GetFilmeRef();
    auto& sali = c.GetSaliRef();
    // 2026-05-26 = Marti (nu weekend, nu vineri)
    auto sp2d = c.CreeazaSpectacol(filme[0], sali[0], "2026-05-26", "18:00", 30.0);
    auto sp3d = c.CreeazaSpectacol(filme[1], sali[0], "2026-05-26", "20:00", 30.0);

    Adult* a1 = new Adult("a1", 30); c.AdaugaClient(a1);
    Adult* a2 = new Adult("a2", 30); c.AdaugaClient(a2);

    auto r2d = c.FaRezervare(a1, sp2d, 0, 0);   // 30.0
    auto r3d = c.FaRezervare(a2, sp3d, 0, 0);   // 30.0 * 1.20 = 36.0
    ASSERT_NEAR(r2d->GetPretFinal(), 30.0, 0.01);
    ASSERT_NEAR(r3d->GetPretFinal(), 36.0, 0.01);
    END_TEST;
}

void test_Pret_DiscountStudent() {
    TEST("Calcul pret: Student primeste 20% discount");
    Cinematograf c("Test", 30, "x", "y");
    c.AdaugaSala(new Sala("A1", 24, 4, 6));
    c.AdaugaFilm(new Film("F", "Drama", 100, 0, "RO", "X", 2024, 8.0, "", "2D"));
    auto& filme = c.GetFilmeRef();
    auto& sali = c.GetSaliRef();
    auto sp = c.CreeazaSpectacol(filme[0], sali[0], "2026-05-26", "18:00", 30.0);

    Student* s = new Student("Student", 22);
    c.AdaugaClient(s);
    auto r = c.FaRezervare(s, sp, 0, 0);
    ASSERT_NEAR(r->GetPretFinal(), 30.0 * 0.80, 0.01);
    END_TEST;
}

void test_Pret_Vineri_50pct() {
    TEST("Calcul pret: Vineri (2026-05-22) → reducere 50%");
    Cinematograf c("Test", 30, "x", "y");
    c.AdaugaSala(new Sala("A1", 24, 4, 6));
    c.AdaugaFilm(new Film("F", "Drama", 100, 0, "RO", "X", 2024, 8.0, "", "2D"));
    auto& filme = c.GetFilmeRef();
    auto& sali = c.GetSaliRef();
    auto sp = c.CreeazaSpectacol(filme[0], sali[0], "2026-05-22", "18:00", 40.0);
    Adult* a = new Adult("a", 30); c.AdaugaClient(a);
    auto r = c.FaRezervare(a, sp, 0, 0);
    ASSERT_NEAR(r->GetPretFinal(), 40.0 * 0.5, 0.01);
    END_TEST;
}

void test_Pret_Weekend_Supliment() {
    TEST("Calcul pret: Sambata (2026-05-23) → supliment +10%");
    Cinematograf c("Test", 30, "x", "y");
    c.AdaugaSala(new Sala("A1", 24, 4, 6));
    c.AdaugaFilm(new Film("F", "Drama", 100, 0, "RO", "X", 2024, 8.0, "", "2D"));
    auto& filme = c.GetFilmeRef();
    auto& sali = c.GetSaliRef();
    auto sp = c.CreeazaSpectacol(filme[0], sali[0], "2026-05-23", "18:00", 40.0);
    Adult* a = new Adult("a", 30); c.AdaugaClient(a);
    auto r = c.FaRezervare(a, sp, 0, 0);
    ASSERT_NEAR(r->GetPretFinal(), 40.0 * 1.10, 0.01);
    END_TEST;
}

void test_Pret_SalaVIP_Supliment() {
    TEST("Calcul pret: Sala VIP (supliment +50%)");
    Cinematograf c("Test", 30, "x", "y");
    c.AdaugaSala(new SalaVIP("V1", 12, 3, 4, 0.50, "Recliner"));
    c.AdaugaFilm(new Film("F", "Drama", 100, 0, "RO", "X", 2024, 8.0, "", "2D"));
    auto& filme = c.GetFilmeRef();
    auto& sali = c.GetSaliRef();
    // 2026-05-26 = marti (zi normala)
    auto sp = c.CreeazaSpectacol(filme[0], sali[0], "2026-05-26", "18:00", 40.0);
    Adult* a = new Adult("a", 30); c.AdaugaClient(a);
    auto r = c.FaRezervare(a, sp, 0, 0);
    ASSERT_NEAR(r->GetPretFinal(), 40.0 * 1.50, 0.01);
    END_TEST;
}

void test_RezervareOnline_Cod() {
    TEST("RezervareOnline: are email si cod confirmare unic");
    Cinematograf c("Test", 30, "x", "y");
    IncarcaDateTest(c);
    auto sp = c.GetSpectacole()[0];
    Adult* a = new Adult("Test", 30); c.AdaugaClient(a);

    sp->RezervaLoc(2, 2);
    RezervareOnline* ro = new RezervareOnline(a, sp, 2, 2, "test@example.com");
    c.GetRezervariRef().Adauga(ro);
    ASSERT_EQ(ro->GetEmailClient(), std::string("test@example.com"));
    ASSERT_TRUE(ro->GetCodConfirmare().find("ONL-") == 0);
    END_TEST;
}

void test_Cinematograf_Cautari() {
    TEST("Cinematograf: cautari dupa ID si dupa nume");
    Cinematograf c("Test", 30, "x", "y");
    IncarcaDateTest(c);
    auto f = c.GasesteFilm("Test2D");
    ASSERT_TRUE(f != nullptr);
    auto fId = c.GasesteFilmDupaID(f->GetID());
    ASSERT_EQ(f, fId);

    auto s = c.GasesteSala("A1");
    ASSERT_TRUE(s != nullptr);
    END_TEST;
}

void test_ICinemaService_Polimorfism() {
    TEST("ICinemaService: Cinematograf folosit prin pointer la interfata");
    Cinematograf c("Test", 30, "x", "y");
    IncarcaDateTest(c);
    ICinemaService* svc = &c;
    Film* f = svc->GasesteFilmDupaID(c.GetFilmeRef()[0]->GetID());
    ASSERT_TRUE(f != nullptr);

    Sala* s = svc->GasesteSalaDupaID(c.GetSaliRef()[0]->GetID());
    ASSERT_TRUE(s != nullptr);
    END_TEST;
}

void test_Spectacol_Statistici() {
    TEST("Spectacol: statistici - locuri ocupate, procent ocupare");
    Cinematograf c("Test", 30, "x", "y");
    c.AdaugaSala(new Sala("A1", 12, 3, 4));    // 12 locuri
    c.AdaugaFilm(new Film("F", "Drama", 100, 0, "RO", "X", 2024, 8.0, "", "2D"));
    auto& filme = c.GetFilmeRef();
    auto& sali = c.GetSaliRef();
    auto sp = c.CreeazaSpectacol(filme[0], sali[0], "2026-05-26", "18:00", 30.0);

    ASSERT_EQ(sp->LocuriOcupate(), 0);
    ASSERT_EQ(sp->LocuriDisponibile(), 12);

    Adult* a = new Adult("a", 30); c.AdaugaClient(a);
    c.FaRezervare(a, sp, 0, 0);
    ASSERT_EQ(sp->LocuriOcupate(), 1);
    ASSERT_EQ(sp->LocuriDisponibile(), 11);
    ASSERT_NEAR(sp->ProcentOcupare(), 100.0 / 12.0, 0.1);
    END_TEST;
}

// ============================================================
//  Runner
// ============================================================
int main() {
    using namespace std;
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║         TESTE UNITARE - SISTEM CINEMA (POO)               ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n\n";

    test_Film_Creare();
    test_Sala_AlocareSiMatrice();
    test_SalaVIP_Polimorfism();
    test_Repository_Template();
    test_Rezervare_Valida();
    test_Rezervare_LocOcupatException();
    test_Rezervare_LocInvalidException();
    test_Rezervare_VarstaInsuficientaException();
    test_Pret_2D_vs_3D();
    test_Pret_DiscountStudent();
    test_Pret_Vineri_50pct();
    test_Pret_Weekend_Supliment();
    test_Pret_SalaVIP_Supliment();
    test_RezervareOnline_Cod();
    test_Cinematograf_Cautari();
    test_ICinemaService_Polimorfism();
    test_Spectacol_Statistici();

    TestFramework::sumar();
    return TestFramework::testeEsuate == 0 ? 0 : 1;
}
