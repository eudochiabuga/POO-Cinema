#pragma once
#include "Persoana.h"

class Student : public Persoana {
public:
    Student(string n, int v, string email = "", string telefon = "");
    float  GetDiscount() const override;
    string GetTip()      const override;
};

class Elev : public Persoana {
public:
    Elev(string n, int v, string email = "", string telefon = "");
    float  GetDiscount() const override;
    string GetTip()      const override;
};

class Adult : public Persoana {
public:
    Adult(string n, int v, string email = "", string telefon = "");
    float  GetDiscount() const override;
    string GetTip()      const override;
};

// Pensionar - discount 30%
class Pensionar : public Persoana {
public:
    Pensionar(string n, int v, string email = "", string telefon = "");
    float  GetDiscount() const override;
    string GetTip()      const override;
};

// Copil sub 14 ani - discount 50%
class Copil : public Persoana {
public:
    Copil(string n, int v, string email = "", string telefon = "");
    float  GetDiscount() const override;
    string GetTip()      const override;
};
