# Sistem de Rezervari pentru un Cinema

Proiect de Programare Orientata pe Obiecte (POO) in C++. Implementeaza un
sistem complet pentru gestiunea unui cinema: filme, sali (normale si VIP),
spectacole, rezervari (online si la casa), cu calcul automat de pret.

```
  ╔═══════════════════════════════════════════════════════════╗
  ║     ██████╗██╗███╗   ██╗███████╗███╗   ███╗ █████╗        ║
  ║    ██╔════╝██║████╗  ██║██╔════╝████╗ ████║██╔══██╗       ║
  ║    ██║     ██║██╔██╗ ██║█████╗  ██╔████╔██║███████║       ║
  ║    ██║     ██║██║╚██╗██║██╔══╝  ██║╚██╔╝██║██╔══██║       ║
  ║    ╚██████╗██║██║ ╚████║███████╗██║ ╚═╝ ██║██║  ██║       ║
  ║     ╚═════╝╚═╝╚═╝  ╚═══╝╚══════╝╚═╝     ╚═╝╚═╝  ╚═╝       ║
  ║          Sistem de Rezervari - Cinema Online              ║
  ╚═══════════════════════════════════════════════════════════╝
```

## Cuprins

- Cerinte indeplinite
- Structura repository
- Cum compilezi si rulezi
- Concepte POO folosite
- Capturi din aplicatie
- Documentatie tehnica

## Cerinte indeplinite

### Obligatorii

- [x] Clase `Film`, `Sala`, `Rezervare` cu atribute si metode complete.
- [x] Relatii clare: `Cinematograf` agreaga `Sala`-i si `Film`-uri; o
      `Rezervare` are referinta la `Film` (prin `Spectacol`) si la un loc
      din `Sala`.
- [x] Metode: afiseaza filme, afiseaza locuri libere / ocupate intr-o sala,
      realizeaza rezervare.
- [x] Gestionare exceptii pentru loc deja ocupat (`LocOcupatException`) si
      index invalid (`LocInvalidException`).
- [x] Cod curat, modular, comentat.
- [x] Compilare cu `g++` standard (`-std=c++17`).
- [x] `Makefile` + `CMakeLists.txt` pentru build.
- [x] Repository Git cu commit-uri descriptive si branch `develop`.

### Facultative

- [x] **Interfata `ICinemaService`** cu metode pure virtuale, implementata
      de `Cinematograf` (`ICinemaService.h`).
- [x] **`RezervareOnline`** care extinde `Rezervare` cu `emailClient` si
      cod de confirmare auto-generat (`ONL-XXXX-id`).
- [x] **Calcul automat al pretului** in functie de:
      - ziua spectacolului (Vineri = -50%, Sambata/Duminica = +10%)
      - tipul filmului (2D vs 3D - supliment +20% pentru 3D)
      - tipul sala (normala vs VIP - supliment +50% pentru VIP)
      - tipul clientului (Adult, Student -20%, Elev -15%, Pensionar -30%,
        Copil -50%)
      - locul (ultimele 2 randuri intr-o sala normala sunt VIP, +30%)
- [x] **Teste unitare** (`tests/test_cinema.cpp`) - 17 teste, 42 assertii,
      framework propriu fara dependinte externe.
- [x] Git: peste 5 commit-uri descriptive, branch `develop`.

## Structura repository

```
cinema-poo/
├── README.md                <-- acest fisier
├── Makefile                 <-- build cu make
├── CMakeLists.txt           <-- build cu cmake
├── docs/
│   └── documentatie.md      <-- documentatie tehnica + UML
├── tests/
│   └── test_cinema.cpp      <-- teste unitare
├── ICinemaService.h         <-- interfata cu metode pure virtuale
├── Cinematograf.{h,cpp}     <-- clasa centrala (implementeaza ICinemaService)
├── Film.{h,cpp}             <-- film (titlu, gen, durata, 2D/3D, ...)
├── Sala.{h,cpp}             <-- sala cu matrice de locuri
├── SalaVIP.{h,cpp}          <-- mostenire publica din Sala
├── Spectacol.{h,cpp}        <-- film + sala + data + ora
├── Rezervare.{h,cpp}        <-- bilet cu calcul de pret
├── RezervareOnline.{h,cpp}  <-- mostenire din Rezervare + email
├── Persoana.{h,cpp}         <-- clasa abstracta
├── TipPersoana.{h,cpp}      <-- Adult/Student/Elev/Pensionar/Copil
├── Angajat.{h,cpp}          <-- baza pentru personal
├── TipAngajat.{h,cpp}       <-- Manager/Casier/Administrator
├── SistemAutentificare.*    <-- login + drepturi
├── Persistenta.{h,cpp}      <-- save/load in fisiere text
├── Raport.{h,cpp}           <-- statistici / top filme
├── Repository.h             <-- template generic (Repository<T>)
├── Exceptii.h               <-- ierarhie exceptii custom
├── UIConsola.h              <-- helper-e pt UI consola (ANSI, Unicode)
└── main_consola.cpp         <-- punct de intrare consola
```

> Codul este 100% portabil: foloseste doar STL si headers C++17 standard.
> Nu depinde de Windows Forms, .NET, MFC sau alte librarii non-standard.

## Cum compilezi si rulezi

### Cu `make` (Linux / macOS / MSYS2 / WSL)

```bash
make           # compileaza ./cinema
./cinema       # ruleaza aplicatia
make test      # compileaza si ruleaza testele unitare
make clean     # sterge fisierele generate
```

### Cu `g++` direct

```bash
g++ -std=c++17 -Wall -Wextra -o cinema main_consola.cpp \
    Film.cpp Sala.cpp SalaVIP.cpp Spectacol.cpp \
    Rezervare.cpp RezervareOnline.cpp \
    Persoana.cpp TipPersoana.cpp \
    Angajat.cpp TipAngajat.cpp \
    SistemAutentificare.cpp Cinematograf.cpp \
    Persistenta.cpp Raport.cpp AppContext.cpp
./cinema
```

### Cu CMake (cross-platform)

```bash
mkdir build && cd build
cmake ..
cmake --build .
./cinema             # aplicatia
./test_cinema        # testele unitare
```

### Pe Windows (cmd.exe, PowerShell, Windows Terminal)

Aplicatia activeaza automat ANSI escape codes + UTF-8 pe Windows 10+.
Recomandat Windows Terminal sau un terminal modern.

## Concepte POO folosite

| Concept | Unde apare | Exemplu |
|---------|-----------|---------|
| **Incapsulare** | toate clasele | atribute `private`/`protected`, getter-i / setter-i, invarianti verificati in constructor |
| **Mostenire** | `SalaVIP : public Sala`, `RezervareOnline : public Rezervare`, `Adult/Student/Elev : public Persoana`, `Manager/Casier : public Angajat` | extinderea functionalitatii |
| **Polimorfism** | `virtual void Afisare()`, `virtual double GetSuplimentPret()`, `virtual float GetDiscount() = 0` | comportament diferit in functie de tipul real |
| **Clase abstracte** | `Persoana`, `Angajat`, `ICinemaService` | metode pure virtuale `= 0` |
| **Interfete** | `ICinemaService` | toate metodele sunt `= 0`, contract pentru orice serviciu de cinema |
| **Sabloane (templates)** | `Repository<T>` | colectie generica cu `Adauga`, `GasestePrimul`, `Filtreaza`, `Sorteaza` |
| **Exceptii custom** | `CinemaException` si subclase | `LocOcupatException`, `LocInvalidException`, `VarstaInsuficientaException`, etc. |
| **STL** | `vector`, `map`, `string`, `function`, `algorithm` | folosit pe larg in `Repository`, `Raport`, `Persistenta` |
| **Operator overloading** | `operator==`, `operator<`, `operator<<` | `Film`, `Sala`, `Rezervare`, `Spectacol` |
| **Constructor de copiere + operator=** | `Sala`, `Spectacol`, `Film`, `Rezervare` | Rule of Three pentru clase cu memorie dinamica |
| **RAND** | matrice de locuri `int**` din `Sala`, `bool**` din `Spectacol` | alocare in constructor, eliberare in destructor |
| **Lambda / functori** | `Repository::Filtreaza`, `Sorteaza` | `[](const Film* f) { return f->GetCategorie() == "Drama"; }` |

## Capturi din aplicatie

Meniul principal cu colorare ANSI:

```
════════════════════════════════════════════════════════════
  CinePlex Iasi - Str. Mihai Eminescu nr. 25, Iasi
════════════════════════════════════════════════════════════
  1. Vezi filmele disponibile
  2. Vezi salile cinematografului
  3. Vezi spectacolele programate
  4. Harta locuri (libere / ocupate)
  5. Rezerva bilet
  6. Rezerva online (cu email)
  7. Vezi toate rezervarile
  8. Vezi incasarile
  9. Demo: exceptii in actiune
  0. Iesire
```

Harta interactiva a salii (legenda colorata):

```
                        ECRAN

         1  2  3  4  5  6  7  8
    R 1  □  □  □  □  □  □  □  □
    R 2  □  □  ■  □  □  □  □  □    <- ocupat la (2, 3)
    R 3  □  □  □  □  □  □  □  □
    R 4  □  □  □  □  □  □  □  □
    R 5  ♛  ♛  ♛  ♛  ♛  ♛  ♛  ♛    VIP
    R 6  ♛  ♛  ♛  ♛  ♛  ♛  ♛  ♛    VIP

  Legenda:  □ liber   ■ ocupat   ♛ VIP
```

## Cum ruleaza testele

```
╔═══════════════════════════════════════════════════════════╗
║         TESTE UNITARE - SISTEM CINEMA (POO)               ║
╚═══════════════════════════════════════════════════════════╝

  [ TEST ] Film: creare cu valori implicite si getter-i ... OK
  [ TEST ] Sala: alocare matrice + ocupare loc ... OK
  [ TEST ] SalaVIP: mostenire si polimorfism ... OK
  [ TEST ] Repository<T>: template, adauga, gaseste, ... OK
  [ TEST ] Rezervare: creare valida + loc devine ocupat ... OK
  [ TEST ] Rezervare: LocOcupatException ... OK
  [ TEST ] Rezervare: LocInvalidException ... OK
  [ TEST ] Rezervare: VarstaInsuficientaException ... OK
  [ TEST ] Calcul pret: 3D = 1.20 x 2D ... OK
  [ TEST ] Calcul pret: Student 20% discount ... OK
  [ TEST ] Calcul pret: Vineri -50% ... OK
  [ TEST ] Calcul pret: Weekend +10% ... OK
  [ TEST ] Calcul pret: Sala VIP +50% ... OK
  [ TEST ] RezervareOnline: cod confirmare ... OK
  [ TEST ] Cinematograf: cautari dupa ID si nume ... OK
  [ TEST ] ICinemaService: polimorfism ... OK
  [ TEST ] Spectacol: statistici ocupare ... OK

═══════════════════════════════════════════════════════════
  REZULTATE:  17 teste totale  |  17 trecute  |  0 esuate  |  42 assertii
═══════════════════════════════════════════════════════════
```

## Documentatie tehnica

Pentru detalii (diagrama UML, descrierea fiecarei clase, explicatii ale
conceptelor POO, posibile imbunatatiri), vezi
[`docs/documentatie.md`](docs/documentatie.md).

## Autor

Buga Eudochia
Tema 3121B - Sistem de Rezervari pentru un Cinema.
