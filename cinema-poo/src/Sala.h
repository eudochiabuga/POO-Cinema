#pragma once
#include <string>
#include <vector>
#include <iostream>
using namespace std;

// Sala = sala de cinema (informatii fizice + matricea pentru rezervari
// "directe", de exemplu cand nu se foloseste fluxul de Spectacol).
// In fluxul nou cu Spectacole, fiecare Spectacol are propria sa matrice;
// matricea din Sala este pastrata pentru compatibilitate si pentru
// scenariul "cinema fara programari".
class Sala
{
protected:
    static int GenerareID;
    int id;

    string NumarSala;
    int Capacitate;
    int NumarRanduri;
    int NumarColoane;
    int** Matrice;
    void AlocaMatrice();
    void ElibereazaMatrice();

public:
    Sala(string sala = "0", int capacitate = 0, int numarRanduri = 0, int numarColoane = 0);
    Sala(const Sala& s);
    Sala& operator=(const Sala& s);
    virtual ~Sala();

    bool OcupaLoc(int rand, int coloana);
    bool VerificaDisponibilitate(int rand, int coloana) const;
    bool EsteLocVIP(int rand) const;
    void ReseteazaLocuri();   // toate locurile devin libere

    virtual void Afisare() const;
    virtual void AfisareHarta() const;
    virtual string Descriere() const;

    int    GetID()         const { return id; }
    string GetNumarSala()  const;
    int    GetCapacitate() const { return Capacitate; }
    int    GetRanduri()    const;
    int    GetColoane()    const;
    virtual double GetSuplimentPret() const;
    virtual bool EsteVIP() const;

    void SetNumarSala(const string& n) { NumarSala = n; }

    friend ostream& operator<<(ostream& out, const Sala& s);

    // Pentru persistenta
    void SetID(int newId) { id = newId; if (newId >= GenerareID) GenerareID = newId + 1; }
    static int GetNextID() { return GenerareID; }
    static void SetNextID(int v) { GenerareID = v; }
};
