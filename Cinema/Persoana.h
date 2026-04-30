#pragma once
#include <string>
using namespace std;


class Persoana
{
protected:
	string nume;
	int varsta;

public:
	Persoana(string n, int v) : nume(n), varsta(v){}
	virtual float GetDiscount() = 0;
	int GetVasrta() { return varsta; }
	virtual ~Persoana();

};

