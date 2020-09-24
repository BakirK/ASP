#include <iostream>
#include <algorithm>
#include <iostream>
#include <typeinfo>
#include <ctime>

using namespace std;

template<typename Tip>
class Iterator;

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
class NizLista: public Lista<Tip>
{
    friend class Iterator<Tip>;
    int brElem, kap, current;
    Tip **p;
    void provjeriVelicinu()
    {
        if(brElem == kap) {
            kap *= 2;
            Tip **p2 = new Tip*[kap];
            for (int i = 0; i < brElem; i++) {
                p2[i] = p[i];
            }
            delete[] p;
            p = p2;
        }
    }
    void provjeriIndex(int index) const
    {
        if(index < 0 || index > brElem - 1) {
            throw std::domain_error("Index van opsega");
        }
    }

    void provjeriJeLiPrazna() const
    {
        if(!brElem) {
            throw std::domain_error("Lista je prazna");
        }
    }

public:
    NizLista(int velicina = 1000)
    {
        this->kap = velicina;
        this->brElem = 0;
        this->current = 0;
        p = new Tip*[kap] {};
    }

    //kopirajuci konstruktor
    NizLista(const NizLista &nizLista)
    {
        this->kap = nizLista.kap;
        this->brElem = nizLista.brElem;
        this->current = nizLista.current;
        p = new Tip*[this->kap];
        for (int i = 0; i < this->brElem; i++) {
            p[i] = new Tip(*(nizLista.p[i]));
        }
    }
    //pomjerajuci konstruktor
    NizLista(NizLista &&nizLista)
    {
        this->current = nizLista.current;
        this->kap = nizLista.kap;
        this->brElem = nizLista.brElem;
        this->p = nizLista.p;
        nizLista.p = nullptr;
    }

    NizLista<Tip> &operator =(const NizLista<Tip>& nizLista)
    {
        if(this != &nizLista) {
            for (int i = 0; i < brElem; i++) {
                delete p[i];
            }
            delete[] p;
            this->kap = nizLista.kap;
            this->brElem = nizLista.brElem;
            this->current = nizLista.current;
            p = new Tip*[this->kap];
            for (int i = 0; i < this->brElem; i++) {
                p[i] = new Tip(*(nizLista.p[i]));
            }
        }
        return *this;
    }
    NizLista<Tip> &operator =(NizLista<Tip>&& nizLista)
    {
        if(this != &nizLista) {
            std::swap(p, nizLista.p);
            std::swap(brElem, nizLista.brElem);
            std::swap(kap, nizLista.kap);
            std::swap(current, nizLista.current);
        }
        return *this;
    }
    ~NizLista()
    {
        for (int i = 0; i < brElem; i++) {
            delete p[i];
        }
        delete[] p;
    }
    int brojElemenata() const
    {
        return brElem;
    }
    Tip& trenutni()
    {
        provjeriJeLiPrazna();
        return *(p[current]);
    }
    const Tip& trenutni() const
    {
        provjeriJeLiPrazna();
        return *(p[current]);
    }
    bool prethodni()
    {
        provjeriJeLiPrazna();
        if(current == 0) {
            return false;
        } else {
            current--;
        }
        return true;
    }
    bool sljedeci()
    {
        provjeriJeLiPrazna();
        if(brElem == current + 1) {
            return false;
        } else {
            current++;
        }
        return true;
    }
    void pocetak()
    {
        provjeriJeLiPrazna();
        current = 0;
    }
    void kraj()
    {
        provjeriJeLiPrazna();
        current = brElem;
        current--;
    }
    void obrisi()
    {
        provjeriJeLiPrazna();
        delete p[current];
        for (int i = current; i < brElem - 1; i++) {
            p[i] = p[i + 1];
        }
        if(current == brElem - 1 && brElem - 1) {
            current--;
        }
        brElem = brElem - 1;
    }
    void dodajIspred(const Tip& el)
    {
        provjeriVelicinu();
        for (int i = brElem; i > current; i--) {
            p[i] = p[i - 1];
        }
        p[current] = new Tip(el);
        if(brElem) {
            current++;
        }
        brElem = brElem + 1;
    }
    void dodajIza(const Tip& el)
    {
        provjeriVelicinu();
        for (int i = brElem; i > current + 1; i--) {
            p[i] = p[i - 1];
        }
        if(!brElem) {
            p[current] = new Tip(el);
        } else {
            p[current + 1] = new Tip(el);
        }
        brElem = brElem + 1;

    }
    Tip& operator[](int index)
    {
        provjeriIndex(index);
        return *p[index];
    }
    const Tip& operator[](int index) const
    {
        provjeriIndex(index);
        return *(p[index]);
    }

};

template<typename Tip>
class JednostrukaLista: public Lista<Tip>
{
    friend class Iterator<Tip>;
    struct Cvor {
        Tip el;
        Cvor* iduci;
    };
    int brElem;
    Cvor *prvi, *zadnji, *current;

    void provjeriIndex(int index) const
    {
        if(index < 0 || index > brElem - 1) {
            throw std::domain_error("Index van opsega");
        }
    }
    void provjeriJeLiPrazna() const
    {
        //provjeriti current?
        if(!prvi) {
            throw std::domain_error("JednostrukaLista je prazna");
        }
    }
public:
    JednostrukaLista()
    {
        prvi = zadnji = current = nullptr;
        brElem = 0;
    }
    JednostrukaLista(const JednostrukaLista &l)
    {
        Cvor *temp(l.prvi), *temp2=nullptr;
        while(temp != nullptr) {
            Cvor *novi = new Cvor;
            novi->el = temp->el;
            if(!temp2) {
                prvi = novi;
            } else {
                temp2->iduci = novi;
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
    JednostrukaLista(JednostrukaLista &&l)
    {
        this->prvi = l.prvi;
        this->current = l.current;
        this->zadnji = l.zadnji;
        this->brElem = l.brElem;
        l.prvi = l.current = l.zadnji = nullptr;
        l.brElem = 0;
    }
    ~JednostrukaLista()
    {
        Cvor* temp;
        while(prvi != nullptr) {
            temp = prvi;
            prvi = prvi->iduci;
            delete temp;
        }
        prvi = nullptr;
    }
    JednostrukaLista<Tip> &operator =(const JednostrukaLista<Tip>& l)
    {
        if(this != &l) {
            Cvor* temp;
            while(prvi != nullptr) {
                temp = prvi;
                prvi = prvi->iduci;
                delete temp;
            }
            temp = l.prvi;
            Cvor *temp2(nullptr);
            while(temp != nullptr) {
                Cvor *novi = new Cvor;
                novi->el = temp->el;
                if(!temp2) {
                    temp2 = prvi = novi;
                } else {
                    temp2->iduci = novi;
                }
                temp2 = novi;
                if(l.current == temp) {
                    current = novi;
                }
                temp = temp->iduci;
            }
            zadnji = temp2;
            zadnji->iduci = nullptr;
            brElem = l.brElem;
        }
        return *this;
    }
    JednostrukaLista<Tip> &operator =(JednostrukaLista<Tip>&& l)
    {
        if(this != &l) {
            std::swap(this->prvi, l.prvi);
            std::swap(this->zadnji, l.zadnji);
            std::swap(this->current, l.current);
            std::swap(this->brElem, l.brElem);
        }
        return *this;
    }


    int brojElemenata() const
    {
        return brElem;
    }
    Tip& trenutni()
    {
        provjeriJeLiPrazna();
        return current->el;
    }
    const Tip& trenutni() const
    {
        provjeriJeLiPrazna();
        return current->el;
    }
    bool prethodni()
    {
        provjeriJeLiPrazna();
        if(prvi == current) {
            return false;
        }
        Cvor* temp(prvi);
        while(temp->iduci != current) {
            temp = temp->iduci;
        }
        current = temp;
        return true;
    }
    bool sljedeci()
    {
        provjeriJeLiPrazna();
        if(current->iduci == nullptr) {
            return false;
        } else {
            current = current->iduci;
        }
        return true;
    }
    void pocetak()
    {
        provjeriJeLiPrazna();
        current = prvi;
    }
    void kraj()
    {
        provjeriJeLiPrazna();
        current = zadnji;
    }
    void obrisi()
    {
        provjeriJeLiPrazna();
        if(current == prvi) {
            prvi = current->iduci;
            delete current;
            current = prvi;
            brElem = brElem - 1;
            if(!brElem) {
                zadnji = prvi;
            }
        } else {
            Cvor* temp(prvi);
            while(temp->iduci != current) {
                temp = temp->iduci;
            }
            temp->iduci = current->iduci;
            delete current;
            brElem = brElem - 1;
            if(temp->iduci == nullptr) {
                zadnji = current = temp;
            } else {
                current = temp->iduci;
            }
        }
    }
    void dodajIspred(const Tip& el)
    {
        Cvor* temp = new Cvor;
        temp->el = el;
        if(prvi == nullptr) {
            temp->iduci = nullptr;
            prvi = current = zadnji = temp;
            brElem = brElem + 1;
            return;
        }
        if(prvi == current) {
            prvi = temp;
            temp->iduci = current;
            brElem = brElem + 1;
            return;
        } else {
            Cvor* p(prvi);
            while(p->iduci != current) {
                p = p->iduci;
            }
            p->iduci = temp;
            temp->iduci = current;
            brElem = brElem + 1;
            return;
        }


    }
    void dodajIza(const Tip& el)
    {
        Cvor* temp = new Cvor;
        temp->el = el;
        if(prvi == nullptr) {
            temp->iduci = nullptr;
            prvi = current = zadnji = temp;
        } else {
            temp->iduci = current->iduci;
            current->iduci = temp;
            if (zadnji == current) {
                zadnji = temp;
            }

        }
        brElem = brElem + 1;
    }
    Tip& operator[](int index)
    {
        provjeriIndex(index);
        Cvor* temp (prvi);
        for (int i = 0; i < index; i++) {
            temp = temp->iduci;
        }
        return temp->el;
    }

    const Tip& operator[](int index) const
    {
        provjeriIndex(index);
        Cvor *temp (prvi);
        for (int i = 0; i < index; i++) {
            temp = temp->iduci;
        }
        return temp->el;
    }
};


template<typename Tip>
class Iterator
{
    const NizLista<Tip> *niz;
    const JednostrukaLista<Tip> *jednostrukaLista;
    int trenutniNiz;
    typename JednostrukaLista<Tip>::Cvor *trenutniLista;
public:
    Iterator(const Lista<Tip>& lista)
    {
        if(typeid(lista) == typeid(NizLista<Tip>)) {
            jednostrukaLista = nullptr;
            trenutniLista = nullptr;
            niz = (const NizLista<Tip>*) &lista;
            trenutniNiz = niz->current;
        } else {
            jednostrukaLista = (const JednostrukaLista<Tip>*) &lista;
            trenutniLista = jednostrukaLista->current;
            niz = nullptr;
            trenutniNiz = 0;
        }
    }
    Iterator(const NizLista<Tip>& nizLista): jednostrukaLista(nullptr), trenutniLista(nullptr), niz(&nizLista), trenutniNiz(niz->current) {}
    Iterator(const JednostrukaLista<Tip>& jednostrukaLista): jednostrukaLista(&jednostrukaLista), trenutniLista(jednostrukaLista->current), niz(nullptr), trenutniNiz(nullptr) {}

    bool prethodni()
    {
        if(!niz) {
            if(!jednostrukaLista->brElem) {
                throw;
            }
            if(jednostrukaLista->prvi == trenutniLista) {
                return false;
            }
            typename JednostrukaLista<Tip>::Cvor* temp(jednostrukaLista->prvi);
            while(temp->iduci != trenutniLista) {
                temp = temp->iduci;
            }
            trenutniLista = temp;
            return true;
        } else {
            if(!niz->brElem) {
                throw;
            }
            if(!trenutniNiz) {
                return false;
            } else {
                trenutniNiz--;
            }
            return true;
        }
    }
    bool sljedeci()
    {
        if(!niz) {
            if(!jednostrukaLista->brElem) {
                throw;
            }
            if(trenutniLista->iduci == nullptr) {
                return false;
            } else {
                trenutniLista = trenutniLista->iduci;
            }
            return true;
        } else {
            if(!niz->brElem) {
                throw;
            }
            if(niz->brElem == trenutniNiz + 1) {
                return false;
            } else {
                trenutniNiz++;
            }
            return true;
        }
    }
    void pocetak()
    {
        if(!niz) {
            if(!jednostrukaLista->brElem) {
                throw;
            }
            trenutniLista = jednostrukaLista->prvi;
        } else {
            if(!niz->brElem) {
                throw;
            }
            trenutniNiz = 0;
        }

    }
    void kraj()
    {
        if(!niz) {
            if(!jednostrukaLista->brElem) {
                throw;
            }
            trenutniLista = jednostrukaLista->zadnji;
        } else {
            if(!niz->brElem) {
                throw;
            }
            trenutniNiz = niz->brElem;
            trenutniNiz--;
        }
    }
    Tip& trenutni()
    {
        if(!niz) {
            if(!jednostrukaLista->brElem) {
                throw;
            }
            return trenutniLista->el;
        } else {
            if(!niz->brElem) {
                throw;
            }
            return *(niz->p[trenutniNiz]);
        }
    }
};

template<typename Tip>
Tip DajMaksimum(const Lista<Tip>& lista)
{
    Iterator<Tip> it(lista);
    it.pocetak();
    Tip max;
    try {
        max = lista.trenutni();
        for (int i = 0; i < lista.brojElemenata(); i++) {
            if(lista[i] > max) {
                max = lista[i];
            }
        }
        return max;
    } catch(...) {

    }
    return max;

}

template<typename Tip>
NizLista<Tip> NizListaMoveKonstruktor()
{
    NizLista<Tip> niz(10);
    niz.dodajIspred(1);
    niz.dodajIza(2);
    niz.kraj();
    niz.dodajIza(3);
    niz.pocetak();
    niz.dodajIspred(0);
    return niz;
}

template<typename Tip>
JednostrukaLista<Tip> JednostrukaListaMoveKonstruktor()
{
    JednostrukaLista<Tip> lista;
    lista.dodajIspred(1);
    lista.dodajIza(2);
    lista.kraj();
    lista.dodajIza(3);
    lista.pocetak();
    lista.dodajIspred(0);
    return lista;
}

void testIzuzeciNizLista() {
    NizLista<int> nizLista(1);
    try {
        nizLista.obrisi();
    } catch(...) {
        cout << "Test 1 ok" << std::endl;
    }

    try {
        nizLista.prethodni();
    } catch(...) {
        cout << "Test 2 ok" << std::endl;
    }

    try {
        nizLista.sljedeci();
    } catch(...) {
        cout << "Test 3 ok" << std::endl;
    }

    try {
        nizLista.trenutni();
    } catch(...) {
        cout << "Test 4 ok" << std::endl;
    }

    try {
        nizLista.pocetak();
    } catch(...) {
        cout << "Test 5 ok" << std::endl;
    }

    try {
        nizLista.kraj();
    } catch(...) {
        cout << "Test 6 ok" << std::endl;
    }

    try {
        nizLista[44] = 0;
    } catch(...) {
        cout << "Test 7 ok" << std::endl;
    }
    const auto& ref(nizLista);
    try {
        std::cout << ref[44];
    } catch(...) {
        cout << "Test 8 ok" << std::endl;
    }

    try {
        ref.trenutni();
    } catch(...) {
        cout << "Test 9 ok" << std::endl;
    }
}

void testIzuzeciPovezanaLista() {
    JednostrukaLista<int> jednostrukaLista;
    try {
        jednostrukaLista.obrisi();
    } catch(...) {
        cout << "Test 1 ok" << std::endl;
    }

    try {
        jednostrukaLista.prethodni();
    } catch(...) {
        cout << "Test 2 ok" << std::endl;
    }

    try {
        jednostrukaLista.sljedeci();
    } catch(...) {
        cout << "Test 3 ok" << std::endl;
    }

    try {
        jednostrukaLista.trenutni();
    } catch(...) {
        cout << "Test 4 ok" << std::endl;
    }

    try {
        jednostrukaLista.pocetak();
    } catch(...) {
        cout << "Test 5 ok" << std::endl;
    }

    try {
        jednostrukaLista.kraj();
    } catch(...) {
        cout << "Test 6 ok" << std::endl;
    }

    try {
        jednostrukaLista[44] = 0;
    } catch(...) {
        cout << "Test 7 ok" << std::endl;
    }
    const auto& uvezanaRef(jednostrukaLista);
    try {
        std::cout << uvezanaRef[44];
    } catch(...) {
        cout << "Test 8 ok" << std::endl;
    }

    try {
        uvezanaRef.trenutni();
    } catch(...) {
        cout << "Test 9 ok" << std::endl;
    }
}



int main()
{
    testIzuzeciNizLista();
    
    NizLista<int> nizLista(1);
    const auto& ref(nizLista);
    
    nizLista.dodajIspred(1);
    nizLista.dodajIza(2);
    if(nizLista.brojElemenata() == 2) {
        cout << "Test 10 ok" << std::endl;
    }

    {
        NizLista<int> temp(3);
        temp.dodajIspred(9);
        temp.dodajIspred(8);
        temp.dodajIspred(7);
        nizLista = std::move(temp);
    }

    if(ref.brojElemenata() == 3 && nizLista.trenutni() == 9) {
        cout << "Test 11 ok" << std::endl;
    }

    nizLista.pocetak();
    if(nizLista.trenutni() == 8) {
        cout << "Test 12 ok" << std::endl;
    }

    nizLista.sljedeci();
    if(nizLista.trenutni() == 7) {
        cout << "Test 13 ok" << std::endl;
    }

    nizLista.dodajIza(6);
    nizLista.sljedeci();
    if(nizLista.trenutni() == 6) {
        cout << "Test 14 ok" << std::endl;
    }

    nizLista.kraj();
    if(nizLista.trenutni() == 9) {
        cout << "Test 15 ok" << std::endl;
    }

    nizLista.dodajIspred(5);
    nizLista.prethodni();
    if(nizLista.trenutni() == 5) {
        cout << "Test 16 ok" << std::endl;
    }

    nizLista.obrisi();
    if(nizLista.trenutni() == 9) {
        cout << "Test 17 ok" << std::endl;
    }

    nizLista.obrisi();
    if(nizLista.trenutni() == 6) {
        cout << "Test 18 ok" << std::endl;
    }

    nizLista.pocetak();
    nizLista.obrisi();
    if(nizLista.trenutni() == 7) {
        cout << "Test 19 ok" << std::endl;
    }

    nizLista = NizListaMoveKonstruktor<int>();
    std::cout << "Elementi liste: ";
    for (int i = 0; i < nizLista.brojElemenata(); i++) {
        std::cout << ref[i] << " ";
    }
    std::cout << std::endl;

    //kopirajuci konsturktor
    NizLista<int> niz2 (nizLista);
    for (int i = 0; i < nizLista.brojElemenata(); i++) {
        if(nizLista[i] != niz2[i]) {
            std::cout << "Belaj" << std::endl;
        }
    }
    nizLista[0] = 5;
    if(nizLista[0] == niz2[0]) {
        std::cout << "Belaj" << std::endl;
    }
    nizLista.pocetak();
    nizLista.obrisi();
    nizLista.kraj();
    nizLista.obrisi();

    std::cout << "Nakon brisanja prvog i zadnjeg: ";
    nizLista.pocetak();
    for (int i = 0; i < nizLista.brojElemenata(); i++) {
        std::cout << ref[i] << " ";
    }
    std::cout << std::endl;
    nizLista.dodajIza(-999);
    std::cout << "Maksimum niz liste: " << DajMaksimum(nizLista) << std::endl;

    std::cout << std::endl << std::endl;






    std::cout<< "--------Jednostruka lista--------" << std::endl << std::endl;
    
    
    testIzuzeciPovezanaLista();
    
    JednostrukaLista<int> jednostrukaLista;
    const auto& uvezanaRef(jednostrukaLista);
    
    jednostrukaLista.dodajIspred(1);
    jednostrukaLista.dodajIza(2);
    if(jednostrukaLista.brojElemenata() == 2) {
        cout << "Test 10 ok" << std::endl;
    }

    {
        JednostrukaLista<int> temp;
        temp.dodajIspred(9);
        temp.dodajIspred(8);
        temp.dodajIspred(7);
        jednostrukaLista = std::move(temp);
    }

    if(uvezanaRef.brojElemenata() == 3 && jednostrukaLista.trenutni() == 9) {
        cout << "Test 11 ok" << std::endl;
    }

    jednostrukaLista.pocetak();
    if(jednostrukaLista.trenutni() == 8) {
        cout << "Test 12 ok" << std::endl;
    }

    jednostrukaLista.sljedeci();
    if(jednostrukaLista.trenutni() == 7) {
        cout << "Test 13 ok" << std::endl;
    }

    jednostrukaLista.dodajIza(6);
    jednostrukaLista.sljedeci();
    if(jednostrukaLista.trenutni() == 6) {
        cout << "Test 14 ok" << std::endl;
    }

    jednostrukaLista.kraj();
    if(jednostrukaLista.trenutni() == 9) {
        cout << "Test 15 ok" << std::endl;
    }

    jednostrukaLista.dodajIspred(5);
    jednostrukaLista.prethodni();
    if(jednostrukaLista.trenutni() == 5) {
        cout << "Test 16 ok" << std::endl;
    }

    jednostrukaLista.obrisi();
    if(jednostrukaLista.trenutni() == 9) {
        cout << "Test 17 ok" << std::endl;
    }

    jednostrukaLista.obrisi();
    if(jednostrukaLista.trenutni() == 6) {
        cout << "Test 18 ok" << std::endl;
    }

    jednostrukaLista.pocetak();
    jednostrukaLista.obrisi();
    if(jednostrukaLista.trenutni() == 7) {
        cout << "Test 19 ok" << std::endl;
    }

    jednostrukaLista = JednostrukaListaMoveKonstruktor<int>();
    std::cout << "Elementi liste: ";
    for (int i = 0; i < jednostrukaLista.brojElemenata(); i++) {
        std::cout << uvezanaRef[i] << " ";
    }
    std::cout << std::endl;

    JednostrukaLista<int> jednostrukaLista2 (jednostrukaLista);
    jednostrukaLista.pocetak();
    jednostrukaLista2.pocetak();
    for (int i = 0; i < jednostrukaLista.brojElemenata(); i++) {
        if(jednostrukaLista.trenutni() != jednostrukaLista2.trenutni()) {
            std::cout << "Belaj" << std::endl;
        }
        if(i < jednostrukaLista.brojElemenata() - 1) {
            jednostrukaLista.sljedeci();
            jednostrukaLista2.sljedeci();
        }
    }
    jednostrukaLista[0] = 5;
    if(jednostrukaLista[0] == jednostrukaLista2[0]) {
        std::cout << "Belaj" << std::endl;
    }
    jednostrukaLista.pocetak();
    jednostrukaLista.obrisi();
    jednostrukaLista.kraj();
    jednostrukaLista.obrisi();

    std::cout << "Nakon brisanja prvog i zadnjeg: ";
    jednostrukaLista.pocetak();
    for (int i = 0; i < jednostrukaLista.brojElemenata(); i++) {
        std::cout << jednostrukaLista.trenutni() << " ";
        if(i < jednostrukaLista.brojElemenata() - 1) {
            jednostrukaLista.sljedeci();
        }
    }
    std::cout << std::endl;
    jednostrukaLista.dodajIza(-999);
    std::cout << "Maksimum jednostruke liste: " << DajMaksimum(jednostrukaLista) << std::endl;
    return 0;
}
