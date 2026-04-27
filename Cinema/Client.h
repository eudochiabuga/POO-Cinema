#pragma once
#include "Persoana.h"

class Client :public Persoana
{
public:
	Client(string nume);
	void Afisare() override;

};
