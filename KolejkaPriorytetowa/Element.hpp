#pragma once

// struktura elementu kolejki - para (wartosc, priorytet)
struct Element {
    int value;
    int priority;

    Element() : value(0), priority(0) {}
    Element(int v, int p) : value(v), priority(p) {}
};
