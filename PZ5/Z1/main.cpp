#include <iostream>
#include <algorithm>
#include <iostream>
#include <typeinfo>
#include <ctime>

using namespace std;

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
class JednostrukaLista: public Lista<Tip>
{
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
    
    void ispisUnazad() { 
        Cvor* temp = prvi;
        if(prvi->iduci != nullptr) {
            prvi = prvi->iduci;
            this->ispisUnazad();
        }
        std::cout << temp->el << " ";
        prvi = temp;
    }
        
};



int fib (int n) {
    if (n<=1) return n; // 0 i 1
    return fib(n-1) + fib(n-2);
}

int fib_petlja (int n) {
    if (n<=1) return n; // 0 i 1
    int pretprosli(0), prosli(1), rezultat;
    for (int i(2); i<=n; i++) {
        rezultat = pretprosli+prosli;
        pretprosli = prosli;
        prosli = rezultat;
    }
    return rezultat;
}

int fib2_0(int n, int last = 1, int secondFromLast = 0) {
    if(n > 2) {
        return fib2_0(n - 1, last + secondFromLast, last);
    }
    else if(n <= 1) {
        return n;
    } else {
        return(last + secondFromLast);
    }
}

int nzd(int x, int y) {
    if(!y) {
        return x;
    } else {
        return nzd(y, x%y);
    }
}



double pow(double a, unsigned int n) {
    if(!n) {
        return 1;
    } else if (n == 1) {
        return a;
    } else {
        double temp = pow(a, n/2);
        if(n%2) {
            return a*temp*temp;
        } else {
            return temp*temp;
        }
    }
}







template<typename Tip>
void insertionSort(Tip* L, int vel) {
    
    for(int i = 1; i <= vel - 1; i++) {
        Tip priv = L[i];
        int j = i - 1;
        while (j >= 0 && L[j] > priv) {
            L[j+1] = L[j];
            j = j-1;
        }
         L[j+1] = priv;
    }    
}
/*
template <class Tip> // Shell sort
void shellSort(Tip* niz, int vel, int* h, int hVel) {
    int H[] = {7, 3,1}; // Niz razmaka
    for (int i = 0; i < 3; i++) {
         int h = H[i];
        for (int j = h; j < duzina; j++) {
         Tip priv = L[j];
        int k = j-h;
        while (k >= 0 && L[k] > priv) {
            L[k+h] = L[k];
            k = k-h;
        }
        L[k+h] = priv;
    }
 }
*/






int main() {
    int niz[10] = {1, 6, 8, 7, 15, 168, 6666, 4898, 5485, 1000};
    insertionSort(&niz[0], 10);
    for (int i = 0; i < 10; i++) {
        std::cout << niz[i];
    }
    
    JednostrukaLista<int> lista;
    for (int i = 0; i < 10; i++) {
        lista.dodajIza(i);
        lista.sljedeci();
    }
    
    lista.ispisUnazad();
    std::cout << std::endl;
    std::cout << pow(5,5);
    return 0;
}
