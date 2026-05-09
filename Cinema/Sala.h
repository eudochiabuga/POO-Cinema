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
	Sala(string sala="0", int capacitate=0, int numarRanduri=0, int numarColoane=0);
	~Sala();
	void Afisare() const;
	Sala(const Sala& sala);
	Sala& operator=(const Sala& sala);
	void OcupaLoc(int rand, int coloana);
	bool VerificaDisponibiltate(int rand, int coloana);
	string GetNumarSala() const
	{
		return NumarSala;
	}
	int GetRanduri() const
	{
		return NumarRanduri;
	}
	int GetColoane() const
	{
		return NumarColoane;
	}
	friend ostream& operator<<(ostream& out, const Sala& sala);


};
