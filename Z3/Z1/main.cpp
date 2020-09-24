#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void countSort(vector<int> &a, int exponent) {
    std::vector<int> temp(a.size());
    std::vector<int> decimale(10, 0);
    auto it = a.begin();
    while(it != a.end()) {
        int temp2 = *it / exponent;
        temp2 %= 10;
        (decimale.at(temp2))++;
        it++;
    }
    
    it = decimale.begin();
    auto it2 = it;
    it++;
    auto end = decimale.end();
    end--;
    while(it2 != end) {
        *it += *it2;
        it2++;
        it++;
    }
    
    for (int i = a.size() - 1; i >= 0; i--) {
        temp.at(decimale.at( (a.at(i) / exponent) % 10 ) - 1) = a.at(i); 
        decimale.at((a.at(i) / exponent ) % 10 )--; 
    } 
    a = temp;
}

void radixSort(vector<int> &a) {
    if(!a.size()) return;
    int max = a.at(0);
    auto it = a.begin();
    while(it != a.end()) {
        if(*it > max) max = *it;
        it++;
    }
    int exponent = 1;
    while((max / exponent) > 0) {
        countSort(a, exponent);
        exponent *= 10;
    }
}

bool jeLiList(int i, int velicina) {
    return (i >= velicina/2) && (i < velicina);
}

int Roditelj(int i) { // Pozicija roditelja
    return (i-1)/2;
} 

void popraviDole(vector<int> &a, int i, int vel = -5) {
    if(vel == -5) {
        vel = a.size();
    }
    while(!jeLiList(i, vel)) {
        int ld = (2*i)+1;
        int dd = (2*i)+2;
        if(dd < vel && a.at(dd) > a.at(ld)) {
            ld = dd;
        }
        if(a.at(i) > a.at(ld)) return;
        int temp = a.at(i);
        a.at(i) = a.at(ld);
        a.at(ld) = temp;
        i = ld;
    }
}

void popraviGore(vector<int> &a, int i) {
    while(i != 0 && a.at(i) > a.at(Roditelj(i))) {
        std::swap(a.at(i), a.at(Roditelj(i)));
        i = Roditelj(i);
    }
}


void stvoriGomilu(vector<int> &a, int vel = -5) {
    if(vel == -5) {
        vel = a.size();
    }
    for (int i = vel/2; i >= 0; i--) {
        if(vel) popraviDole(a, i, vel);
    }
}

void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina) {
    a.push_back(umetnuti);
    velicina++;
    popraviGore(a, velicina-1);
}

int izbaciPrvi(vector<int> &a, int &velicina) {
    if(!a.size()) {
        throw;
    }
    velicina--;
    std::swap(a.at(0), a.at(velicina));
    if(velicina) {
        popraviDole(a, 0, velicina);
    }
    return a.at(velicina);
}

void gomilaSort(vector<int> &a) {
    stvoriGomilu(a);
    std::vector<int> temp(a.size());
    int vel = a.size();
    int actual = vel;
    for (int i = 0; i < vel; i++) {
        /*for(int j(0);j<actual;j++){
            cout<<a[j]<<" ";
        }*/
        int t = izbaciPrvi(a, actual);
        temp.at(i) = t;
        //std::cout << "izb: " << t << std::endl;
        stvoriGomilu(a, actual);
    }
    a.clear();
    for (int i = temp.size()-1; i >= 0; i--) {
        a.push_back(temp.at(i));
    }
}


int main() {
    return 0;
}
