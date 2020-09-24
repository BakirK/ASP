#include <iostream>
#include <algorithm>
#include <iostream>
#include <typeinfo>
clock_t vrijeme1 = clock();

using namespace std;

template<typename Tip>
class Iterator;

template<typename Tip>
class DvostrukaLista;

//DVOSTRUKA LISTA SE NALAZI PRI KRAJU

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
    JednostrukaLista(const JednostrukaLista& l)
    {
        Cvor *temp(l.prvi), *temp2 = nullptr;
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
        prvi = nullptr; //ne treba?
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
    const DvostrukaLista<Tip> *dvostrukaLista;
    int trenutniNiz;
    typename JednostrukaLista<Tip>::Cvor *trenutniLista;
    typename DvostrukaLista<Tip>::Cvor *trenutniDvostruka;
public:
    Iterator(const Lista<Tip>& lista)
    {
        if(typeid(lista) == typeid(NizLista<Tip>)) {
            jednostrukaLista = nullptr;
            trenutniLista = nullptr;
            dvostrukaLista = nullptr;
            trenutniDvostruka = nullptr;
            
            niz = (const NizLista<Tip>*) &lista;
            trenutniNiz = niz->current;
        } else if(typeid(JednostrukaLista<Tip>) == typeid(lista)){
            jednostrukaLista = (const JednostrukaLista<Tip>*) &lista;
            trenutniLista = jednostrukaLista->current;
            
            niz = nullptr;
            trenutniNiz = 0;
            dvostrukaLista = nullptr;
            trenutniDvostruka = nullptr;
        } else {
            dvostrukaLista = (const DvostrukaLista<Tip>*) &lista;
            trenutniDvostruka = dvostrukaLista->current;
            
            niz = nullptr;
            trenutniNiz = 0;
            jednostrukaLista = nullptr;
            trenutniLista = nullptr;
        }
    }
    Iterator(const NizLista<Tip>& nizLista): jednostrukaLista(nullptr), trenutniLista(nullptr), niz(&nizLista), trenutniNiz(niz->current) {
        dvostrukaLista = nullptr;
        trenutniDvostruka = nullptr;
    }
    Iterator(const JednostrukaLista<Tip>& jednostrukaLista): jednostrukaLista(&jednostrukaLista), trenutniLista(jednostrukaLista->current), niz(nullptr), trenutniNiz(nullptr) {
        dvostrukaLista = nullptr;
        trenutniDvostruka = nullptr;
    }
    Iterator(const DvostrukaLista<Tip>& dvostrukaLista): dvostrukaLista(&dvostrukaLista), trenutniDvostruka(dvostrukaLista->current){
        jednostrukaLista = nullptr;
        trenutniLista = nullptr;
        niz = nullptr;
        trenutniNiz = nullptr;
    }

    bool prethodni()
    {
        if(jednostrukaLista) {
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
        } else if(niz) {
            if(!niz->brElem) {
                throw;
            }
            if(!trenutniNiz) {
                return false;
            } else {
                trenutniNiz--;
            }
            return true;
        } else {
            if(!dvostrukaLista->brElem) {
                throw;
            }
            if(dvostrukaLista.prvi == trenutniDvostruka) {
                return false;
            } else {
                trenutniDvostruka = trenutniDvostruka->prethodni;
            }
            return true;
        }
    }
    bool sljedeci()
    {
        if(jednostrukaLista) {
            if(!jednostrukaLista->brElem) {
                throw;
            }
            if(trenutniLista->iduci == nullptr) {
                return false;
            } else {
                trenutniLista = trenutniLista->iduci;
            }
            return true;
        } else if(niz) {
            if(!niz->brElem) {
                throw;
            }
            if(niz->brElem == trenutniNiz + 1) {
                return false;
            } else {
                trenutniNiz++;
            }
            return true;
        } else {
            if(!dvostrukaLista->brElem) {
                throw;
            }
            if(dvostrukaLista->zadnji == trenutniDvostruka) {
                return false;
            } else {
                trenutniDvostruka = trenutniDvostruka->iduci;
            }
            return true;
        }
    }
    void pocetak()
    {
        if(jednostrukaLista) {
            if(!jednostrukaLista->brElem) {
                throw;
            }
            trenutniLista = jednostrukaLista->prvi;
        } else if(niz) {
            if(!niz->brElem) {
                throw;
            }
            trenutniNiz = 0;
        }  else {
            if(!(dvostrukaLista->brElem)) {
                throw;
            }
            trenutniDvostruka = dvostrukaLista->prvi;
        }

    }
    void kraj()
    {
        if(jednostrukaLista) {
            if(!jednostrukaLista->brElem) {
                throw;
            }
            trenutniLista = jednostrukaLista->zadnji;
        } else if(niz) {
            if(!niz->brElem) {
                throw;
            }
            trenutniNiz = niz->brElem;
            trenutniNiz--;
        } else {
            if(!dvostrukaLista->brElem) {
                throw;
            }
            trenutniDvostruka = dvostrukaLista->zadnji;
        }
    }
    Tip& trenutni()
    {
        if(jednostrukaLista) {
            if(!jednostrukaLista->brElem) {
                throw;
            }
            return trenutniLista->el;
        } else if(niz) {
            if(!niz->brElem) {
                throw;
            }
            return *(niz->p[trenutniNiz]);
        } else {
            if(!dvostrukaLista->brElem) {
                throw;
            }
            return trenutniDvostruka->el;
        }
    }
};

template<typename Tip>
Tip dajMaksimum(const Lista<Tip>& lista)
{
    Iterator<Tip> it(lista);
    try {
        it.pocetak();
    }
    catch (...) {
        //ovo se ne desi jer se prekine program kad pocetak baci izuzetak
        throw;
    }
    Tip max;
    max = it.trenutni();
    it.pocetak();
    for (int i = 0; i < lista.brojElemenata(); i++) {
        if(it.trenutni() > max) {
            max = it.trenutni();
        }
        it.sljedeci();
    }
    return max;
}

template<typename Tip>
class DvostrukaLista: public Lista<Tip> {
    friend class Iterator<Tip>;
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

DvostrukaLista<int> dajListu() {
    DvostrukaLista<int> l;
    l.dodajIza(3);
    l.dodajIza(1);
    l.dodajIza(2);
    return l;
}

void testMoveConstructor() {
    DvostrukaLista<int> temp (dajListu());
    DvostrukaLista<int> l = std::move(temp);
    if(l.brojElemenata() != 3) std::cout << "Problem testMoveConstructor" << std::endl;
    if(l[0] != 3) std::cout << "Problem testJednako" << std::endl;
    if(l[1] != 2) std::cout << "Problem testMoveConstructor" << std::endl;
    if(l[2] != 1) std::cout << "Problem testMoveConstructor" << std::endl;
    std::cout << "testMoveConstructor ok" << std::endl;
}

void testCopyConstructor() {
    DvostrukaLista<int> temp (dajListu());
    DvostrukaLista<int> l (temp);
    if(l.brojElemenata() != 3) std::cout << "Problem testMoveConstructor" << std::endl;
    if(l[0] != 3) std::cout << "Problem testJednako" << std::endl;
    if(l[1] != 2) std::cout << "Problem testMoveConstructor" << std::endl;
    if(l[2] != 1) std::cout << "Problem testMoveConstructor" << std::endl;
    std::cout << "testMoveConstructor ok" << std::endl;
}

void testJednako() {
    DvostrukaLista<int> l, l2;
    l.dodajIza(1);
    l.dodajIza(2);
    l.dodajIza(3);
    l2 = l;
    l.obrisi(); l.obrisi(); l.obrisi();
    if(l2.trenutni() != 1) std::cout << "Problem testJednako" << std::endl;
    if(l2[1] != 3) std::cout << "Problem testJednako" << std::endl;
    if(l2[2] != 2) std::cout << "Problem testJednako" << std::endl;
    if(l2.brojElemenata() != 3) std::cout << "Problem testJednako" << std::endl;
    
    
    l = std::move(l2);
    if(l.trenutni() != 1) std::cout << "Problem testJednako" << std::endl;
    if(l[1] != 3) std::cout << "Problem testJednako" << std::endl;
    if(l[2] != 2) std::cout << "Problem testJednako" << std::endl;
    if(l.brojElemenata() != 3) std::cout << "Problem testJednako" << std::endl;
    if(l2.brojElemenata() != 0) std::cout << "Problem testJednako" << std::endl;
    std::cout << "testJednako ok" << std::endl;
}





void testBrojElemenata() {
    DvostrukaLista<int> l;
    if(l.brojElemenata() != 0) {
        std::cout << "Problem testBrojElemenata" << std::endl;
    }
    l.dodajIspred(33);
    if(l.brojElemenata() != 1) {
        std::cout << "Problem testBrojElemenata" << std::endl;
    }
    l.dodajIspred(32);
    l.obrisi();
    if(l.brojElemenata() != 1) {
        std::cout << "Problem testBrojElemenata" << std::endl;
    }
    std::cout << "testBrojElemenata ok" << std::endl;
}

template<typename T>
void testConstTrenutni(const DvostrukaLista<T>& l) {
    if(l.trenutni() != 17) {
        std::cout << "Problem testTrenutni" << std::endl;
    }
}

void testTrenutni() {
    DvostrukaLista<int> l;
    try {
        l.trenutni();
        std::cout << "Problem testTrenutni" << std::endl;
    } catch(...) {}
    l.dodajIspred(15);
    if(l.trenutni() != 15) std::cout << "Problem testTrenutni" << std::endl;
    l.dodajIza(33);
    if(l.trenutni() != 15) std::cout << "Problem testTrenutni" << std::endl;
    l.dodajIspred(17);
    if(l.trenutni() != 15) std::cout << "Problem testTrenutni" << std::endl;
    l.obrisi();
    if(l.trenutni() != 33) std::cout << "Problem testTrenutni" << std::endl;
    l.obrisi();
    if(l.trenutni() != 17) std::cout << "Problem testTrenutni" << std::endl;
    testConstTrenutni<int>(l);
    l.obrisi();
    try {
        l.trenutni();
        std::cout << "Problem testTrenutni" << std::endl;
    } catch(...) {}
    std::cout << "testTrenutni ok" << std::endl;
}

void testPrethodni() {
    DvostrukaLista<int> l;
    try {
        l.prethodni();
        std::cout << "Problem testPrethodni" << std::endl;
    } catch(...){}
    l.dodajIspred(5);
    l.dodajIspred(3);
    l.dodajIspred(1);
    l.prethodni();
    if(l.trenutni() != 1) std::cout << "Problem testPrethodni" << std::endl;
    l.prethodni();
    if(l.trenutni() != 3) std::cout << "Problem testPrethodni" << std::endl;
    bool temp = l.prethodni();
    if(!temp) {
        std::cout << "testPrethodni ok" << std::endl;    
    }
}


void testSljedeci() {
    DvostrukaLista<int> l;
    try {
        l.sljedeci();
        std::cout << "Problem testSljedeci" << std::endl;
    } catch(...){}
    l.dodajIza(5);
    l.dodajIza(3);
    l.dodajIza(1);
    l.sljedeci();
    if(l.trenutni() != 1) std::cout << "Problem testSljedeci" << std::endl;
    l.sljedeci();
    if(l.trenutni() != 3) std::cout << "Problem testSljedeci" << std::endl;
    bool temp = l.sljedeci();
    if(!temp) {
        std::cout << "testSljedeci ok" << std::endl;    
    }
}


void testPocetak() {
    DvostrukaLista<int> l;
    try {
        l.pocetak();
        std::cout << "Problem testPocetak" << std::endl;
    } catch(...){}
    l.dodajIspred(5);
    l.dodajIspred(3);
    l.dodajIspred(1);
    l.pocetak();
    if(l.trenutni() != 3) std::cout << "Problem testPocetak" << std::endl;
    std::cout << "testPocetak ok" << std::endl;
}

void testKraj() {
    DvostrukaLista<int> l;
    try {
        l.kraj();
        std::cout << "Problem testKraj" << std::endl;
    } catch(...){}
    l.dodajIza(5);
    l.dodajIza(3);
    l.dodajIza(1);
    l.kraj();
    if(l.trenutni() != 3) std::cout << "Problem testKraj" << std::endl;
    std::cout << "testKraj ok" << std::endl;
}

void testObrisi() {
    DvostrukaLista<int> l;
    try {
        l.obrisi();
        std::cout << "Problem testObrisi" << std::endl;
    } catch(...){}
    l.dodajIza(5);
    l.dodajIza(3);
    l.dodajIza(1);
    l.obrisi();
    if(l.trenutni() != 1) std::cout << "Problem testObrisi" << std::endl;
    l.obrisi();
    if(l.trenutni() != 3) std::cout << "Problem testObrisi" << std::endl;
    std::cout << "testObrisi ok" << std::endl;
}

void testDodajIspred() {
    DvostrukaLista<int> l;
    l.dodajIspred(5);
    if(l.trenutni() != 5) std::cout << "Problem testDodajIspred" << std::endl;
    l.dodajIspred(3);
    if(l.trenutni() != 5) std::cout << "Problem testDodajIspred" << std::endl;
    l.prethodni();
    l.dodajIspred(1);
    l.prethodni();
    if(l.trenutni() != 1) std::cout << "Problem testDodajIspred" << std::endl;
    std::cout << "testDodajIspred ok" << std::endl;
}

void testDodajIza() {
    DvostrukaLista<int> l;
    l.dodajIza(5);
    if(l.trenutni() != 5) std::cout << "Problem testDodajIza" << std::endl;
    l.dodajIza(3);
    if(l.trenutni() != 5) std::cout << "Problem testDodajIza" << std::endl;
    l.sljedeci();
    l.dodajIza(1);
    l.sljedeci();
    if(l.trenutni() != 1) std::cout << "Problem testDodajIza" << std::endl;
    std::cout << "testDodajIza ok" << std::endl;
}

template<typename T>
void testConstUglaste(const DvostrukaLista<T> l) {
    if(l[1] != 6) std::cout << "Problem testUglaste const" << std::endl;
}

void testUglaste() {
    DvostrukaLista<int> l;
    try {
        l[420];
        std::cout << "Problem testUglaste" << std::endl;
    }
    catch (...) {
    }
    l.dodajIza(5);
    if(l[0] != 5) std::cout << "Problem testUglaste" << std::endl;
    l.dodajIza(6);
    if(l[1] != 6) std::cout << "Problem testUglaste" << std::endl;
    testConstUglaste(l);
    std::cout << "testUglaste ok" << std::endl;
}

void testMax() {
    DvostrukaLista<int> l;
    //try catch ne radi jer se throw ne propagira do catch 
    //vec samo prekine program iz nekog razloga???
    /*try{
        dajMaksimum(l);
        std::cout << "Problem testMax" << std::endl;
    } catch (...) {
        throw;
    }*/
    
    for (int i = 0; i < 100; i++) {
        l.dodajIza(i);
        l.sljedeci();
    }
    int max = dajMaksimum(l);
    if(max != 99) std::cout << "Problem testMax" << std::endl; 
    else std::cout << "testMax ok" << std::endl;
}

int main()
{ 
    testCopyConstructor();
    testCopyConstructor();
    testJednako();
    testBrojElemenata();
    testTrenutni();
    testPrethodni();
    testSljedeci();
    testPocetak();
    testKraj();
    testObrisi();
    testDodajIspred();
    testDodajIza();
    testUglaste();
    try {
        testMax();
    }
    catch (...) {
        std::cout << "greska" << std::endl;
    }
    
    
    /*
    clock_t vrijeme1 = clock();
    DvostrukaLista<int> l;
    for (int i(0); i<=100000; i++) {
        l.dodajIspred(i);
        
    }
    for (int i(l.brojElemenata()-1); i>=0; i--) {
        std::cout << l[i] <<" ";
        
    }
    
    std::cout << dajMaksimum(l);
    clock_t vrijeme2 = clock();
    int ukvrijeme = (vrijeme2-vrijeme1) / (CLOCKS_PER_SEC / 1000);
        std::cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms.";*/
    return 0;
}
