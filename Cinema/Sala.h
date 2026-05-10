#pragma once
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Sala
{
protected:
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

    virtual void Afisare() const;
    virtual void AfisareHarta() const;

    string GetNumarSala() const;
    int GetRanduri() const;
    int GetColoane() const;
    virtual double GetSuplimentPret() const;
    virtual bool EsteVIP() const;

    friend ostream& operator<<(ostream& out, const Sala& s);
};


	
