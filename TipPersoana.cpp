#include "TipPersoana.h"

Student::Student(string n, int v) : Persoana(n, v) {}
float  Student::GetDiscount() const  { return 0.20f; }
string Student::GetTip()  const    { return "Student"; }

Elev::Elev(string n, int v) : Persoana(n, v) {}
float  Elev::GetDiscount() const { return 0.15f; }
string Elev::GetTip()   const   { return "Elev"; }

Adult::Adult(string n, int v) : Persoana(n, v) {}
float  Adult::GetDiscount() const  { return 0.0f; }
string Adult::GetTip()  const    { return "Adult"; }
