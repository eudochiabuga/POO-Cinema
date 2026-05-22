# ============================================================
#  Makefile - Sistem de Rezervari Cinema
#
#  Tinte:
#    make          - compileaza aplicatia consola (./cinema)
#    make test     - compileaza si ruleaza testele unitare
#    make run      - compileaza si ruleaza aplicatia
#    make clean    - sterge fisierele compilate
#    make help     - afiseaza acest mesaj
# ============================================================

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-deprecated-declarations -O2
LDFLAGS  =

# Sursele de baza (BIBLIOTECA - fara entry-point)
LIB_SRCS = Film.cpp Sala.cpp SalaVIP.cpp Spectacol.cpp \
           Rezervare.cpp RezervareOnline.cpp \
           Persoana.cpp TipPersoana.cpp \
           Angajat.cpp TipAngajat.cpp \
           SistemAutentificare.cpp \
           Cinematograf.cpp \
           Persistenta.cpp Raport.cpp \
           AppContext.cpp

TARGET   = cinema
TEST_BIN = test_cinema

# ---- Aplicatie principala ----
all: $(TARGET)

$(TARGET): $(LIB_SRCS) main_consola.cpp
	@echo ">>> Compilare aplicatie cu g++..."
	$(CXX) $(CXXFLAGS) -o $(TARGET) main_consola.cpp $(LIB_SRCS) $(LDFLAGS)
	@echo ">>> Gata! Ruleaza ./$(TARGET)"

# ---- Rulare ----
run: $(TARGET)
	./$(TARGET)

# ---- Teste unitare ----
test: $(TEST_BIN)
	@echo ">>> Rulare teste..."
	@./$(TEST_BIN)

$(TEST_BIN): $(LIB_SRCS) tests/test_cinema.cpp
	@echo ">>> Compilare teste cu g++..."
	$(CXX) $(CXXFLAGS) -I. -o $(TEST_BIN) tests/test_cinema.cpp $(LIB_SRCS) $(LDFLAGS)

# ---- Curatare ----
clean:
	rm -f $(TARGET) $(TEST_BIN) *.o tests/*.o

# ---- Help ----
help:
	@echo "Sistem de Rezervari Cinema - tinte make disponibile:"
	@echo "  make        - compileaza aplicatia principala (./cinema)"
	@echo "  make run    - compileaza si ruleaza aplicatia"
	@echo "  make test   - compileaza si ruleaza testele unitare"
	@echo "  make clean  - sterge fisierele compilate"

.PHONY: all run test clean help
