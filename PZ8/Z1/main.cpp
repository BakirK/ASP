#include <iostream>
#include <utility> 
#include <time.h>
#include <stdlib.h>


template <typename TipKljuca, typename TipVrijednosti>
class Mapa
{
public:
    Mapa() {}
    virtual ~Mapa() {};
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca& kljuc) = 0;
    virtual TipVrijednosti& operator[](TipKljuca index) = 0;
    virtual const TipVrijednosti& operator[](TipKljuca index) const = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class NizMapa: public Mapa<TipKljuca, TipVrijednosti> {
    std::pair<TipKljuca, TipVrijednosti>** elementi;
    int brElem, kapacitet;
    TipVrijednosti defaultVrijednost;
    public:
    NizMapa(unsigned int velicina = 1000) {
        defaultVrijednost = TipVrijednosti();
        elementi = new std::pair<TipKljuca, TipVrijednosti>*[velicina]();
        brElem = 0; kapacitet = velicina;
    }
    NizMapa(const NizMapa& mapa) {
        defaultVrijednost = TipVrijednosti();
        this->elementi = new std::pair<TipKljuca, TipVrijednosti>*[mapa.kapacitet]();
        this->brElem = mapa.brElem;
        this->kapacitet = mapa.kapacitet;
        for (int i = 0; i < brElem; i++) {
            elementi[i] = new std::pair<TipKljuca, TipVrijednosti>(*(mapa.elementi[i]));
        }
    }
    NizMapa(NizMapa &&mapa) {
        defaultVrijednost = TipVrijednosti();
        this->brElem = mapa.brElem;
        this->kapacitet = mapa.kapacitet;
        this->elementi = mapa.elementi;
        mapa.elementi = nullptr;
        mapa.brElem = 0;
        mapa.kapacitet = 0;
    }
    NizMapa<TipKljuca, TipVrijednosti> &operator =(const NizMapa<TipKljuca, TipVrijednosti>& mapa) {
        if(this != &mapa) {
            //destruktor
            for (int i = 0; i < brElem; i++) {
            delete elementi[i];
            }
            delete[] elementi;
            elementi = nullptr;
            //konstruktor kopije
            this->elementi = new std::pair<TipKljuca, TipVrijednosti>*[mapa.kapacitet]();
            this->brElem = mapa.brElem;
            this->kapacitet = mapa.kapacitet;
            for (int i = 0; i < brElem; i++) {
                elementi[i] = new std::pair<TipKljuca, TipVrijednosti>(*(mapa.elementi[i]));
            }
        }
        return *this;
    }
    NizMapa<TipKljuca, TipVrijednosti> &operator =(NizMapa<TipKljuca, TipVrijednosti>&& mapa) {
        if(this != &mapa) {
            std::swap(elementi, mapa.elementi);
            std::swap(brElem, mapa.brElem);
            std::swap(kapacitet, mapa.kapacitet);
        }
        return *this;
    }
    ~NizMapa() {
        for (int i = 0; i < brElem; i++) {
            delete elementi[i];
        }
        delete[] elementi;
        elementi = nullptr;
    }
    void provjeriKapacitet() {
        if(!kapacitet) {
            elementi = new std::pair<TipKljuca, TipVrijednosti>*[1000];
            kapacitet = 1000;
            brElem = 0;
        }
        else if(brElem == kapacitet) {
            if(!kapacitet) kapacitet = 500;
            std::pair<TipKljuca, TipVrijednosti>** temp = new std::pair<TipKljuca, TipVrijednosti>*[kapacitet*2];
            
            for (int i = 0; i < kapacitet; i++) {
                temp[i] = elementi[i];
            }
            delete[] elementi;
            elementi = temp;
            //temp = nullptr;
            kapacitet *= 2;
        }
    }
    
    
    int brojElemenata() const {
        return brElem;
    }
    void obrisi() override {
        for (int i = 0; i < brElem; i++)  {
            delete elementi[i];
        }
        delete[] elementi;
        elementi = nullptr;
        brElem = 0;
        kapacitet = 0;
    }
    void obrisi(const TipKljuca& kljuc) override {
        for (int i = 0; i < brElem; i++) {
            if(elementi[i]->first == kljuc) {
                delete elementi[i];
                for (int j = i; j < brElem-1; j++) {
                    elementi[j] = elementi[j+1];
                }
                brElem--;
                return;
            }
        }
        throw;
    }
    TipVrijednosti& operator[](TipKljuca index) {
        for (int i = 0; i < brElem; i++) {
            //std::cout << i+1 << elementi[i]->first << " index" << index <<std::endl;
            if(elementi[i]->first == index) {
                return elementi[i]->second;
            }
        }
        provjeriKapacitet();
        elementi[brElem] = new std::pair<TipKljuca, TipVrijednosti>();
        elementi[brElem]->first = index;
        brElem++;
        return elementi[brElem-1]->second;
    }
    const TipVrijednosti& operator[](TipKljuca index) const {
        for (int i = 0; i < brElem; i++) {
            if(elementi[i]->first == index) {
                return elementi[i]->second;
            }
        }
        return defaultVrijednost;
    }
};













template<typename tipKljuca, typename tipVrijednost>
class BinStabloMapa: public Mapa<tipKljuca, tipVrijednost> {
    struct Cvor {
        tipKljuca kljuc;
        tipVrijednost vrijednost;
        Cvor *lijevo, *desno, *roditelj;
    };
    Cvor* groot;
    int brElem;
    tipVrijednost tempVrijednost;
    Cvor* trazi(const tipKljuca& kljuc, Cvor* c) const {
        if(!c) {
            return nullptr;
        }
        if(kljuc == c->kljuc) {
            return c;
        }
        if(kljuc < c->kljuc) {
            return trazi(kljuc, c->lijevo);
        } else {
            return trazi(kljuc, c->desno);
        }
    }
    
    Cvor* dodaj(const tipKljuca kljuc, const tipVrijednost& vrijednost, Cvor*& c, Cvor* roditelj) {
        if(!c) {
            c = new Cvor();
            c->kljuc = kljuc;
            c->vrijednost = vrijednost;
            c->lijevo = c->desno = nullptr;
            c->roditelj = roditelj;
            brElem++;
            return c;
        }
        if(kljuc == c->kljuc) {
            return c;
        }
        if(kljuc < c->kljuc) {
            //roditelj postaje trenutni tj. c
            return dodaj(kljuc, vrijednost, c->lijevo, c);
        } else {
            return dodaj(kljuc, vrijednost, c->desno, c);
        }
    }
    void deleteAll(Cvor *c) {
        if(!c){
          return;  
        } 
        deleteAll(c->desno);
        deleteAll(c->lijevo);
        delete c;
        brElem--;
    }
    void copy(Cvor*& ovaj, Cvor* drugi, Cvor* parent=nullptr) {
        if(!drugi) {
            return;
        }
        ovaj = new Cvor;
        ovaj->kljuc = drugi->kljuc;
        ovaj->vrijednost = drugi->vrijednost;
        ovaj->lijevo = drugi->lijevo;
        ovaj->desno = drugi->desno;
        ovaj->roditelj = parent;
        copy(ovaj->desno, drugi->desno, ovaj);
        copy(ovaj->lijevo, drugi->lijevo, ovaj);
        brElem++;
    }
    public:
    BinStabloMapa() {
        tempVrijednost = tipVrijednost();
        groot = nullptr;
        brElem = 0;
    }
    BinStabloMapa(const BinStabloMapa<tipKljuca, tipVrijednost>& s) {
        tempVrijednost = tipVrijednost();
        groot = nullptr;
        brElem = 0;
        copy(groot, s.groot, nullptr);
    }
    BinStabloMapa(BinStabloMapa<tipKljuca, tipVrijednost>&& s) {
        std::swap(groot, s.groot);
        std::swap(brElem, s.brElem);
        tempVrijednost = tipVrijednost();
    }
    ~BinStabloMapa() {
        deleteAll(groot);
    };
    int brojElemenata() const override {
        return brElem;
    }
    void obrisi() override {
         deleteAll(groot);
         groot = nullptr;
    }
    private:
    void deleteHelper(Cvor* c) {
        if(!c) {
            return;
        }
        //jedno dijete sa xor
        if((bool)(c->desno) ^ (bool)(c->lijevo)) {
            Cvor* dijete = c->desno;
            if(!dijete) {
                dijete = c->lijevo;
            }
            
            if(c->roditelj) {
                if(c->roditelj->desno == c) {
                    c->roditelj->desno = dijete;
                } else {
                    c->roditelj->lijevo = dijete;
                }    
                dijete->roditelj = c->roditelj;
            }
            
            if(groot == c) {
                Cvor* temp = c->lijevo;
                if(temp) {
                    while(temp->desno) {
                        temp = temp->desno;
                    }
                    groot->kljuc = temp->kljuc;
                    groot->vrijednost = temp->vrijednost;
                    deleteHelper(temp);
                    return;
                }
                temp = c->desno;
                if(temp) {
                    temp = c->desno;
                    while(temp->lijevo) {
                        temp = temp->lijevo;
                    }
                    groot->kljuc = temp->kljuc;
                    groot->vrijednost = temp->vrijednost;
                    deleteHelper(temp);
                    return;
                } else {
                    std::cout << "praznp";
                    delete c;
                    groot = nullptr;
                    brElem--;
                    return;
                }
                
            }
            delete c;
            brElem = brElem - 1;
            return;
        }
        //nema djece 
        if(!(c->lijevo) && !(c->desno)) {
            if(c->roditelj) {
                if(c->roditelj->desno == c) {
                    c->roditelj->desno = nullptr;
                } else {
                    c->roditelj->lijevo = nullptr;
                }
            }
            if(groot == c) {
                groot = nullptr;
            }
            delete c;
            brElem = brElem - 1;
            return;
        } 
        //dvoje djece
        if(c->desno && c->lijevo) {
            Cvor* temp = c->lijevo;
            while(temp->desno) {
                temp = temp->desno;
            }
            c->kljuc = temp->kljuc;
            c->vrijednost = temp->vrijednost;
            deleteHelper(temp);
            return;    
        } 
    }
    public:
    void obrisi(const tipKljuca& kljuc) override {
        Cvor* c = trazi(kljuc, groot);
        deleteHelper(c);
        //brElem = 0;
    }
    
    BinStabloMapa<tipKljuca, tipVrijednost> &operator =(const BinStabloMapa<tipKljuca, tipVrijednost>& s) {
        if(&s != this) {
            deleteAll(groot);
            groot = nullptr;
            copy(groot, s.groot, nullptr);
        }
        return *this;
    }
    
    BinStabloMapa<tipKljuca, tipVrijednost> &operator =(BinStabloMapa<tipKljuca, tipVrijednost>&& s) {
        if(&s != this) {
            std::swap(groot, s.groot);
            std::swap(brElem, s.brElem);
        }
        return *this;
    }
    
    tipVrijednost& operator[](tipKljuca index) override {
        Cvor* temp = dodaj(index, tipVrijednost(), groot, nullptr);
        return temp->vrijednost;
    }
    const tipVrijednost& operator[](tipKljuca index) const override {
        Cvor* temp = trazi(index, groot);
        if(!temp) {
            return tempVrijednost;
        } else {
            return temp->vrijednost;
        }
    }
};

using namespace std;
int main() {
    srand (time(NULL));
    BinStabloMapa<int, int> s;
    NizMapa<int, int> m;
    int niz[1000];
    for (int i = 0; i < 1000; i++) {
        niz[i] = rand();
    }
    clock_t vrijeme1 = clock();
    for (int i = 0; i < 1000; i++) {
        s[i] = niz[i];
    }
    clock_t vrijeme2 = clock();
    int ukvrijeme1 = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Ukupno vrijeme dodavanja elemenata za binarno stablo je " << ukvrijeme1 << " [ms].\n";
    //79 ms
    
    
    clock_t vrijeme3 = clock();
    for (int i = 0; i < 1000; i++) {
        m[i] = niz[i];
    }
    clock_t vrijeme4 = clock();
    int ukvrijeme2 = (vrijeme4 - vrijeme3) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Ukupno vrijeme dodavanja elemenata za nizMapu je " << ukvrijeme2 << " [ms].\n";
    //20 ms
    
    //Vrijeme potrebno za dodavanje elemenata proporcijalno raste broju vec postjecih elemenata
    //O(n) postaje logn ukoliko koristimo stablo umjesto niza
    
    
    vrijeme1 = clock();
    for (int i = 0; i < 1000; i++) {
        s[i] = 0;
    }
    vrijeme2 = clock();
    ukvrijeme1 = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Ukupno vrijeme pristupanja elementima za binarno stablo je " << ukvrijeme1 << " [ms].\n";
    //75 ms
    
    
    
    vrijeme3 = clock();
    for (int i = 0; i < 1000; i++) {
        m[i] = niz[i];
    }
    vrijeme4 = clock();
    ukvrijeme2 = (vrijeme4 - vrijeme3) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Ukupno vrijeme  pristupanja elementima za nizMapu je " << ukvrijeme2 << " [ms].\n";
    //16 ms
    //Ista stvar se desava i pri pristupanju elemenata. Kod nizMape je O(n) 
    //jer je potreno proci kroz cijeli niz u najgorem slucaju dok je kod stabla logn jer se polovi svaki put
    
return 0;
}

