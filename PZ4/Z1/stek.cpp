#include <iostream>
#include <algorithm>
#include <iostream>
#include <typeinfo>

using namespace std;

template<typename T>
class Iterator;

template <typename T>
class Lista
{
public:
    Lista() {}
    virtual ~Lista() {};
    virtual int brojElemenata() const = 0;
    virtual T& trenutni() = 0;
    virtual const T& trenutni() const = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const T& el) = 0;
    virtual void dodajIza(const T& el) = 0;
    virtual T& operator[](int index) = 0;
    virtual const T& operator[](int index) const = 0;
};

template<typename T>
class NizLista: public Lista<T>
{
    friend class Iterator<T>;
    int brElem, kap, current;
    T **p;
    void provjeriVelicinu()
    {
        if(brElem == kap) {
            kap *= 2;
            T **p2 = new T*[kap];
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
        p = new T*[kap] {};
    }

    //kopirajuci konstruktor
    NizLista(const NizLista &nizLista)
    {
        this->kap = nizLista.kap;
        this->brElem = nizLista.brElem;
        this->current = nizLista.current;
        p = new T*[this->kap];
        for (int i = 0; i < this->brElem; i++) {
            p[i] = new T(*(nizLista.p[i]));
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

    NizLista<T> &operator =(const NizLista<T>& nizLista)
    {
        if(this != &nizLista) {
            for (int i = 0; i < brElem; i++) {
                delete p[i];
            }
            delete[] p;
            this->kap = nizLista.kap;
            this->brElem = nizLista.brElem;
            this->current = nizLista.current;
            p = new T*[this->kap];
            for (int i = 0; i < this->brElem; i++) {
                p[i] = new T(*(nizLista.p[i]));
            }
        }
        return *this;
    }
    NizLista<T> &operator =(NizLista<T>&& nizLista)
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
    T& trenutni()
    {
        provjeriJeLiPrazna();
        return *(p[current]);
    }
    const T& trenutni() const
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
    void dodajIspred(const T& el)
    {
        provjeriVelicinu();
        for (int i = brElem; i > current; i--) {
            p[i] = p[i - 1];
        }
        p[current] = new T(el);
        if(brElem) {
            current++;
        }
        brElem = brElem + 1;
    }
    void dodajIza(const T& el)
    {
        provjeriVelicinu();
        for (int i = brElem; i > current + 1; i--) {
            p[i] = p[i - 1];
        }
        if(!brElem) {
            p[current] = new T(el);
        } else {
            p[current + 1] = new T(el);
        }
        brElem = brElem + 1;

    }
    T& operator[](int index)
    {
        provjeriIndex(index);
        return *p[index];
    }
    const T& operator[](int index) const
    {
        provjeriIndex(index);
        return *(p[index]);
    }

};


template<typename T>
class Stek {
    NizLista<T> lista;
  public:  
  Stek() {}
  void brisi() {
      int temp = lista.brojElemenata();
      for (int i = 0; i < temp; i++) {
          lista.obrisi();
      }
  }
  void stavi(const T& el) {
      if(lista.brojElemenata()) {
        lista.kraj();
      }
      lista.dodajIza(el);
      lista.sljedeci();
  }
  T skini() {
      lista.kraj();
      T el (lista.trenutni());
      lista.obrisi();
      return el;
  }
  T& vrh() {
      lista.kraj();
      return lista.trenutni();
  }
  int brojElemenata() {
      return lista.brojElemenata();
  }
};

void testIzuzeci() {
    Stek<int> s;
    try{
        s.skini();
        std::cout << "Test 1 nije ok" << std::endl;
    } catch(...) {
        std::cout << "Test 1 ok" << std::endl;
    }
    
    try{
        s.vrh();
        std::cout << "Test 2 nije ok" << std::endl;
    } catch(...) {
        std::cout << "Test 2 ok" << std::endl;
    }
}

void testPushPop() {
    Stek<int> s;
    s.stavi(1);
    s.stavi(2);
    s.stavi(3);
    if(s.vrh() != 3) {
        std::cout << "Test 3 nije ok"<< std::endl;
        return;
    }
    s.skini();
    if(s.vrh() != 2) {
        std::cout << "Test 3 nije ok"<< std::endl;
        return;
    }
    s.skini();
    s.skini();
    try{
        s.skini();        
    } catch(...) {
        std::cout << "Test 3 ok" << std::endl;
    }
    
    
}

void testBrisanje() {
    Stek<int> s;
    s.stavi(420);
    s.stavi(45);
    s.stavi(88);
    if(s.brojElemenata() != 3) {
        std::cout << "Test 4 nije ok" << std::endl;
        return;
    }
    s.brisi();
    try {
        s.vrh();
    }
    catch (...) {
        std::cout << "Test 4 ok" << std::endl;
    }
}

void palindrm() {
    NizLista<char> l;
    l.dodajIza('k');
    l.dodajIza('a');
    l.sljedeci();
    l.dodajIza('p');
    l.sljedeci();
    l.dodajIza('a');
    l.sljedeci();
    l.dodajIza('p');
    
    Stek<char> s;
    Stek<char> s2;
    for (int i = 0; i < l.brojElemenata(); i++) {
        s.stavi(l[i]);
    }
    for (int i = l.brojElemenata()-1; i >= 0; i--) {
        s2.stavi(l[i]);
    }
     for (int i = l.brojElemenata()-1; i >= 0; i--) {
        if(s.skini() != s2.skini()) {
            std::cout << "belaj";
            return;
        }
    }
    std::cout << "palindrom ok";
    
}
