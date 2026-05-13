#pragma once
#include <vector>
#include <functional>
#include <algorithm>
using namespace std;

// Repository<T> - clasa template generica pentru gestionarea unei colectii de obiecte de tip T*
// Demonstreaza folosirea template-urilor si a functorilor.
template <class T>
class Repository {
private:
    vector<T*> elemente;
    bool detineMemoria; // daca true, distructorul sterge obiectele

public:
    Repository(bool detineMemoria = false) : detineMemoria(detineMemoria) {}

    ~Repository() {
        if (detineMemoria)
            for (auto e : elemente)
                delete e;
    }

    // Adaugare
    void Adauga(T* element) {
        if (element) elemente.push_back(element);
    }

    // Sterge un element (fara a-l elibera daca nu detinem memoria)
    bool Sterge(T* element) {
        auto it = find(elemente.begin(), elemente.end(), element);
        if (it == elemente.end()) return false;
        if (detineMemoria) delete *it;
        elemente.erase(it);
        return true;
    }

    bool StergeIndex(size_t index) {
        if (index >= elemente.size()) return false;
        if (detineMemoria) delete elemente[index];
        elemente.erase(elemente.begin() + index);
        return true;
    }

    // Acces
    T* operator[](size_t index) const {
        if (index >= elemente.size()) return nullptr;
        return elemente[index];
    }

    size_t Marime() const { return elemente.size(); }
    bool Vid() const { return elemente.empty(); }

    // Returneaza colectia interna (read-only)
    const vector<T*>& GetToate() const { return elemente; }

    // Cautare cu predicat (functor / lambda)
    T* GasestePrimul(function<bool(const T*)> predicat) const {
        for (auto e : elemente)
            if (predicat(e)) return e;
        return nullptr;
    }

    vector<T*> Filtreaza(function<bool(const T*)> predicat) const {
        vector<T*> rezultat;
        for (auto e : elemente)
            if (predicat(e)) rezultat.push_back(e);
        return rezultat;
    }

    // Sortare cu comparator
    void Sorteaza(function<bool(const T*, const T*)> comparator) {
        sort(elemente.begin(), elemente.end(), comparator);
    }

    // Iteratori (pentru range-based for)
    typename vector<T*>::iterator begin() { return elemente.begin(); }
    typename vector<T*>::iterator end() { return elemente.end(); }
    typename vector<T*>::const_iterator begin() const { return elemente.begin(); }
    typename vector<T*>::const_iterator end() const { return elemente.end(); }
};
