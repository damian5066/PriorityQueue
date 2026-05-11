#pragma once
#include "PriorityQueue.hpp"
#include "Element.hpp"

// Implementacja kolejki priorytetowej za pomoca nieposortowanej tablicy dynamicznej
// Tablice relokujemy przy dodawaniu i usuwaniu elementów zgodnie z wymaganiami
class ArrayPQ : public PriorityQueue {
private:
    Element* arr;   // tablica elementow
    int size;       // aktualna liczba elementow
    int capacity;   // aktualna pojemnosc

    // powieksza lub zmniejsza tablice
    void resize(int newCapacity);

    // zwraca indeks elementu o najwyzszym priorytecie
    int findMaxIndex() const;

public:
    ArrayPQ();
    ~ArrayPQ();

    void insert(int value, int priority) override;
    int extractMax() override;
    int findMax() const override;
    void modifyKey(int value, int newPriority) override;
    int returnSize() const override;
    void display() const override;
    bool isEmpty() const override;
};
