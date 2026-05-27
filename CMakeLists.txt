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

# Directorul cu sursele C++
set(SRCDIR src)

# Sursele de baza (biblioteca interna, fara entry-point)
set(LIB_SOURCES
    ${SRCDIR}/Film.cpp
    ${SRCDIR}/Sala.cpp
    ${SRCDIR}/SalaVIP.cpp
    ${SRCDIR}/Spectacol.cpp
    ${SRCDIR}/Rezervare.cpp
    ${SRCDIR}/RezervareOnline.cpp
    ${SRCDIR}/Persoana.cpp
    ${SRCDIR}/TipPersoana.cpp
    ${SRCDIR}/Angajat.cpp
    ${SRCDIR}/TipAngajat.cpp
    ${SRCDIR}/SistemAutentificare.cpp
    ${SRCDIR}/Cinematograf.cpp
    ${SRCDIR}/Persistenta.cpp
    ${SRCDIR}/Raport.cpp
    ${SRCDIR}/AppContext.cpp
)

# Aplicatia consola
add_executable(cinema ${SRCDIR}/main_consola.cpp ${LIB_SOURCES})
target_include_directories(cinema PRIVATE ${SRCDIR})

# Serverul web
add_executable(cinema_web ${SRCDIR}/main_web.cpp ${LIB_SOURCES})
target_include_directories(cinema_web PRIVATE ${SRCDIR})
target_link_libraries(cinema_web pthread)

# Testele unitare
add_executable(test_cinema tests/test_cinema.cpp ${LIB_SOURCES})
target_include_directories(test_cinema PRIVATE ${SRCDIR})

enable_testing()
add_test(NAME unit_tests COMMAND test_cinema)
