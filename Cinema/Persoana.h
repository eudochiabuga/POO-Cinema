#pragma once
#include <string>
using namespace std;


class Persoana
{
protected:
	string nume;

public:
	Persoana(string nume) : nume(nume){}
    virtual void Afisare() = 0;
	virtual ~Persoana();

};

