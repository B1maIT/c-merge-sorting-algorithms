//============================================================================
// Zadanie 10_01 
// 
//  Sortowanie zew Mds
//
//  WCY23KY3S1   Dmytro Stefko
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Funkcja laczenia dwoch podtablic w jedna
void merge(int* data, int left, int middle, int right) {
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // Alokacja pamieci dla podtablic
    int* leftPart = (int*)malloc(n1 * sizeof(int));
    int* rightPart = (int*)malloc(n2 * sizeof(int));

    // Kopiowanie danych do podtablic
    for (int i = 0; i < n1; ++i) {
        leftPart[i] = data[left + i];
    }
    for (int j = 0; j < n2; ++j) {
        rightPart[j] = data[middle + 1 + j];
    }

    // Scalanie podtablic
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftPart[i] <= rightPart[j]) {
            data[k] = leftPart[i];
            i++;
        } else {
            data[k] = rightPart[j];
            j++;
        }
        k++;
    }

    // Kopiowanie pozostalych elementow
    while (i < n1) {
        data[k] = leftPart[i];
        i++;
        k++;
    }
    while (j < n2) {
        data[k] = rightPart[j];
        j++;
        k++;
    }

    // Zwolnienie pamieci
    free(leftPart);
    free(rightPart);
}

// Funkcja sortowania przez scalanie
void mergeSort(int* data, int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        mergeSort(data, left, middle);        
        mergeSort(data, middle + 1, right);  
        merge(data, left, middle, right);    
    }
}

// Funkcja do wyswietlania tablicy
void wypisz(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%6d", arr[i]);
        if ((i + 1) % 10 == 0) printf("\n"); // Nowa linia co 10 elementow
    }
    if (n % 10 != 0) printf("\n");
}

// Funkcja do odczytu danych z pliku
int* odczytaj(const char* filename, int* rozmiar) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Blad otwarcia pliku");
        return NULL;
    }
    int* data = NULL;
    int liczba, licznik = 0;

    // Odczyt danych z pliku do tablicy
    while (fscanf(file, "%d", &liczba) == 1) {
        int* newData = (int*)realloc(data, (licznik + 1) * sizeof(int));
        if (!newData) {
            perror("Blad relokacji pamieci");
            free(data);
            fclose(file);
            return NULL;
        }
        data = newData;
        data[licznik++] = liczba;
    }
    fclose(file);
    *rozmiar = licznik;
    return data;
}

// Funkcja do zapisu danych do pliku
void zapisz(const char* filename, int* data, int rozmiar) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Blad otwarcia pliku");
        return;
    }
    // Zapis danych do pliku w formie kolumn
    for (int i = 0; i < rozmiar; i++) {
        fprintf(file, "%6d", data[i]);
        if ((i + 1) % 10 == 0) fprintf(file, "\n");
    }
    if (rozmiar % 10 != 0) fprintf(file, "\n");
    fclose(file);
}

// Glowna funkcja programu
int main() {
    int* data = NULL;
    int rozmiar = 0;

    // Wyswietlenie menu
    printf("Wybierz opcje:\n");
    printf("1. Generacja losowych wartosci\n");
    printf("2. Odczyt z pliku (we50.txt)\n");
    printf("3. Wpisanie z klawiatury\n");
    printf("Twoj wybor: ");

    int wybor;
    scanf("%d", &wybor);

    switch (wybor) {
        case 1: { // Generacja losowych liczb
            printf("Wpisz ilosc liczb do generacji: ");
            int n;
            scanf("%d", &n);
            int min = 1;
            int max = 999;
            data = (int*)malloc(n * sizeof(int));
            if (!data) {
                perror("Blad alokacji pamieci");
                return 1;
            }
            srand(time(0));
            for (int i = 0; i < n; i++) {
                data[i] = min + rand() % (max - min + 1);
            }
            rozmiar = n;
            break;
        }
        case 2: { // Odczyt danych z pliku
            data = odczytaj("we50.txt", &rozmiar);
            if (!data) {
                return 1;
            }
            break;
        }
        case 3: { // Wpisanie danych z klawiatury
            printf("Wpisz liczby do sortowania (aby skonczyc wpisz nie-liczbe):\n");
            int liczba;
            while (scanf("%d", &liczba) == 1) {
                int* newData = (int*)realloc(data, (rozmiar + 1) * sizeof(int));
                if (!newData) {
                    perror("Blad relokacji pamieci");
                    free(data);
                    return 1;
                }
                data = newData;
                data[rozmiar++] = liczba;
            }
            while (getchar() != '\n'); // Czyszczenie bufora
            break;
        }
        default: { // Zly wybor
            fprintf(stderr, "Zly wybor.\n");
            return 1;
        }
    }

    // Wyswietlenie danych przed sortowaniem
    printf("Dane do sortowania:\n");
    wypisz(data, rozmiar);

    // Sortowanie danych
    mergeSort(data, 0, rozmiar - 1);

    // Wyswietlenie danych po sortowaniu
    printf("Dane po sortowaniu:\n");
    wypisz(data, rozmiar);

    // Zapis do pliku
    zapisz("wy.txt", data, rozmiar);

    // Zwolnienie pamieci
    free(data);
    return 0;
}
