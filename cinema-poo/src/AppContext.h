#pragma once
#include "Cinematograf.h"
#include "SistemAutentificare.h"
#include <string>

// CinemaContext - tine referinte la obiectele globale Cinematograf si SistemAutentificare.
// Numit "CinemaContext" pentru a evita conflict cu System::AppContext din .NET.
class CinemaContext {
public:
    static Cinematograf* cinema;
    static SistemAutentificare* auth;
    static std::string folderDate;
};
