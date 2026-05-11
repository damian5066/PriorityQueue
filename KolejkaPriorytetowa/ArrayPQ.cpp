#include "ArrayPQ.hpp"
#include <iostream>
using namespace std;

ArrayPQ::ArrayPQ() {
    capacity = 10;
    size = 0;
    arr = new Element[capacity];
}

ArrayPQ::~ArrayPQ() {
    delete[] arr;
}

void ArrayPQ::resize(int newCapacity) {
    Element* newArr = new Element[newCapacity];

    // kopiujemy tyle ile mamy elementow (nie wiecej niz nowa pojemnosc)
    int toCopy = size < newCapacity ? size : newCapacity;
    for (int i = 0; i < toCopy; i++) {
        newArr[i] = arr[i];
    }

    delete[] arr;
    arr = newArr;
    capacity = newCapacity;
}

int ArrayPQ::findMaxIndex() const {
    if (size == 0) return -1;

    int maxIdx = 0;
    for (int i = 1; i < size; i++) {
        if (arr[i].priority > arr[maxIdx].priority) {
            maxIdx = i;
        }
    }
    return maxIdx;
}

void ArrayPQ::insert(int value, int priority) {
    // jesli brakuje miejsca - powiekszamy tablice
    if (size == capacity) {
        resize(capacity * 2);
    }

    // dodajemy na koniec(O(1))
    arr[size] = Element(value, priority);
    size++;
}

int ArrayPQ::extractMax() {
    if (isEmpty()) {
        cout << "Kolejka jest pusta!" << endl;
        return -1;
    }

    int maxIdx = findMaxIndex(); // O(n) - przeszukanie całej tablicy
    int maxVal = arr[maxIdx].value;

    // usuwamy element przesuwajac pozostale
    arr[maxIdx] = arr[size - 1];
    size--;

	// zmniejszamy tablice jesli zajeta jest mniej niz 1/4 pojemnosci zeby nie marnowac pamieci
    if (size > 0 && size <= capacity / 4 && capacity > 10) {
        resize(capacity / 2);
    }

    return maxVal;
}

int ArrayPQ::findMax() const {
    if (isEmpty()) {
        cout << "Kolejka jest pusta!" << endl;
        return -1;
    }

    // musimy przejrzec cala tablice
    return arr[findMaxIndex()].value;
}

void ArrayPQ::modifyKey(int value, int newPriority) {
    // szukamy elementu liniowo
    for (int i = 0; i < size; i++) {
        if (arr[i].value == value) {
           // cout << "Zmieniono priorytet elementu " << value
                //<< " z " << arr[i].priority << " na " << newPriority << endl;
            arr[i].priority = newPriority;
            return;
        }
    }
    //cout << "Nie znaleziono elementu o wartosci " << value << endl;
}

int ArrayPQ::returnSize() const {
    return size;
}

bool ArrayPQ::isEmpty() const {
    return size == 0;
}

void ArrayPQ::display() const {
    if (isEmpty()) {
        cout << "Kolejka jest pusta." << endl;
        return;
    }

    int maxIdx = findMaxIndex();
    cout << "Zawartosc tablicy (wartosc, priorytet):" << endl;
    for (int i = 0; i < size; i++) {
        cout << "  [" << arr[i].value << ", prio=" << arr[i].priority << "]";
        if ((i + 1) % 5 == 0) cout << endl;
    }
    cout << endl;
    cout << "Maksymalny element: wartosc=" << arr[maxIdx].value
        << ", priorytet=" << arr[maxIdx].priority << endl;
}
