#pragma once
#include "Rezervare.h"
#include <string>

// RezervareOnline extinde Rezervare cu informatii specifice
// cumpararii online: emailul clientului si codul de confirmare.
// Demonstreaza mostenirea si extinderea unei clase existente.
class RezervareOnline : public Rezervare
{
    std::string emailClient;
    std::string codConfirmare;   // generat automat

    static std::string GenereazaCod(int id);

public:
    RezervareOnline(Persoana* c, Spectacol* sp, int rand, int col,
        const std::string& email);

    std::string GetEmailClient()    const { return emailClient; }
    std::string GetCodConfirmare()  const { return codConfirmare; }

    void SetEmailClient(const std::string& e) { emailClient = e; }

    // Suprascrie afisarea pentru a include detaliile online
    void Afisare() const;
    std::string ToString() const;

    friend std::ostream& operator<<(std::ostream& out, const RezervareOnline& r);
};