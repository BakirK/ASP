#include <iostream>
#include <algorithm>
#include <iostream>
#include <typeinfo>
#include "stek.cpp"

using namespace std;

template<typename Tip>
class DvostrukaLista;

template<typename Tip>
class DvostrukaLista: public Lista<Tip> {
    struct Cvor {
        Tip el;
        Cvor *iduci, *prethodni;
    };
    int brElem;
    mutable int lastAccessedIndex = -1;
    mutable Cvor *lastAccessed = nullptr;
    Cvor *prvi, *zadnji, *current;
    void provjeriIndex(int index) const {
        if(index < 0 || index > brElem - 1) {
            throw std::domain_error("Index van opsega");
        }
    }
    void provjeriJeLiPrazna() const {
        if(!prvi) {
            throw std::domain_error("JednostrukaLista je prazna");
        }
    }
    public:
    DvostrukaLista() {
        lastAccessed = prvi = zadnji = current = nullptr;
        brElem = 0;
    }
    DvostrukaLista(const DvostrukaLista<Tip>& l) {
        Cvor *temp(l.prvi), *temp2 = nullptr;
        while (temp != nullptr) {
            Cvor *novi = new Cvor;
            novi->el = temp->el;
            if(temp2 == nullptr) {
                novi->prethodni = nullptr;
                prvi = novi;
            } else {
                temp2->iduci = novi;
                novi->prethodni = temp2;
            }
            temp2 = novi;
            if(l.current == temp) {
                current = novi;
            }
            temp = temp->iduci;
        }
        temp2->iduci = nullptr;
        zadnji = temp2;
        brElem = l.brElem;
    }
    DvostrukaLista(DvostrukaLista<Tip> &&l) {
        this->prvi = l.prvi;
        this->current = l.current;
        this->zadnji = l.zadnji;
        this->brElem = l.brElem;
        l.prvi = l.zadnji = l.current = nullptr;
        l.brElem = 0;
    }
    ~DvostrukaLista() {
        Cvor *temp;
        while(prvi != nullptr) {
            temp = prvi;
            prvi  = prvi->iduci;
            delete temp;
        }
        prvi = zadnji = current = nullptr; //ne treba?
    }
    DvostrukaLista<Tip>& operator =(const DvostrukaLista<Tip>& l) {
        if(this != &l) {
            //destruktor
            Cvor *temp;
            while(prvi != nullptr) {
                temp = prvi;
                prvi  = prvi->iduci;
                delete temp;
            }
            //konstruktor kopije
            temp = l.prvi;
            Cvor *temp2 = nullptr;
            while (temp != nullptr) {
                Cvor *novi = new Cvor;
                novi->el = temp->el;
                if(temp2 == nullptr) {
                    novi->prethodni = nullptr;
                    prvi = novi;
                } else {
                    temp2->iduci = novi;
                    novi->prethodni = temp2;
                }
                temp2 = novi;
                if(l.current == temp) {
                    current = novi;
                }
                temp = temp->iduci;
            }
            temp2->iduci = nullptr;
            zadnji = temp2;
            brElem = l.brElem;
        }
        return *this;
    }
    
    DvostrukaLista<Tip>& operator =(DvostrukaLista<Tip>&& l) {
        if(this != &l) {
            std::swap(this->prvi, l.prvi);
            std::swap(this->zadnji, l.zadnji);
            std::swap(this->current, l.current);
            std::swap(this->brElem, l.brElem);
        }
        return *this;
    }
    int brojElemenata() const {
        return brElem;
    }
    Tip& trenutni() {
        provjeriJeLiPrazna();
        return current->el;
    }
    const Tip& trenutni() const {
        provjeriJeLiPrazna();
        return current->el;
    }
    bool prethodni() {
        provjeriJeLiPrazna();
        if(prvi == current) {
            return false;
        }
        current = current->prethodni;
        return true;
    }
    bool sljedeci() {
        provjeriJeLiPrazna();
        if(current->iduci == nullptr) {
            return false;
        }
        current = current->iduci;
        return true;
    }
    void pocetak() {
        provjeriJeLiPrazna();
        current = prvi;
    }
    void kraj() {
        provjeriJeLiPrazna();
        current = zadnji;
    }
    void obrisi() {
        provjeriJeLiPrazna();
        if(current == prvi) {
            prvi = prvi->iduci;
            delete current;
            current = prvi;
            brElem--;
            if(current) {
                current->prethodni = nullptr;    
            }
            if(!brElem) {
                zadnji = prvi;
            }
        } else {
            Cvor *temp = &(*(current->prethodni));
            temp->iduci = current->iduci;
            delete current;
            brElem--;
            if(temp->iduci == nullptr) {
                zadnji = current = temp;
            } else {
                current = temp->iduci;
                current->prethodni = temp;
            }
        }
    }
    void dodajIspred(const Tip& el) {
        Cvor* temp = new Cvor;
        temp->el = el;
        if(prvi == nullptr) {
            temp->prethodni = nullptr;
            temp->iduci = nullptr;
            prvi = current = zadnji = temp;
            brElem++;
            return;
        } else if(prvi == current) {
            prvi->prethodni = temp;
            temp->iduci = prvi;
            prvi = temp;
            temp->prethodni = nullptr;
            brElem++;
            return;
        } else {
            Cvor *pIspred = &(*(current->prethodni));
            pIspred->iduci = temp;
            temp->prethodni = pIspred;
            temp->iduci = current;
            current->prethodni = temp;
            brElem++;
            return;
        }
    }
    void dodajIza(const Tip& el) {
        Cvor* temp = new Cvor;
        temp->el = el;
        if(prvi == nullptr) {
            temp->prethodni = nullptr;
            temp->iduci = nullptr;
            prvi = current = zadnji = temp;
            brElem++;
            return;
        } else {
            if(current->iduci != nullptr) {
                Cvor *pIza = &(*(current->iduci));    
                pIza->prethodni = temp;
            }
            temp->iduci = current-> iduci;
            temp->prethodni = current;
            current->iduci = temp;
            if(zadnji == current) {
                zadnji = temp;
            }
            brElem++;
        }
    }
    Tip& operator[] (int index) {
        provjeriIndex(index);
        if(lastAccessedIndex == -1) {
            lastAccessedIndex = index;
            
        } else if(index == lastAccessedIndex) {
            return lastAccessed->el;
        }
        else if(index == lastAccessedIndex + 1) {
            if(lastAccessed->iduci) {
                lastAccessed = lastAccessed->iduci;
                lastAccessedIndex++;
                return lastAccessed->el;
            }
        } else if(index == lastAccessedIndex - 1) {
            if(lastAccessed->prethodni) {
                lastAccessed = lastAccessed->prethodni;
                lastAccessedIndex--;
                return lastAccessed->el;
            }
        }
        Cvor* temp(prvi);
        for (int i = 0; i < index; i++) {
            temp = temp->iduci;
        }
        lastAccessed = temp;
        lastAccessedIndex = index;
        return temp->el;
    }
    const Tip& operator[] (int index) const {
        provjeriIndex(index);
        if(lastAccessedIndex == -1) {
            lastAccessedIndex = index;
            
        } else if(index == lastAccessedIndex) {
            return lastAccessed->el;
        }
        else if(index == lastAccessedIndex + 1) {
            if(lastAccessed->iduci) {
                lastAccessed = lastAccessed->iduci;
                lastAccessedIndex++;
                return lastAccessed->el;
            }
        } else if(index == lastAccessedIndex - 1) {
            if(lastAccessed->prethodni) {
                lastAccessed = lastAccessed->prethodni;
                lastAccessedIndex--;
                return lastAccessed->el;
            }
        }
        Cvor* temp(prvi);
        for (int i = 0; i < index; i++) {
            temp = temp->iduci;
        }
        lastAccessed = temp;
        lastAccessedIndex = index;
        return temp->el;
    }
};


template <typename Tip>
class Red {
    DvostrukaLista<Tip> dvostruka;
public: 
    Red() {}
    void brisi() {
        int temp = dvostruka.brojElemenata();
        for (int i = 0; i < temp; i++) {
            dvostruka.obrisi();
        }
    }
    void stavi(const Tip& el) {
        if(dvostruka.brojElemenata()) {
            dvostruka.kraj();
        }
        dvostruka.dodajIza(el);
        
    }
    Tip skini() {
        dvostruka.pocetak();
        Tip temp = dvostruka.trenutni();
        dvostruka.obrisi();
        return temp;
    }
    Tip& celo() {
        dvostruka.pocetak();
        return dvostruka.trenutni();
    }
    int brojElemenata() {
        return dvostruka.brojElemenata();
    }
};

void testBrisi() {
    Red<int> r;
    r.stavi(5);
    r.stavi(5);
    r.stavi(5);
    if(r.brojElemenata() != 3) {
        std::cout << "problem testBrisi";
        return;
    }
    r.brisi();
    if(r.brojElemenata()) {
        std::cout << "problem testBrisi";
        return;
    }
    try {
        r.brisi();
    } catch(...) {
        std::cout << "problem testBrisi";
        return;
    }
    std::cout << "testBrisi ok" << std::endl;
}

void testStavi() {
    const int temp = 33;
    Red<int> r;
    r.stavi(temp);
    r.stavi(34);
    if(r.brojElemenata() != 2) {
        std::cout << "problem testStavi";
        return;
    }
    if(r.celo() != 33) {
        std::cout << "problem testStavi";
        return;
    }
    std::cout << "testStavi ok" << std::endl;
}

void testSkini() {
    Red<int> r;
    r.stavi(1);
    if(r.celo() != 1) {
        std::cout << "problem testSkini" << std::endl;
        return;
    }
    int temp = r.skini();
    if(temp != 1) {
        std::cout << "problem testSkini" << std::endl;
        return;
    }
    try {
        r.skini();
        std::cout << "problem testSkini" << std::endl;
        return;
    } catch(...) {
        std::cout << "testSkini ok" << std::endl;
    }
}

void testCelo() {
    Red<int> r;
     try {
        r.celo();
        std::cout << "problem testCelo1" << std::endl;
        return;
    } catch(...) {
        
    }
    r.stavi(33);
    r.stavi(34);
    int& temp = r.celo();
    if(temp != 33)  {
        std::cout << "problem testCelo2" << std::endl;
        return;
    }
    temp = 0;
    if(r.celo() != 0)  {
        std::cout << "problem testCelo3" << std::endl;
        return;
    }
    std::cout << "testCelo ok" << std::endl;
}

void testBrElem() {
    Red<int> r;
    if(r.brojElemenata()) {
        std::cout << "problem testBrElem";
        return;
    }
    r.stavi(1);
    r.stavi(-1);
    if(r.brojElemenata() != 2) {
        std::cout << "problem testBrElem";
        return;
    }
    std::cout << "testBrElem ok" << std::endl;
}
template <typename Tip>
void izbaciSve(Red<Tip>& r) {
    int brElem = r.brojElemenata();
    Tip suma (0), temp (0);
    for (int i = 0; i < brElem; i++) {
        temp = r.skini();
        suma += temp;
        r.stavi(temp);
    }
    Tip avg(suma / brElem);
    for (int i = 0; i < brElem; i++) {
        temp = r.skini();
        if(temp < avg) {
            r.stavi(temp);
        } 
    }
}

void testIzbaciSve() {
    Red<int> r;
    for (int i = 1; i <= 10; i++) {
        r.stavi(i);
    }
    std::cout << "Elementi prije izbacivanja: "; 
    for (int i = 0; i < r.brojElemenata(); i++) {
        int temp = r.skini();
        std::cout << temp << " ";
        r.stavi(temp);
    }
    std::cout << std::endl;
    izbaciSve(r);
    std::cout << "Elementi poslije izbacivanja: "; 
    for (int i = 0; i < r.brojElemenata(); i++) {
        int temp = r.skini();
        std::cout << temp << " ";
        r.stavi(temp);
    }
}

template <typename Tip>
void drugiZad(Stek<Tip>& s, Red<Tip>& r, int pocetnaDuzinaReda) {
    int duzinaStek (s.brojElemenata());
    for (int i = 0; i < duzinaStek; i++) {
        r.stavi(s.skini());
    }
    for (int i = 0; i < pocetnaDuzinaReda; i++) {
        s.stavi(r.skini());
    }
    for (int i = 0; i < pocetnaDuzinaReda; i++) {
        r.stavi(s.skini());
    }
    int temp = r.brojElemenata();
    for (int i = 0; i < temp; i++) {
        s.stavi(r.skini());
    }
}

void testDrugi() {
    Stek<int> s;
    for (int i = 5; i < 9; i++) {
        s.stavi(i);
    }
    Red<int> r;
    for (int i = 1; i < 5; i++) {
        r.stavi(i);
    }
    drugiZad(s, r, r.brojElemenata());
    int temp = s.brojElemenata();
    for (int i = 0; i < temp; i++) {
        std::cout << s.skini() << " ";
    }
}





int main() {
    /*testBrisi();
    testStavi();
    testSkini();
    testCelo();
    testBrElem();*/
    //testIzbaciSve();
    //testDrugi();
    
    
    
    
    
    return 0;
}
