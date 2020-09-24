#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

template<typename Tip>
void bubble_sort2(Tip* niz, int vel) {
    //int c = 0;
    for (int i = vel - 1; i >= 1; i--) {
        for (int j = 1; j <= i; j++) {
            if (niz[j-1] > niz[j]) {
                //c++;
                Tip priv = niz[j-1];
                niz[j-1] = niz[j];
                niz[j] = priv;
            }
        }
    }
    //std:: cout << "Kompleksnost 1: " << c << std::endl;
}

template<typename Tip>
void bubble_sort(Tip* niz, int vel) {
    //int c=0;
    for (int i = 0; i < vel; i++) {
        for (int j = 1; j < vel-i; j++) {
            if (niz[j-1] > niz[j]) {
                //c++;
                Tip priv = niz[j-1];
                niz[j-1] = niz[j];
                niz[j] = priv;
            }
        }
    }
    //std:: cout << "Kompleksnost 2: " << c << std::endl;
}

template<typename Tip>
void selection_sort(Tip* niz, int vel) {
    for (int i = 0; i < vel - 1; i++) {
        Tip min = niz[i];
        int indexMin = i;
        for (int j = i + 1; j <= vel - 1; j++) {
            if (niz[j] < min) {
                min = niz[j];
                indexMin = j;
            }
        }
        niz[indexMin] = niz[i];
        niz[i] = min;
    }
}




template<typename Tip>
void quickySwappy(Tip* niz, int i, int j) {
     Tip temp = niz[i];
     niz[i] = niz[j];
     niz[j] = temp;
}


template<typename Tip>
int partition(Tip* niz, int l, int r) {
    Tip pivot = niz[l];
    int p = l + 1;
    while(p <= r) {
        if (niz[p] <= pivot) {
            p++;
        }
        else {
            break;
        }
    }
    
    for (int i = p+1; i <= r; i++) {
        if(niz[i] < pivot) {
            quickySwappy(niz, p, i);
            p++;
        }
    }
    quickySwappy(niz, l, p-1);
    return p-1;
}


template<typename Tip>
void quicky_sort(Tip* niz, int l, int r) {
    if(l < r) {
        int j = partition(niz, l, r);
        quicky_sort(niz, l, j-1);
        quicky_sort(niz, j+1, r);
    }
}



template<typename Tip>
void quick_sort(Tip* niz, int vel) {
    quicky_sort(niz, 0, vel-1);
}





template<typename Tip>
void merge(Tip* niz, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    //int leftArr[m - l + 1], rightArr[r - m];
    Tip* leftArr = new Tip[n1];
    Tip* rightArr = new Tip[n2];
    for (int i = 0; i < n1; i++) {
        leftArr[i] = niz[l+i];
    }
    for (int i = 0; i < n2; i++) {
        rightArr[i] = niz[m + 1 + i];
    }
    
    //spajanje
    int i = 0, j = 0, k = l;
    for (; i < n1 && j < n2; k++) {
        if(leftArr[i] < rightArr[j]) {
            niz[k] = leftArr[i];
            i++;
        } else {
            niz[k] = rightArr[j];
            j++;
        }
    }
    //kopiram ostatak ako ga ima
    while(i < n1) {
        niz[k] = leftArr[i];
        k++;
        i++;
    }
    while (j < n2) {
        niz[k] = rightArr[j];
        k++;
        j++;
    }
    delete[] leftArr;
    delete[] rightArr;
}


template<typename Tip>
void divide(Tip* niz, int l, int r) {
    if(l < r) {
        int m = (l + r) / 2;
        divide(niz, l, m);
        divide(niz, m+1, r);
        merge(niz, l, m, r);
    }
}



template<typename Tip>
void merge_sort(Tip* niz, int vel) {
    divide(niz, 0, vel-1);
}


void ucitaj(std::string filename, int*& niz, int &vel) {
    vel = 0;
    std::ifstream input(filename);
    int temp;
    while(input >> temp) {
        vel = vel + 1;
    }
    input.clear();
    input.seekg(0, std::ios::beg);
    niz = new int[vel];
    for (int i = 0; i < vel; i++) {
        input >> temp;
        if(!input) {
            break;
        }
        niz[i] = temp;
    }
    input.close();
}

void generisi(const std::string &filename, int vel) {
    std::srand(std::time(NULL));
    std::ofstream output;
    output.open(filename);
    for (int i = 0; i < vel; i++) {
        output << std::rand() % 1000 << " ";
    }
    output.close();
}

void provjeri() {
    std::cout << "Izaberite:\n 1.generisati brojeve u file\n 2.ucitati postojeci(bilo koji drugi broj):  ";
    int izbor;
    do {
        std::cin.clear();
        std::cin >> izbor;
        if(!std::cin) {
            std::cout << "Pogresan unos - unesite ponvo\n";
        }
    } while (!std::cin);
    
    std::cout << "Ime fajla: ";
    std::string input;
    std::cin >> input;
    
    int vel;
    int* niz(0);
    if(izbor == 1) {
         std::cout << "Broj zapisa: ";
        std::cin >> vel;
        generisi(input, vel);
    } 
    ucitaj(input, niz, vel);
    
    std::cout << "Izaberite algoritam sortiranja:" << std::endl;
    std::cout << "1. Bubble sort" << std::endl;
    std::cout << "2. Selection sort" << std::endl;
    std::cout << "3. Quick sort" << std::endl;
    std::cout << "4. Merge sort: "; 
    std::cin >> izbor;
    clock_t vrijeme1 = clock();
    switch(izbor) {
        case 1: {
            bubble_sort(niz, vel);
            break;    
        }
        case 2: {
            selection_sort(niz, vel);
            break;    
        }
        case 3: {
            quick_sort(niz, vel);
            break;    
        }
        case 4: {
            merge_sort(niz, vel);
            break;    
        }
        default: {
            std::cout << "Pogresan izbor";
            return;
        }
    }
    clock_t vrijeme2 = clock();
    int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Vrijeme izvrsavanja: " << ukvrijeme << "[ms]\n";
    
    for (int i = 0; i < vel-1; i++) {
        if(niz[i] > niz[i+1]) {
            std::cout << "Niz je pogresno sortiran";
            return;
        }
    }
    std::cout << "Niz je dobro sortiran" << std::endl;
    std::ofstream output;
    output.open("sortirano.txt");
    for (int i = 0; i < vel; i++) {
        output << niz[i] << " ";
    }
    output.close();
    std::cout << "Sortirani niz je upisan u datoteku sortirano.txt";
    delete[] niz;
}

void generisiZaBubble(std::string filename, int vel) {
    
}

template<typename Tip>
void insertionSort(Tip* niz, int vel) {
    int j;
    for (int i = 0; i < vel - 1; i++) {
        Tip temp = niz[i];
        j = i - 1;
        while(j >= 0 && niz[j] > temp) {
            niz[j+1] = niz[j];
            j--;
        }
        niz[j+1] = temp;
    }
}

template<typename Tip>
void shellSort(Tip* niz, int vel, int* h, int hVel) {
    for (int i = 0; i < hVel; i++) {
        for(int j = h[i]; j < vel; j++) {
            Tip temp = niz[j];
            int k = j - h[i];
            while (k >= 0 && niz[k] > temp) {
                niz[k + h[i]] = niz[k];
                k -= h[i];
            }
            niz[k + h[i]] = temp;
        }
    }
}


template<typename Tip>
void countingSort(Tip* a, int vel, int* bNiz, int k) {
    
}

int main() {
    provjeri();
    return 0;
}
