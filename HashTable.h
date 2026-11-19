#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <ostream>
#include <stdexcept>
#include <string>
#include "Dict.h"
#include "TableEntry.h"

#include "../../P1/PRA_2627_P1/ListLinked.h" // ¡ADAPTA A TU RUTA!

template <typename V>
class HashTable : public Dict<V> {
private:
    int n;
    int max;
    ListLinked<TableEntry<V>>* table;

    // Función hash: suma de los valores ASCII módulo max
    int h(std::string key) {
        int sum = 0;
        for (size_t i = 0; i < key.length(); ++i) {
            sum += int(key.at(i));
        }
        return sum % max;
    }

public:
    // Constructor
    HashTable(int size) : n(0), max(size) {
        table = new ListLinked<TableEntry<V>>[size];
    }

    // Destructor
    ~HashTable() {
        delete[] table;
    }

    // Devuelve el número total de cubetas
    int capacity() {
        return max;
    }

    // Métodos heredados de Dict<V>
    void insert(std::string key, V value) override {
        int bucket = h(key);
        TableEntry<V> entry(key, value);
        if (table[bucket].search(entry) != -1) {
            throw std::runtime_error("Key '" + key + "' already exists!");
        }
        table[bucket].prepend(entry);
        n++;
    }

    V search(std::string key) override {
        int bucket = h(key);
        int pos = table[bucket].search(TableEntry<V>(key));
        if (pos == -1) {
            throw std::runtime_error("Key '" + key + "' not found!");
        }
        return table[bucket].get(pos).value;
    }

    V remove(std::string key) override {
        int bucket = h(key);
        int pos = table[bucket].search(TableEntry<V>(key));
        if (pos == -1) {
            throw std::runtime_error("Key '" + key + "' not found!");
        }
        TableEntry<V> removed = table[bucket].remove(pos);
        n--;
        return removed.value;
    }

    int entries() override {
        return n;
    }

    // Sobrecarga del operador []
    V operator[](std::string key) {
        return search(key);
    }

    // Sobrecarga del operador <<
    friend std::ostream& operator<<(std::ostream &out, const HashTable<V> &th) {
        out << "HashTable [entries: " << th.n << ", capacity: " << th.max << "]" << std::endl;
        out << "==============" << std::endl << std::endl;
        for (int i = 0; i < th.max; ++i) {
            out << "== Cubeta " << i << " ==" << std::endl << std::endl;
            out << th.table[i] << std::endl << std::endl;
        }
        out << "==============" << std::endl;
        return out;
    }
};

#endif
