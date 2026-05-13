#pragma once
#include <string>
#include <iostream>
using namespace std;

// Persoana - clasa de baza abstracta pentru clientii cinematografului.
// Sub-clasele (Adult, Student, Elev) definesc tipul si discount-ul.
class Persoana
{
protected:
    static int GenerareID;
    int id;
    string nume;
    int varsta;
    string email;
    string telefon;

public:
    Persoana(string n, int v, string email = "", string telefon = "");
    virtual ~Persoana();

    int    GetID()      const { return id; }
    int    GetVarsta()  const { return varsta; }
    string GetNume()    const { return nume; }
    string GetEmail()   const { return email; }
    string GetTelefon() const { return telefon; }

    void SetNume(const string& n) { nume = n; }
    void SetVarsta(int v) { varsta = v; }
    void SetEmail(const string& e) { email = e; }
    void SetTelefon(const string& t) { telefon = t; }

    virtual float  GetDiscount() const = 0;
    virtual string GetTip()      const = 0;
    virtual void   Afisare()     const;

    friend ostream& operator<<(ostream& out, const Persoana& p);

    // Pentru persistenta
    void SetID(int newId) { id = newId; if (newId >= GenerareID) GenerareID = newId + 1; }
    static int GetNextID() { return GenerareID; }
    static void SetNextID(int v) { GenerareID = v; }
};
