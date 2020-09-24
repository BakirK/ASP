#include <iostream>
#include <algorithm>
#include <iostream>
#include <vector>
#include <typeinfo>
using namespace std;

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

void pretraga(Stek<std::vector<int> >& s, int trazeni) {
    std::vector<int>* v = nullptr;
    try{
        if(!s.brojElemenata()) {
            std::cout << "Nema elementa" << std::endl;
            return;
        }
        v = new std::vector<int>(s.skini());
        if(v->size()) {
            if(trazeni == v->at(0)) std::cout << 0 << " " << s.brojElemenata();
            else if(trazeni > v->at(0)) {
                auto p = std::lower_bound(v->begin(), v->end(), trazeni);
                if(p != v->end() && *p == trazeni) {
                    std::cout << p - v->begin() << " " << s.brojElemenata();
                    s.stavi(*v);
                    delete v;
                    return;
                } else {
                    std::cout << "Nema elementa" << std::endl;
                    s.stavi(*v);
                    delete v;
                    return;
                }
            } else {
                if(s.brojElemenata()) {
                    pretraga(s, trazeni);    
                } else {
                    std::cout << "Nema elementa" << std::endl;
                    s.stavi(*v);
                    delete v;
                    return;
                }
            }
        } 
        else {
            if(s.brojElemenata()) {
                pretraga(s, trazeni);    
            } else {
                std::cout << "Nema elementa" << std::endl;
                s.stavi(*v);
                delete v;
                return;
            }
        }
        s.stavi(*v);
        delete v;
    } catch(...) {
        delete v;
        return;
    }
}

void pretragaTest1() {
    Stek<std::vector<int> > s;
    pretraga(s, 45); //nema elemenata
}

void pretragaTest2() {
    Stek<std::vector<int> > s;
    for(int j = 0; j < 1000; j+=100) {
        std::vector<int> v;
        for (int i = j; i < j + 20; i++) {
            v.push_back(i);
        }
        s.stavi(v);
    }
    pretraga(s, 900); //0 9 prvi element u prvom vektoru
}

void pretragaTest3() {
    Stek<std::vector<int> > s;
    for(int j = 0; j < 1000; j+=100) {
        std::vector<int> v;
        for (int i = j; i < j + 20; i++) {
            v.push_back(i);
        }
        s.stavi(v);
    }
    pretraga(s, 17000); //nema trazenog elementa
    pretraga(s, 19); //zadnji element u zadnjem vektoru
}


int main() {
    pretragaTest1();
    pretragaTest2();
    std::cout << std::endl;
    pretragaTest3();
    return 0;
}
