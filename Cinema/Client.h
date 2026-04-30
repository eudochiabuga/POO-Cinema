#pragma once
#include "Persoana.h"

class Student : public Persoana
{
public:
	Student(string n, int v) : Persoana(n, v) {}
	float GetDiscount() override { return 0.20f; }
};

class Elev : public Persoana

{
public:
	Elev(string n, int v) : Persoana(n, v) {}
	float GetDiscount() override { return 0.15f; }
};

class Adult : public Persoana
{
public:
	Adult(string n, int v) : Persoana(n, v) {}
	float GetDiscount() override { return 0.0f; }
	
};
