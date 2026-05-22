# ============================================================
#  CMakeLists.txt - alternativa moderna la Makefile
#
#  Build:
#    mkdir build && cd build
#    cmake ..
#    cmake --build .
#    ./cinema            # aplicatia
#    ./test_cinema       # testele
# ============================================================
cmake_minimum_required(VERSION 3.10)
project(CinemaPOO CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W3 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wno-deprecated-declarations)
endif()

# Sursele de baza (biblioteca interna, fara entry-point)
set(LIB_SOURCES
    Film.cpp
    Sala.cpp
    SalaVIP.cpp
    Spectacol.cpp
    Rezervare.cpp
    RezervareOnline.cpp
    Persoana.cpp
    TipPersoana.cpp
    Angajat.cpp
    TipAngajat.cpp
    SistemAutentificare.cpp
    Cinematograf.cpp
    Persistenta.cpp
    Raport.cpp
    AppContext.cpp
)

# Aplicatia consola
add_executable(cinema main_consola.cpp ${LIB_SOURCES})

# Testele unitare
add_executable(test_cinema tests/test_cinema.cpp ${LIB_SOURCES})
target_include_directories(test_cinema PRIVATE .)

enable_testing()
add_test(NAME unit_tests COMMAND test_cinema)
