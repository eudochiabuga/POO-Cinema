// Cinema.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Film.h"
#include "Sala.h"
using namespace std;

int main()
{
    Film f1("Inception", "Actiune", 148, 13, "Engleza");
    Film f2("Avatar", "Actiune", 150, 12, "Engleza");

    cout << "Afisare film 1: " << endl;
    f1.Afisare();
    cout << "Afisare film 2: " << endl;
    f2.Afisare();
    f2 = f1;
    cout << "Afisare dupa atribuire: " << endl; 
    f2.Afisare();

    


}

