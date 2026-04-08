//============================================================================
// Zadanie 10_02
// 
//  Sortowanie zew Mds
//
//  WCY23KY3S1   Dmytro Stefko
//============================================================================

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

// Funkcja dzieli dane z pliku wejsciowego na dwie serie
void podzielNaSerie(const string &plikWe, const string &plikPom1, const string &plikPom2) {
    ifstream wejscie(plikWe.c_str());
    ofstream pomoc1(plikPom1.c_str());
    ofstream pomoc2(plikPom2.c_str());

    if (!wejscie || !pomoc1 || !pomoc2) {
        cerr << "Blad otwarcia plikow!" << endl;
        return;
    }

    int poprzednia, aktualna;
    bool doPom1 = true; // Flaga okreslajaca, do ktorego pliku zapisac

    wejscie >> poprzednia;
    while (wejscie >> aktualna) {
        if (doPom1)
            pomoc1 << poprzednia << endl;
        else
            pomoc2 << poprzednia << endl;

        if (aktualna < poprzednia) // Sprawdzenie zmiany serii
            doPom1 = !doPom1;

        poprzednia = aktualna;
    }

    // Zapis ostatniego elementu
    if (doPom1)
        pomoc1 << poprzednia << endl;
    else
        pomoc2 << poprzednia << endl;

    wejscie.close();
    pomoc1.close();
    pomoc2.close();
}

// Funkcja scala serie z dwoch plikow pomocniczych do pliku wyjsciowego
void scalSerie(const string &plikPom1, const string &plikPom2, const string &plikWe) {
    ifstream pomoc1(plikPom1.c_str());
    ifstream pomoc2(plikPom2.c_str());
    ofstream wyjscie(plikWe.c_str());

    if (!pomoc1 || !pomoc2 || !wyjscie) {
        cerr << "Blad otwarcia plikow!" << endl;
        return;
    }

    int val1, val2;
    bool koniec1 = !(pomoc1 >> val1); // Czy plik pomoc1 jest pusty
    bool koniec2 = !(pomoc2 >> val2); // Czy plik pomoc2 jest pusty

    while (!koniec1 || !koniec2) {
        if (!koniec1 && (koniec2 || val1 <= val2)) {
            wyjscie << val1 << endl;
            if (!(pomoc1 >> val1))
                koniec1 = true;
        } else {
            wyjscie << val2 << endl;
            if (!(pomoc2 >> val2))
                koniec2 = true;
        }
    }

    pomoc1.close();
    pomoc2.close();
    wyjscie.close();
}

// Funkcja realizujaca sortowanie naturalne przez scalanie
void naturalMergeSort(const string &plikWe) {
    const string plikPom1 = "pom1.txt";
    const string plikPom2 = "pom2.txt";

    while (true) {
        podzielNaSerie(plikWe, plikPom1, plikPom2);

        ifstream pomoc1(plikPom1.c_str());
        ifstream pomoc2(plikPom2.c_str());

        // Sprawdzenie czy ktorys z plikow pomocniczych jest pusty
        bool pusty1 = pomoc1.peek() == EOF;
        bool pusty2 = pomoc2.peek() == EOF;

        pomoc1.close();
        pomoc2.close();

        if (pusty1 || pusty2) {
            break; // Koniec sortowania
        }

        scalSerie(plikPom1, plikPom2, plikWe);
    }
}

// Funkcja wyswietla dane z pliku
void wyswietlZPliku(const string &plik) {
    ifstream wejscie(plik.c_str());

    if (!wejscie) {
        cerr << "Blad otwarcia pliku!" << endl;
        return;
    }

    int liczba;
    int licznik = 0;

    while (wejscie >> liczba) {
        cout << setw(5) << liczba;
        licznik++;

        if (licznik == 10) { // Nowa linia co 10 elementow
            cout << endl;
            licznik = 0;
        }
    }

    if (licznik != 0) {
        cout << endl;
    }

    wejscie.close();
}

// Glowna funkcja programu
int main() {
    cout << "Dane sa wczytywane z pliku we50.txt" << endl;

    const string plikWe = "we50.txt";

    cout << "Dane przed sortowaniem:" << endl;
    wyswietlZPliku(plikWe);

    naturalMergeSort(plikWe);

    cout << "Dane po sortowaniu:" << endl;
    wyswietlZPliku(plikWe);

    return 0;
}
