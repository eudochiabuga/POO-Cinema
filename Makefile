# ============================================================
#  Makefile - Sistem de Rezervari Cinema
#
#  Tinte:
#    make          - compileaza aplicatia consola (./cinema)
#    make web      - compileaza serverul web (./cinema_web)
#    make test     - compileaza si ruleaza testele unitare
#    make run      - compileaza si ruleaza aplicatia
#    make clean    - sterge fisierele compilate
#    make help     - afiseaza acest mesaj
# ============================================================

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-deprecated-declarations -O2
LDFLAGS  =

SRCDIR   = src

# Sursele de baza (BIBLIOTECA - fara entry-point)
LIB_SRCS = $(SRCDIR)/Film.cpp $(SRCDIR)/Sala.cpp $(SRCDIR)/SalaVIP.cpp \
           $(SRCDIR)/Spectacol.cpp \
           $(SRCDIR)/Rezervare.cpp $(SRCDIR)/RezervareOnline.cpp \
           $(SRCDIR)/Persoana.cpp $(SRCDIR)/TipPersoana.cpp \
           $(SRCDIR)/Angajat.cpp $(SRCDIR)/TipAngajat.cpp \
           $(SRCDIR)/SistemAutentificare.cpp \
           $(SRCDIR)/Cinematograf.cpp \
           $(SRCDIR)/Persistenta.cpp $(SRCDIR)/Raport.cpp \
           $(SRCDIR)/AppContext.cpp

TARGET   = cinema
WEB_BIN  = cinema_web
TEST_BIN = test_cinema

# ---- Aplicatie principala ----
all: $(TARGET)

$(TARGET): $(LIB_SRCS) $(SRCDIR)/main_consola.cpp
	@echo ">>> Compilare aplicatie cu g++..."
	$(CXX) $(CXXFLAGS) -I$(SRCDIR) -o $(TARGET) $(SRCDIR)/main_consola.cpp $(LIB_SRCS) $(LDFLAGS)
	@echo ">>> Gata! Ruleaza ./$(TARGET)"

# ---- Server web ----
web: $(WEB_BIN)

$(WEB_BIN): $(SRCDIR)/main_web.cpp $(LIB_SRCS)
	@echo ">>> Compilare server web..."
	$(CXX) $(CXXFLAGS) -I$(SRCDIR) -o $(WEB_BIN) $(SRCDIR)/main_web.cpp $(LIB_SRCS) -lpthread
	@echo ">>> Gata! Ruleaza ./$(WEB_BIN) si deschide http://localhost:8080"

# ---- Rulare ----
run: $(TARGET)
	./$(TARGET)

# ---- Teste unitare ----
test: $(TEST_BIN)
	@echo ">>> Rulare teste..."
	@./$(TEST_BIN)

$(TEST_BIN): $(LIB_SRCS) tests/test_cinema.cpp
	@echo ">>> Compilare teste cu g++..."
	$(CXX) $(CXXFLAGS) -I$(SRCDIR) -o $(TEST_BIN) tests/test_cinema.cpp $(LIB_SRCS) $(LDFLAGS)

# ---- Curatare ----
clean:
	rm -f $(TARGET) $(WEB_BIN) $(TEST_BIN) *.o src/*.o tests/*.o

# ---- Help ----
help:
	@echo "Sistem de Rezervari Cinema - tinte make disponibile:"
	@echo "  make        - compileaza aplicatia principala (./cinema)"
	@echo "  make run    - compileaza si ruleaza aplicatia"
	@echo "  make web    - compileaza serverul web (./cinema_web)"
	@echo "  make test   - compileaza si ruleaza testele unitare"
	@echo "  make clean  - sterge fisierele compilate"

.PHONY: all run web test clean help
