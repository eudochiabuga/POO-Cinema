#include "Client.h"
#include <iostream>
using namespace std;

Client::Client(string nume):Persoana(nume){}

void Client::Afisare()
{
	cout << "Client: " << nume << endl;
}
