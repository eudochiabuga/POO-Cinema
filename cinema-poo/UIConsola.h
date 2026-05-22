#pragma once
// ============================================================
//  UIConsola.h - Helper-e pentru o consola moderna:
//   - culori ANSI (functioneaza pe Linux/macOS si Windows 10+)
//   - chenare Unicode (box drawing characters)
//   - banner ASCII
//   - utilitare de citire si formatare
//
//  Toate functiile sunt in namespace UI ca sa nu polueze global.
//  Pe Windows, EnableAnsi() activeaza ANSI escape codes si UTF-8.
// ============================================================
#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <sstream>

#ifdef _WIN32
  // Inainte de windows.h:
  //  NOMINMAX           -> dezactiveaza macro-urile min/max care sparg std::numeric_limits::max()
  //  WIN32_LEAN_AND_MEAN-> reduce headerele incluse, eliminand conflict 'byte' cu std::byte
  //  _HAS_STD_BYTE 0    -> dezactiveaza std::byte (evita ambiguitate cu byte din rpcndr.h)
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif
  #ifndef _HAS_STD_BYTE
    #define _HAS_STD_BYTE 0
  #endif
  #include <windows.h>
#endif

namespace UI {
    // ---- Culori ANSI ----
    inline constexpr const char* RESET = "\033[0m";
    inline constexpr const char* BOLD = "\033[1m";
    inline constexpr const char* DIM = "\033[2m";
    inline constexpr const char* ITALIC = "\033[3m";
    inline constexpr const char* UNDERLINE = "\033[4m";

    inline constexpr const char* BLACK = "\033[30m";
    inline constexpr const char* RED = "\033[31m";
    inline constexpr const char* GREEN = "\033[32m";
    inline constexpr const char* YELLOW = "\033[33m";
    inline constexpr const char* BLUE = "\033[34m";
    inline constexpr const char* MAGENTA = "\033[35m";
    inline constexpr const char* CYAN = "\033[36m";
    inline constexpr const char* WHITE = "\033[37m";
    inline constexpr const char* GRAY = "\033[90m";

    inline constexpr const char* BG_BLUE = "\033[44m";
    inline constexpr const char* BG_RED = "\033[41m";
    inline constexpr const char* BG_GREEN = "\033[42m";

    // Activeaza ANSI / UTF-8 pe Windows. No-op pe alte sisteme.
    inline void EnableAnsi() {
#ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        if (GetConsoleMode(h, &mode))
            SetConsoleMode(h, mode | 0x0004 /*ENABLE_VIRTUAL_TERMINAL_PROCESSING*/);
#endif
    }

    // ---- Linii orizontale ----
    inline void HR(int n = 60, const std::string& ch = "─", const char* color = CYAN) {
        std::cout << color;
        for (int i = 0; i < n; i++) std::cout << ch;
        std::cout << RESET << "\n";
    }

    inline void HRDouble(int n = 60, const char* color = CYAN) {
        std::cout << color;
        for (int i = 0; i < n; i++) std::cout << "═";
        std::cout << RESET << "\n";
    }

    // ---- Banner ASCII ----
    inline void Banner() {
        std::cout << CYAN << BOLD;
        std::cout << "\n";
        std::cout << "  ╔═══════════════════════════════════════════════════════════╗\n";
        std::cout << "  ║                                                           ║\n";
        std::cout << "  ║     ██████╗██╗███╗   ██╗███████╗███╗   ███╗ █████╗        ║\n";
        std::cout << "  ║    ██╔════╝██║████╗  ██║██╔════╝████╗ ████║██╔══██╗       ║\n";
        std::cout << "  ║    ██║     ██║██╔██╗ ██║█████╗  ██╔████╔██║███████║       ║\n";
        std::cout << "  ║    ██║     ██║██║╚██╗██║██╔══╝  ██║╚██╔╝██║██╔══██║       ║\n";
        std::cout << "  ║    ╚██████╗██║██║ ╚████║███████╗██║ ╚═╝ ██║██║  ██║       ║\n";
        std::cout << "  ║     ╚═════╝╚═╝╚═╝  ╚═══╝╚══════╝╚═╝     ╚═╝╚═╝  ╚═╝       ║\n";
        std::cout << "  ║                                                           ║\n";
        std::cout << "  ║          " << YELLOW << "Sistem de Rezervari - Cinema Online" << CYAN << "            ║\n";
        std::cout << "  ║                                                           ║\n";
        std::cout << "  ╚═══════════════════════════════════════════════════════════╝\n";
        std::cout << RESET << "\n";
    }

    // ---- Casete cu titlu ----
    inline void Titlu(const std::string& t, const char* color = CYAN) {
        const int LATIME = 60;
        int padTotal = LATIME - 2 - (int)t.size();
        if (padTotal < 0) padTotal = 0;
        int padL = padTotal / 2;
        int padR = padTotal - padL;

        std::cout << "\n" << color << BOLD;
        std::cout << "┌";
        for (int i = 0; i < LATIME; i++) std::cout << "─";
        std::cout << "┐\n";
        std::cout << "│" << std::string(padL, ' ') << t << std::string(padR, ' ') << "│\n";
        std::cout << "└";
        for (int i = 0; i < LATIME; i++) std::cout << "─";
        std::cout << "┘" << RESET << "\n";
    }

    // ---- Mesaje colorate ----
    inline void Info(const std::string& m) {
        std::cout << CYAN << "  i  " << RESET << m << "\n";
    }
    inline void Succes(const std::string& m) {
        std::cout << GREEN << BOLD << "  OK " << RESET << GREEN << m << RESET << "\n";
    }
    inline void Avertizare(const std::string& m) {
        std::cout << YELLOW << BOLD << "  !  " << RESET << YELLOW << m << RESET << "\n";
    }
    inline void Eroare(const std::string& m) {
        std::cout << RED << BOLD << "  X  " << RESET << RED << m << RESET << "\n";
    }

    // ---- Prompt ----
    inline std::string Prompt(const std::string& text) {
        std::cout << MAGENTA << "  > " << RESET << text << ": ";
        return "";
    }

    // ---- Citire numar intreg cu validare ----
    inline int CitesteInt(const std::string& mesaj, int minim = 0, int maxim = 999999) {
        int val;
        while (true) {
            std::cout << MAGENTA << "  > " << RESET << mesaj << ": ";
            if (std::cin >> val && val >= minim && val <= maxim) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return val;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            Eroare("Valoare invalida. Introdu un numar intre " +
                std::to_string(minim) + " si " + std::to_string(maxim) + ".");
        }
    }

    // ---- Citire string ----
    inline std::string CitesteString(const std::string& mesaj) {
        std::cout << MAGENTA << "  > " << RESET << mesaj << ": ";
        std::string s;
        std::getline(std::cin, s);
        return s;
    }

    // ---- Apasa Enter ----
    inline void Pauza() {
        std::cout << GRAY << "\n  (apasa Enter pentru a continua...)" << RESET;
        std::cin.get();
    }

    // ---- Curata ecranul (cross-platform) ----
    inline void Clear() {
        std::cout << "\033[2J\033[H";
    }
}
