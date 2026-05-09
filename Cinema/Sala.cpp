#include "Sala.h"

Sala::Sala(string sala, int capacitate, int numarRanduri, int numarColoane)
{
	NumarSala = sala;
	Capacitate = capacitate;
	NumarRanduri = numarRanduri;
	NumarColoane = numarColoane;
	AlocaMatrice();
	for (int i = 0; i < NumarRanduri; i++)
		for (int j = 0; j < NumarColoane; j++)
			Matrice[i][j] = 0;
		
}

Sala::~Sala()
{
	ElibereazaMatrice();
}

void Sala::Afisare() const
{
	cout << "Numar sala: " << NumarSala << endl;
	cout << "Capacitate: " << Capacitate << endl;
	cout << "Numar randuri: " << NumarRanduri << endl;
	cout << "Numar coloane: " << NumarColoane << endl;
}

Sala::Sala(const Sala& sala)
{
	NumarSala = sala.NumarSala;
	Capacitate = sala.Capacitate;
	NumarRanduri = sala.NumarRanduri;
	NumarColoane = sala.NumarColoane;
	AlocaMatrice();
	for (int i = 0; i < NumarRanduri; i++)
		for (int j = 0; j < NumarColoane; j++)
			Matrice[i][j] = sala.Matrice[i][j];


}
Sala& Sala::operator=(const Sala& s)
{
	if (this != &s)
	{
		ElibereazaMatrice();
		NumarSala = s.NumarSala;
		Capacitate = s.Capacitate;
		NumarRanduri = s.NumarRanduri;
		NumarColoane = s.NumarColoane;
		AlocaMatrice();
		for (int i = 0; i < NumarRanduri; i++)
			for (int j = 0; j < NumarColoane; j++)
				Matrice[i][j] = s.Matrice[i][j];
	}
	return *this;
}
void Sala::AlocaMatrice()
{
	if (NumarRanduri > 0 && NumarColoane > 0)
	{
		Matrice = new int* [NumarRanduri];
		for (int i = 0; i < NumarRanduri; i++)
			Matrice[i] = new int[NumarColoane];
	}
	else
	{
		Matrice = nullptr;
	}
}

void Sala::ElibereazaMatrice()
{
	if (Matrice)
	{
		for (int i = 0; i < NumarRanduri; i++)
			delete[] Matrice[i];
		delete[] Matrice;
		Matrice = nullptr;
	}
}