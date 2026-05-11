#include <iostream>
#include <random> 
#include <ctime>
#include <fstream>
#include <string>
#include "HeapPQ.hpp"
#include "ArrayPQ.hpp"
#include "Timer.hpp"

using namespace std;

mt19937 rng;

// struktura przechowujaca wyniki jednego pomiaru dla jednej struktury
struct BenchmarkResult {
    string structureName;
    int size;
    long long insert_ns;
    long long findMax_ns;
    long long extractMax_ns;
    long long modifyKey_ns;
    long long returnSize_ns;
};


// generuje losowy priorytet - zakres kilkukrotnie wiekszy niz rozmiar struktur
int randomPriority(int structureSize) {
    uniform_int_distribution<int> dist(1, structureSize * 5);
    return dist(rng);
}

// generuje losowa wartosc elementu
int randomValue() {
    uniform_int_distribution<int> dist(1, 100000);
    return dist(rng);
}

// wypelnia kolejke losowymi danymi
void fillRandom(PriorityQueue* pq, int n) {
    for (int i = 0; i < n; i++) {
        pq->insert(randomValue(), randomPriority(n));
    }
}


// przeprowadza pomiary dla jednej implementacji, wypisuje wyniki i zwraca je
// kazdy pomiar to srednia z kilku powtorzen
BenchmarkResult benchmark(PriorityQueue* pq, const string& name, int structureSize, int repetitions) {
    Timer t;
    long long totalTime;
    BenchmarkResult result;
    result.structureName = name;
    result.size = structureSize;

    cout << "\n " << name << " | rozmiar=" << structureSize << " | liczba powtorzen=" << repetitions <<endl;

    // INSERT 
    totalTime = 0;
    for (int r = 0; r < repetitions; r++) {
        // tworzymy nowy element do wstawienia (nie liczymy tworzenia)
        int val = randomValue();
        int prio = randomPriority(structureSize);

        t.start();
        pq->insert(val, prio);
        totalTime += t.stop();

        // usuwamy zeby rozmiar sie nie zmienial miedzy powtorzeniami
        pq->extractMax();
    }
    result.insert_ns = totalTime / repetitions;
    cout << "insert:      " << result.insert_ns << " ns" << endl;

    // FIND-MAX 
    totalTime = 0;
    for (int r = 0; r < repetitions; r++) {
        t.start();
        pq->findMax();
        totalTime += t.stop();
    }
    result.findMax_ns = totalTime / repetitions;
    cout << "find-max:    " << result.findMax_ns << " ns" << endl;

    // EXTRACT-MAX
	// extract-max usuwa element, wiec po kazdym pomiarze wstawiamy nowy losowy element zeby rozmiar sie nie zmienial miedzy powtorzeniami
	// czas dodania elementu nie jest liczony, bo mierzymy tylko extract-max, ale musimy to zrobic zeby test byl rzetelny
    totalTime = 0;
    for (int r = 0; r < repetitions; r++) {

        t.start();

        auto val = pq->extractMax();

        totalTime += t.stop();

        pq->insert(randomValue(), randomPriority(structureSize));
    }
    result.extractMax_ns = totalTime / repetitions;
    cout << "extract-max: " << result.extractMax_ns << " ns" << endl;

    // MODIFY-KEY
    // modify-key wymaga znajomosci jakiejs wartosci w strukturze
	// wstawiamy losowe znane wartosci przed pomiarem, zeby miec pewnosc ze istnieja w strukturze
    vector<int> knownValues;
    for (int i = 0; i < repetitions; i++) {
		pq->extractMax(); // usuwamy najpierw tyle elementow ile bedziemy wstawiać, zeby rozmiar został stały
		// w osobnej pętli, żeby mieć pewność, że przy usuwaniu nie trafimy na elementy, które dopiero co wstawiliśmy do testu
    }
    for (int i = 0; i < repetitions; i++) {
        int val = randomValue();
        knownValues.push_back(val);
        pq->insert(val, randomPriority(structureSize));
    }

    totalTime = 0;
    for (int r = 0; r < repetitions; r++) {

        int val = knownValues[r];
        int newPrio = randomPriority(structureSize);

        t.start();
        pq->modifyKey(val, newPrio);
        totalTime += t.stop();
    }
    result.modifyKey_ns = totalTime / repetitions;
    cout << "modify-key:  " << result.modifyKey_ns << " ns" << endl;

    // RETURN-SIZE 
    totalTime = 0;
    for (int r = 0; r < repetitions; r++) {
        t.start();
        pq->returnSize();
        totalTime += t.stop();
    }
    result.returnSize_ns = totalTime / repetitions;
    cout << "return-size: " << result.returnSize_ns << " ns" << endl;

    return result;
}

// glowna funkcja badan - testuje obie struktury na tych samych danych
void runBenchmarks(int seed) {
    // rozmiary struktur 
    int sizes[] = { 5000, 8000, 10000, 16000, 20000, 40000, 60000, 100000 };
    int numSizes = 8;
    int repetitions = 100; // liczba powtorzen dla uśrednienia

    cout << "WYNIKI BADAN" << endl;
    cout << "Liczba powtorzen na pomiar: " << repetitions << endl;

    // zbieramy wszystkie wyniki zeby zapisac do CSV
    BenchmarkResult wyniki[16];
    int liczbaWynikow = 0;

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];

        // generujemy te same dane dla obu struktur
        // najpierw zapisujemy je do tablicy pomocniczej
        int* values = new int[n];
        int* priorities = new int[n];
        for (int i = 0; i < n; i++) {
            values[i] = randomValue();
            priorities[i] = randomPriority(n);
        }

        // wypelniamy kopiec
        HeapPQ* heap = new HeapPQ();
        for (int i = 0; i < n; i++) {
            heap->insert(values[i], priorities[i]);
        }

        // wypelniamy tablice tymi samymi danymi
        ArrayPQ* arr = new ArrayPQ();
        for (int i = 0; i < n; i++) {
            arr->insert(values[i], priorities[i]);
        }

        // pomiary, zapisujemy zwrocone wyniki
        wyniki[liczbaWynikow++] = benchmark(heap, "Kopiec_binarny", n, repetitions);
        wyniki[liczbaWynikow++] = benchmark(arr, "Tablica_dynamiczna", n, repetitions);

        delete heap;
        delete arr;
        delete[] values;
        delete[] priorities;
    }

    cout << "Badania zakonczone." << endl;

    // Zapis pomiarów do pliku
    string nazwaPliku = "wyniki_badan_seed_" + to_string(seed) + ".csv";
    ofstream csv(nazwaPliku.c_str());

    if (!csv.is_open()) {
        cout << "Blad: nie mozna otworzyc pliku " << nazwaPliku << " do zapisu!" << endl;
        return;
    }

	// naglowek CSV - sredniki jako separator 
    csv << "struktura;rozmiar;insert_ns;find_max_ns;extract_max_ns;modify_key_ns;return_size_ns" << endl;

    for (int i = 0; i < liczbaWynikow; i++) {
        csv << wyniki[i].structureName << ";"
            << wyniki[i].size << ";"
            << wyniki[i].insert_ns << ";"
            << wyniki[i].findMax_ns << ";"
            << wyniki[i].extractMax_ns << ";"
            << wyniki[i].modifyKey_ns << ";"
            << wyniki[i].returnSize_ns << endl;
    }

    csv.close();
    cout << "Wyniki zapisano do pliku: " << nazwaPliku << endl;
}

// MENU
void displayMainMenu() {
    cout << "Wybierz strukture danych:" << endl;
    cout << "1. Kopiec binarny (Heap)" << endl;
    cout << "2. Tablica dynamiczna (Array)" << endl;
    cout << "3. Przeprowadz badania" << endl;
    cout << "0. Wyjdz" << endl;
    cout << "Wybor: ";
}

void displayStructureMenu(const string& structureName) {
    cout << "\n" << structureName << "" << endl;
    cout << "1. Dodaj element (insert)" << endl;
    cout << "2. Usun i zwroc max (extract-max)" << endl;
    cout << "3. Podejrzyj max (find-max/peek)" << endl;
    cout << "4. Zmien priorytet elementu (modify-key)" << endl;
    cout << "5. Rozmiar kolejki (return-size)" << endl;
    cout << "6. Wyswietl zawartosc" << endl;
    cout << "7. Wygeneruj losowo" << endl;
    cout << "0. Wroc do menu glownego" << endl;
    cout << "Wybor: ";
}

// obsluga menu dla konkretnej implementacji
void handleStructureMenu(PriorityQueue* pq, const string& name) {
    int choice;
    do {
        displayStructureMenu(name);
        cin >> choice;

        switch (choice) {
        case 1: {
            int val, prio;
            cout << "Podaj wartosc: ";
            cin >> val;
            cout << "Podaj priorytet: ";
            cin >> prio;
            pq->insert(val, prio);
            cout << "Dodano element [" << val << ", prio=" << prio << "]" << endl;
            break;
        }
        case 2: {
            if (!pq->isEmpty()) {
                int max = pq->extractMax();
                cout << "Usunieto i zwrocono element o wartosci: " << max << endl;
            }
            else {
                cout << "Kolejka jest pusta!" << endl;
            }
            break;
        }
        case 3: {
            if (!pq->isEmpty()) {
                cout << "Element o najwyzszym priorytecie: " << pq->findMax() << endl;
            }
            else {
                cout << "Kolejka jest pusta!" << endl;
            }
            break;
        }
        case 4: {
            int val, newPrio;
            cout << "Podaj wartosc elementu do modyfikacji: ";
            cin >> val;
            cout << "Podaj nowy priorytet: ";
            cin >> newPrio;
            pq->modifyKey(val, newPrio);
            break;
        }
        case 5: {
            cout << "Rozmiar kolejki: " << pq->returnSize() << endl;
            break;
        }
        case 6: {
            pq->display();
            break;
        }
        case 7: {
            int n;
            cout << "Podaj rozmiar (min 1): ";
            cin >> n;
            if (n < 1) { cout << "Zly rozmiar." << endl; break; }
            fillRandom(pq, n);
            cout << "Wygenerowano " << n << " losowych elementow." << endl;
            break;
        }
        case 0:
            cout << "Powrot do menu glownego." << endl;
            break;
        default:
            cout << "Zly wybor, sprobuj ponownie." << endl;
        }
    } while (choice != 0);
}

// MAIN
int main() {
    rng.seed(time(nullptr));

    HeapPQ* heap = new HeapPQ();
    ArrayPQ* arr = new ArrayPQ();

    int choice;
    do {
        displayMainMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            handleStructureMenu(heap, "Kopiec binarny");
            break;
        case 2:
            handleStructureMenu(arr, "Tablica dynamiczna");
            break;
        case 3:
        {

            int seeds[] = {1, 6, 19, 42, 57, 78, 98, 123, 232, 345, 512, 999, 1111, 1555, 2024 };
            int numSeeds = 15;

            for (int s = 0; s < numSeeds; s++) {

                rng.seed(seeds[s]);
                cout << "SEED = " << seeds[s] << endl;
                runBenchmarks(seeds[s]);
            }

            rng.seed(time(nullptr)); // przywracamy losowosc po benchmarkach

            break;
        }
        case 0:
            cout << "Do widzenia!" << endl;
            break;
        default:
            cout << "Zly wybor!" << endl;
        }
    } while (choice != 0);

    delete heap;
    delete arr;

    return 0;
}
