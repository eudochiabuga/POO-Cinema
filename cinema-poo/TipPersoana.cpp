#include "TipPersoana.h"

Student::Student(string n, int v, string email, string telefon) : Persoana(n, v, email, telefon) {}
float  Student::GetDiscount() const { return 0.20f; }
string Student::GetTip()      const { return "Student"; }

Elev::Elev(string n, int v, string email, string telefon) : Persoana(n, v, email, telefon) {}
float  Elev::GetDiscount() const { return 0.15f; }
string Elev::GetTip()      const { return "Elev"; }

Adult::Adult(string n, int v, string email, string telefon) : Persoana(n, v, email, telefon) {}
float  Adult::GetDiscount() const { return 0.0f; }
string Adult::GetTip()      const { return "Adult"; }

Pensionar::Pensionar(string n, int v, string email, string telefon) : Persoana(n, v, email, telefon) {}
float  Pensionar::GetDiscount() const { return 0.30f; }
string Pensionar::GetTip()      const { return "Pensionar"; }

Copil::Copil(string n, int v, string email, string telefon) : Persoana(n, v, email, telefon) {}
float  Copil::GetDiscount() const { return 0.50f; }
string Copil::GetTip()      const { return "Copil"; }
