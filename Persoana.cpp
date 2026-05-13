#include "Persoana.h"

Persoana::Persoana(string n, int v) : nume(n), varsta(v) {}

int    Persoana::GetVarsta() const { return varsta; }
string Persoana::GetNume()   const { return nume; }

Persoana::~Persoana() {}
