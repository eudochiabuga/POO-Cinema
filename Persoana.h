#pragma once
#include <string>
using namespace std;


class Persoana
{
protected:
	string nume;
	int varsta;

public:
	Persoana(string n, int v);
	virtual float GetDiscount() const = 0;
	virtual string GetTip() const = 0;
	int GetVarsta() const;
	string GetNume() const;
	virtual ~Persoana();

};

