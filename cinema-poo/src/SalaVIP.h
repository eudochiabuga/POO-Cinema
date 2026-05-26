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
    SalaVIP(const SalaVIP& s);
    SalaVIP& operator=(const SalaVIP& s);
    ~SalaVIP();

    void Afisare() const override;
    void AdaugaProdusMeniu(const string& produs);
    void StergeProdusMeniu(const string& produs);
    void StergeMeniu();
    void AfisareMeniu() const;
    void AfisareHarta() const override;
    string Descriere() const override;

    double GetSuplimentPret() const override;
    bool EsteVIP() const override;
    string GetTipScaun() const { return TipScaun; }
    const vector<string>& GetMeniu() const { return MeniuDisponibil; }

    void SetTipScaun(const string& t) { TipScaun = t; }
    void SetSuplimentPret(double s) { SuplimentPret = s; }

    friend ostream& operator<<(ostream& out, const SalaVIP& s);
};
