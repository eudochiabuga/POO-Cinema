#pragma once
#include "Film.h"
#include "Sala.h"
#include "Persoana.h"
#include "Spectacol.h"
#include <ctime>
#include <iomanip>

// Rezervare = bilet emis pentru un client (Persoana) la un spectacol,
// pentru un anumit loc (rand, coloana). Calculeaza pretul final aplicand
// discount-uri (varsta, tip persoana), supliment VIP, reduceri zile speciale.
class Rezervare
{
    static int GenerareID;
    int idRezervare;
    Persoana* client;        // nu detine memoria
    Spectacol* spectacol;    // nu detine memoria
    int rand, coloana;
    double pretBaza;
    double pretFinal;
    string dataEmiterii;     // "YYYY-MM-DD HH:MM"

public:
    Rezervare(Persoana* c, Spectacol* sp, int r, int col);
    Rezervare(const Rezervare& r);
    Rezervare& operator=(const Rezervare& r);
    virtual ~Rezervare() = default;

    // Polimorfism: clasa de baza Rezervare returneaza false, RezervareOnline true.
    virtual bool EsteOnline() const { return false; }

    double CalculeazaPret() const;
    bool EsteVineri() const;
    bool EsteWeekend() const;

    void Afisare() const;
    string ToString() const;

    int    GetID()         const { return idRezervare; }
    double GetPretFinal()  const { return pretFinal; }
    double GetPretBaza()   const { return pretBaza; }
    string GetNumeClient() const { return client ? client->GetNume() : ""; }
    string GetTipClient()  const { return client ? client->GetTip() : ""; }
    string GetTitluFilm()  const;
    string GetNumarSala()  const;
    string GetData()       const;
    string GetOra()        const;
    int    GetRand()       const { return rand; }
    int    GetColoana()    const { return coloana; }
    Persoana* GetClient()  const { return client; }
    Spectacol* GetSpectacol() const { return spectacol; }
    string GetDataEmiterii() const { return dataEmiterii; }

    friend ostream& operator<<(ostream& out, const Rezervare& r);

    // Pentru persistenta
    void SetID(int newId) { idRezervare = newId; if (newId >= GenerareID) GenerareID = newId + 1; }
    static int GetNextID() { return GenerareID; }
    static void SetNextID(int v) { GenerareID = v; }
    void SetDataEmiterii(const string& d) { dataEmiterii = d; }
};
