#pragma once
#include "Persoana.h"

class Student : public Persoana {
public:
    Student(string n, int v);
    float GetDiscount() const override;
    string GetTip() const override;
};

class Elev : public Persoana {
public:
    Elev(string n, int v);
    float GetDiscount() const override;
    string GetTip() const override;
};

class Adult : public Persoana {
public:
    Adult(string n, int v);
    float GetDiscount()  const override;
    string GetTip() const override;
};