#pragma once

// Klasa bazowa (interfejs) dla kolejki priorytetowej
// Wszystkie implementacje muszą dziedziczyć po tej klasie
class PriorityQueue {
public:
    // destruktor wirtualny żeby działało dziedziczenie
    virtual ~PriorityQueue() {}

    // dodanie elementu z priorytetem
    virtual void insert(int value, int priority) = 0;

    // usuniecie i zwrocenie elementu o najwyzszym priorytecie
    virtual int extractMax() = 0;

    // podejrzenie max elementu bez usuwania
    virtual int findMax() const = 0;

    // zmiana priorytetu elementu
    virtual void modifyKey(int value, int newPriority) = 0;

    // rozmiar kolejki
    virtual int returnSize() const = 0;

    // wyswietlenie zawartosci
    virtual void display() const = 0;

    // sprawdzenie czy pusta
    virtual bool isEmpty() const = 0;
};
