#pragma once
#include "Sala.h"
#include <vector>



class SalaVIP : public Sala
{
private:
	vector<string> MeniuDisponibil;
	double SuplimentPret;
	string TipScaun;
	
public:
	SalaVIP(string sala = "VIP", int capacitate = 0, int randuri = 0, int coloane = 0,
		double suplimentPret = 0.5, string tipScaun = "Recliner");
	~SalaVIP();
	void Afisare() const;
	void AdaugaProdusMeniu(string produs);
	void AfisareMeniu() const;
	friend ostream& operator<<(ostream & out, const SalaVIP & s);


};