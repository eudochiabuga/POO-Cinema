#pragma once
#include "Sala.h"


class SalaVIP : public Sala
{
private:
	float taxaProtocol;
	string* MeniuCatering;
	int NrProduse;
public:
	SalaVIP(string sala="0", int randuri=0, int coloane=0)
};