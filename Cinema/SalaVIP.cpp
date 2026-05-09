#include "SalaVIP.h"

SalaVIP::SalaVIP(string sala, int capacitate, int randuri, int coloane,
	double suplimentPret, string tipScaun)
	:Sala(sala, capacitate, randuri, coloane)
{
	SuplimentPret = suplimentPret;
	TipScaun = tipScaun;
}

SalaVIP::~SalaVIP()
{
	cout << "Sala VIP distrusă: " << GetNumarSala() << endl;
}

void SalaVIP::Afisare() const
{
	cout << "===Sala VIP " << GetNumarSala() << "==="<<endl;
	cout << "Scaune:" << TipScaun << "| Supliment pret: +" << SuplimentPret * 100 << "%" << endl;
	if (!MeniuDisponibil.empty())
	{
		cout << "Meniu disponibil: ";
		for (auto& produs : MeniuDisponibil)
			cout << produs << ", ";
		cout << endl;
	}
	cout << endl;
	Sala::Afisare();

}
void SalaVIP::AdaugaProdusMeniu(string produs)
{
	MeniuDisponibil.push_back(produs);
}
void SalaVIP::AfisareMeniu() const
{
	cout << "Meniu pentru sala " << GetNumarSala() << ":" << endl;
	for (auto& produs : MeniuDisponibil)
		cout << "- " << produs << endl;
}
ostream& operator<<(ostream & out, const SalaVIP & s)
{
	out << "Sala VIP " << s.GetNumarSala() << " (" << s.GetRanduri() << "x" << s.GetColoane()
		<< "), supliment pret " << s.SuplimentPret * 100 << "%, scaune: " << s.TipScaun;
	return out;
}