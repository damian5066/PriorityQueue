#include "HeapPQ.hpp"
#include <iostream>
using namespace std;

HeapPQ::HeapPQ() {
    // zaczynamy od malej tablicy
    capacity = 10;
    size = 0;
    heap = new Element[capacity];
}

HeapPQ::~HeapPQ() {
    delete[] heap;
}

void HeapPQ::swap(int i, int j) {
    Element tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

void HeapPQ::resize() {
    // podwajamy pojemnosc
    int newCapacity = capacity * 2;
    Element* newHeap = new Element[newCapacity];

    // kopiujemy stare dane
    for (int i = 0; i < size; i++) {
        newHeap[i] = heap[i];
    }

    delete[] heap;
    heap = newHeap;
    capacity = newCapacity;
}

void HeapPQ::heapifyUp(int index) {
    // dopoki nie jestesmy w korzeniu i ojciec ma mniejszy priorytet
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[parent].priority < heap[index].priority) {
            swap(parent, index);
            index = parent;
        }
        else {
			break; // aktualne miejsce jest poprawne
        }
    }
}

void HeapPQ::heapifyDown(int index) {
    // przywracamy wlasnosc kopca idac w dol
    while (true) {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        // sprawdzamy czy lewy syn istnieje i ma wiekszy priorytet
        if (left < size && heap[left].priority > heap[largest].priority) {
            largest = left;
        }

        // sprawdzamy prawego syna
        if (right < size && heap[right].priority > heap[largest].priority) {
            largest = right;
        }

        // jesli znalezlismy wiekszego syna to zamieniamy i idziemy dalej
        if (largest != index) {
            swap(index, largest);
            index = largest;
        }
        else {
            break; // kopiec jest poprawny
        }
    }
}

void HeapPQ::insert(int value, int priority) {
    // sprawdzamy czy trzeba powiekszye tablice
    if (size == capacity) {
        resize();
    }

    // dodajemy na koniec i przywracamy wlasnosc kopca
    heap[size] = Element(value, priority);
    heapifyUp(size);
    size++;
}

int HeapPQ::extractMax() {
    if (isEmpty()) {
        cout << "Kolejka jest pusta!" << endl;
        return -1;
    }

    int maxVal = heap[0].value; // max jest zawsze w korzeniu

    // wstawiamy ostatni element na miejsce korzenia i przywracamy kopiec
    heap[0] = heap[size - 1];
    size--;

    if (size > 0) {
        heapifyDown(0);
    }

    return maxVal;
}

int HeapPQ::findMax() const {
    if (isEmpty()) {
        cout << "Kolejka jest pusta!" << endl;
        return -1;
    }
    // korzen zawsze ma max priorytet - O(1)
    return heap[0].value;
}

void HeapPQ::modifyKey(int value, int newPriority) {
    // szukamy elementu o danej wartosci
    int index = -1;
    for (int i = 0; i < size; i++) {
        if (heap[i].value == value) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        //cout << "Nie znaleziono elementu o wartosci " << value << endl;
        return;
    }

    int oldPriority = heap[index].priority;
    heap[index].priority = newPriority;

    // w zaleznosci od tego czy zwiekszamy czy zmniejszamy priorytet
    // musimy przywrocic kopiec w innym kierunku
    if (newPriority > oldPriority) {
        heapifyUp(index);   // increase-key - idziemy w gore
    }
    else {
        heapifyDown(index); // decrease-key - idziemy w dol
    }

    //cout << "Zmieniono priorytet elementu " << value << " z " << oldPriority << " na " << newPriority << endl;
}

int HeapPQ::returnSize() const {
    return size;
}

bool HeapPQ::isEmpty() const {
    return size == 0;
}

void HeapPQ::display() const {
    if (isEmpty()) {
        cout << "Kolejka jest pusta." << endl;
        return;
    }

    cout << "Zawartosc kopca (wartosc, priorytet):" << endl;
    for (int i = 0; i < size; i++) {
        cout << "  [" << heap[i].value << ", prio=" << heap[i].priority << "]";
        if ((i + 1) % 5 == 0) cout << endl; // co 5 elementow nowa linia
    }
    cout << endl;
    cout << "Maksymalny element: wartosc=" << heap[0].value << ", priorytet=" << heap[0].priority << endl;
}
