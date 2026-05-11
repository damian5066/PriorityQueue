#pragma once
#include "PriorityQueue.hpp"
#include "Element.hpp"

// Implementacja kolejki priorytetowej MAX za pomoca kopca binarnego
// Kopiec binarny to drzewo binarne ktore spelnia wlasnosc kopca:
// rodzic ma zawsze wiekszy lub rowny priorytet niz jego dzieci
// Przechowujemy kopiec w tablicy
// Dla elementu na pozycji i:
//   lewy syn: 2*i + 1
//   prawy syn: 2*i + 2
//   rodzic: (i-1) / 2
class HeapPQ : public PriorityQueue {
private:
    Element* heap;   // tablica przechowujaca elementy kopca
    int size;        // aktualna liczba elementow
    int capacity;    // pojemnosc tablicy

    // powiekszenie tablicy gdy brakuje miejsca
    void resize();

    // przywracanie wlasnosci kopca w gore (po insercie)
    void heapifyUp(int index);

    // przywracanie wlasnosci kopca w dol (po extractMax)
    void heapifyDown(int index);

    // zamiana dwoch elementow miejscami
    void swap(int i, int j);

public:
    HeapPQ();
    ~HeapPQ();

    void insert(int value, int priority) override;
    int extractMax() override;
    int findMax() const override;
    void modifyKey(int value, int newPriority) override;
    int returnSize() const override;
    void display() const override;
    bool isEmpty() const override;
};
