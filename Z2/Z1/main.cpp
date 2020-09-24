#include <iostream>
#include <algorithm>
#include <iostream>
#include <typeinfo>

template <typename Tip>
class Lista
{
public:
    Lista() {}
    virtual ~Lista() {};
    virtual int brojElemenata() const = 0;
    virtual Tip& trenutni() = 0;
    virtual const Tip& trenutni() const = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const Tip& el) = 0;
    virtual void dodajIza(const Tip& el) = 0;
    virtual Tip& operator[](int index) = 0;
    virtual const Tip& operator[](int index) const = 0;
};


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
            throw std::domain_error("DvostrukaLista je prazna");
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




template<typename T>
class DvostraniRed {
    DvostrukaLista<T> dvostruka;
    public:
    void staviNaVrh(const T& el) {
        if(dvostruka.brojElemenata()) {
            dvostruka.kraj();
        }
        dvostruka.dodajIza(el);
        dvostruka.sljedeci();
    }
    
    T skiniSaVrha() {
        dvostruka.kraj();
        T el (dvostruka.trenutni());
        dvostruka.obrisi();
        return el;
    }
    void staviNaCelo(const T& el) {
        if(dvostruka.brojElemenata()) {
            dvostruka.pocetak();
        }
        dvostruka.dodajIspred(el);
        dvostruka.prethodni();
    }
    T skiniSaCela() {
        dvostruka.pocetak();
        T temp = dvostruka.trenutni();
        dvostruka.obrisi();
        return temp;
    }
    T& vrh() {
        dvostruka.kraj();
        return dvostruka.trenutni();
    }
    T& celo() {
        dvostruka.pocetak();
        return dvostruka.trenutni();
    }
    void brisi() {
        int temp = dvostruka.brojElemenata();
        for (int i = 0; i < temp; i++) {
            dvostruka.obrisi();
        }
    }
    int brojElemenata() const{
        return dvostruka.brojElemenata();
    }
};

void testStaviNaVrh() {
    DvostraniRed<int> red;
    red.staviNaVrh(5);
    red.staviNaVrh(6);
    red.staviNaVrh(7);
    if(red.skiniSaVrha() != 7) {
        std::cout << "testStaviNaVrh not ok" << std::endl;
        return;
    }
    if(red.skiniSaVrha() != 6) {
        std::cout << "testStaviNaVrh not ok" << std::endl;
        return;
    }
    if(red.skiniSaVrha() != 5) {
        std::cout << "testStaviNaVrh not ok" << std::endl;
        return;
    }
    std::cout << "testStaviNaVrh ok" << std::endl;
}

void testSkiniSaVrha() {
    DvostraniRed<int> red;
    try{
        red.skiniSaVrha();
        std::cout << "testSkiniSaVrha not ok" << std::endl;
        return;
    } catch(...) {
        
    }
    red.staviNaCelo(4);
    red.staviNaVrh(5);
    red.staviNaVrh(6);
    if(red.skiniSaVrha() != 6) {
        std::cout << "testStaviNaVrh not ok" << std::endl;
        return;
    }
    red.brisi();
    try{
        red.skiniSaVrha();
        std::cout << "testSkiniSaVrha not ok" << std::endl;
        return;
    } catch(...) {
    }
    std::cout << "testSkiniSaVrha ok" << std::endl;
}

void testStaviNaCelo() {
    DvostraniRed<int> red;
    red.staviNaCelo(1);
    red.staviNaCelo(2);
    red.staviNaCelo(3);
    if(red.skiniSaCela() != 3) {
        std::cout << "testStaviNaCelo not ok" << std::endl;
        return;
    }
    std::cout << "testStaviNaCelo ok" << std::endl;
}

void testSkiniSaCela() {
    DvostraniRed<int> red;
    try{
        red.skiniSaCela();
        std::cout << "testSkiniSaCela not ok" << std::endl;
        return;
    } catch(...) {
    }
    red.staviNaCelo(1);
    red.staviNaCelo(2);
    red.staviNaVrh(3);
    if(red.skiniSaCela() != 2) {
        std::cout << "testStaviNaCelo not ok" << std::endl;
        return;
    }
    if(red.skiniSaCela() != 1) {
        std::cout << "testStaviNaCelo not ok" << std::endl;
        return;
    }
    if(red.skiniSaCela() != 3) {
        std::cout << "testStaviNaCelo not ok" << std::endl;
        return;
    }
    try{
        red.skiniSaCela();
        std::cout << "testSkiniSaCela not ok" << std::endl;
        return;
    } catch(...) {
    }
    std::cout << "testSkiniSaCela ok" << std::endl;
}

void testVrh() {
    DvostraniRed<int> red;
    try{
        red.vrh();
        std::cout << "testSkiniSaCela not ok" << std::endl;
        return;
    } catch(...) {
    }
    red.staviNaVrh(1);
    if(red.vrh() != 1) {
        std::cout << "testCelo not ok" << std::endl;
        return;
    }
    red.vrh() = 72;
    int temp = red.skiniSaVrha();
    if(temp != 72) {
        std::cout << "testCelo not ok" << std::endl;
        return;
    }
    std::cout << "testCelo ok" << std::endl;
}

void testCelo() {
    DvostraniRed<int> red;
    try{
        red.celo();
        std::cout << "testSkiniSaCela not ok" << std::endl;
        return;
    } catch(...) {
    }
    red.staviNaCelo(1);
    if(red.celo() != 1) {
        std::cout << "testCelo not ok" << std::endl;
        return;
    }
    red.celo() = 72;
    int temp = red.skiniSaCela();
    if(temp != 72) {
        std::cout << "testCelo not ok" << std::endl;
        return;
    }
    std::cout << "testCelo ok" << std::endl;
}

void testObrisi() {
    DvostraniRed<int> red;
    red.staviNaCelo(3);
    red.staviNaCelo(6);
    red.staviNaCelo(9);
    if(red.brojElemenata() != 3) {
        std::cout << "testObrisi not ok" << std::endl;
        return;
    }
    red.brisi();
    if(red.brojElemenata() != 0) {
        std::cout << "testObrisi not ok" << std::endl;
        return;
    }
    std::cout << "testObrisi ok" << std::endl;
}

void testBrojElemenata() {
    DvostraniRed<int> red;
    if(red.brojElemenata() != 0) {
        std::cout << "testBrojElemenata not ok" << std::endl;
        return;
    }
    for (int i = 0; i < 100; i++) {
        red.staviNaVrh(i);
    }
    if(red.brojElemenata() != 100) {
        std::cout << "testBrojElemenata not ok" << std::endl;
        return;
    }
    red.brisi();
    if(red.brojElemenata() != 0) {
        std::cout << "testBrojElemenata not ok" << std::endl;
        return;
    }
    std::cout << "testBrojElemenata ok" << std::endl;
}


int main() {
    testStaviNaVrh();
    testSkiniSaVrha();
    testStaviNaCelo();
    testSkiniSaCela();
    testVrh();
    testCelo();
    testObrisi();
    testBrojElemenata();
    return 0;
}
