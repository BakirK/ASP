#include <iostream>
#include <utility> 
#include <time.h>
#include <stdlib.h>
using namespace std;



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
        brElem = 0; 
        kapacitet = velicina;
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













template<typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa: public Mapa<TipKljuca, TipVrijednosti> {
    struct Cvor {
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor *lijevo, *desno, *roditelj;
    };
    Cvor* groot;
    int brElem;
    TipVrijednosti tempVrijednost;
    Cvor* trazi(const TipKljuca& kljuc, Cvor* c) const {
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
    
    Cvor* dodaj(const TipKljuca kljuc, const TipVrijednosti& vrijednost, Cvor*& c, Cvor* roditelj) {
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
        tempVrijednost = TipVrijednosti();
        groot = nullptr;
        brElem = 0;
    }
    BinStabloMapa(const BinStabloMapa<TipKljuca, TipVrijednosti>& s) {
        tempVrijednost = TipVrijednosti();
        groot = nullptr;
        brElem = 0;
        copy(groot, s.groot, nullptr);
    }
    BinStabloMapa(BinStabloMapa<TipKljuca, TipVrijednosti>&& s) {
        std::swap(groot, s.groot);
        std::swap(brElem, s.brElem);
        tempVrijednost = TipVrijednosti();
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
    void obrisi(const TipKljuca& kljuc) override {
        Cvor* c = trazi(kljuc, groot);
        deleteHelper(c);
        //brElem = 0;
    }
    
    BinStabloMapa<TipKljuca, TipVrijednosti> &operator =(const BinStabloMapa<TipKljuca, TipVrijednosti>& s) {
        if(&s != this) {
            deleteAll(groot);
            groot = nullptr;
            copy(groot, s.groot, nullptr);
        }
        return *this;
    }
    
    BinStabloMapa<TipKljuca, TipVrijednosti> &operator =(BinStabloMapa<TipKljuca, TipVrijednosti>&& s) {
        if(&s != this) {
            std::swap(groot, s.groot);
            std::swap(brElem, s.brElem);
        }
        return *this;
    }
    
    TipVrijednosti& operator[](TipKljuca index) override {
        Cvor* temp = dodaj(index, TipVrijednosti(), groot, nullptr);
        return temp->vrijednost;
    }
    const TipVrijednosti& operator[](TipKljuca index) const override {
        Cvor* temp = trazi(index, groot);
        if(!temp) {
            return tempVrijednost;
        } else {
            return temp->vrijednost;
        }
    }
};


template<typename TipKljuca, typename TipVrijednosti>
class HashMapa: public Mapa<TipKljuca, TipVrijednosti> {
    unsigned int brElem, kapacitet;
    unsigned int (*hash)(TipKljuca k, unsigned int max) = nullptr;
    std::pair<TipKljuca, TipVrijednosti>** elementi;
    TipVrijednosti defaultVrijednost;
    std::pair<TipKljuca, TipVrijednosti>* DEL;
    void provjeriKapacitet() {
        if(!kapacitet) {
            elementi = new std::pair<TipKljuca, TipVrijednosti>*[1000]();
            kapacitet = 1000;
            brElem = 0;
        }
        else if(brElem == kapacitet) {
            bool flag = true;
            if(!kapacitet) {
                kapacitet = 500;
                flag = false;
            }
                
            std::pair<TipKljuca, TipVrijednosti>** temp = new std::pair<TipKljuca, TipVrijednosti>*[kapacitet*2]();
            
            for (int i = 0; i < kapacitet && flag; i++) {
                temp[i] = elementi[i];
            }
            
            delete[] elementi;
            elementi = temp;
            //temp = nullptr;
            kapacitet *= 2;
        }
    }
    void provjeriHash() const {
        if(!hash) throw std::domain_error("Nije postavljena hash fuckcija");
    }
    void provjeriBrojElemenata() const {
        if(!brElem) throw std::domain_error("HashMapa je prazna");
    }
    public:
    HashMapa(int velicina = 1000) {
        defaultVrijednost = TipVrijednosti();
        elementi = new std::pair<TipKljuca, TipVrijednosti>*[velicina]();
        for (int i = 0; i < velicina; i++) {
            elementi[i] = nullptr;
        }
        brElem = 0; 
        kapacitet = velicina;
        DEL = new std::pair<TipKljuca, TipVrijednosti>();
    }
    HashMapa(const HashMapa<TipKljuca, TipVrijednosti> &mapa) {
        defaultVrijednost = TipVrijednosti();
        this->elementi = new std::pair<TipKljuca, TipVrijednosti>*[mapa.kapacitet]();
        this->brElem = mapa.brElem;
        this->kapacitet = mapa.kapacitet;
        this->hash = mapa.hash;
        for (int i = 0; i < kapacitet; i++) {
            if(mapa.elementi[i]) {
                elementi[i] = new std::pair<TipKljuca, TipVrijednosti>(*(mapa.elementi[i]));
            } else {
                elementi[i] = nullptr;
            }
        }
        DEL = new std::pair<TipKljuca, TipVrijednosti>();
    }
    HashMapa(HashMapa<TipKljuca, TipVrijednosti> &&mapa) {
        this->hash = mapa.hash;
        defaultVrijednost = TipVrijednosti();
        this->brElem = mapa.brElem;
        this->kapacitet = mapa.kapacitet;
        this->elementi = mapa.elementi;
        mapa.elementi = nullptr;
        mapa.brElem = 0;
        mapa.kapacitet = 0;
        DEL = new std::pair<TipKljuca, TipVrijednosti>();
    }
    ~HashMapa() {
        obrisi();
        delete DEL;
    }
    void definisiHashFunkciju(unsigned int (*in)(TipKljuca, unsigned int)) {
        hash = in;
    }
    int brojElemenata() const {
        return brElem;
    }
    void obrisi() override {
        for (int i = 0; i < kapacitet; i++)  {
            if(elementi[i] != DEL) {
                delete elementi[i];    
                elementi[i] = nullptr;
            }
        }
        delete[] elementi;
        elementi = nullptr;
        brElem = 0;
        kapacitet = 0;
    }
    void obrisi(const TipKljuca& kljuc) override {
        provjeriBrojElemenata();
        provjeriHash();
        unsigned int k = hash(kljuc, kapacitet);
        bool flag = false;
        unsigned int index;
        for (int i = k; i < kapacitet; i++) {
            if(elementi[i] && elementi[i]->first == kljuc) {
                index = i;
                flag = true;
                break;
            }
        }
        for (int i = 0; i < k && !flag; i++) {
            if(elementi[i] && elementi[i]->first == kljuc) {
                index = i;
                flag = true;
                break;
            }
        }
        if(!flag) {
            throw std::domain_error("Nije pronadjen element sa tim kljucem");
        } else {
            delete elementi[index];
            elementi[index] = DEL;
            brElem--;
        }
    }
    HashMapa<TipKljuca, TipVrijednosti> &operator =(const HashMapa<TipKljuca, TipVrijednosti>& mapa) {
        if(this != &mapa) {
            //destruktor
            obrisi();
            //konstruktor kopije
            this->elementi = new std::pair<TipKljuca, TipVrijednosti>*[mapa.kapacitet]();
            this->brElem = mapa.brElem;
            this->kapacitet = mapa.kapacitet;
            for (int i = 0; i < kapacitet; i++) {
                if(mapa.elementi[i]) {
                    elementi[i] = new std::pair<TipKljuca, TipVrijednosti>(*(mapa.elementi[i]));    
                } else {
                    elementi[i] = nullptr;
                }
            }
        }
        return *this;
    }
    HashMapa<TipKljuca, TipVrijednosti> &operator =(HashMapa<TipKljuca, TipVrijednosti>&& mapa) {
        if(this != &mapa) {
            std::swap(elementi, mapa.elementi);
            std::swap(brElem, mapa.brElem);
            std::swap(kapacitet, mapa.kapacitet);
        }
        return *this;
    }
    TipVrijednosti& operator[](TipKljuca index) override {
        provjeriHash();
        provjeriKapacitet();
        unsigned int k = hash(index, kapacitet);
        for (int i = k; i < kapacitet; i++) {
            if(elementi[i] && elementi[i]->first == index && elementi[i] != DEL) {
                return elementi[i]->second;
            }
        }
        
        for (int i = 0; i < k; i++) {
            if(elementi[i] && elementi[i]->first == index && elementi[i] != DEL) {
                return elementi[i]->second;
            }
        }
        
        
        
        
         for (int i = k; i < kapacitet; i++) {
            if(!elementi[i] || elementi[i] == DEL) {
                elementi[i] = new std::pair<TipKljuca, TipVrijednosti>();
                elementi[i]->first = index;
                brElem++;
                return elementi[i]->second;
            }
        }
        for (int i = 0; i < k; i++) {
            if(!elementi[i] || elementi[i] == DEL) {
                elementi[i] = new std::pair<TipKljuca, TipVrijednosti>();
                elementi[i]->first = index;
                brElem++;
                return elementi[i]->second;
            }
        }
        
        //da me warning nesmara
        return elementi[0]->second;
    }
    const TipVrijednosti& operator[](TipKljuca index) const override {
        this->provjeriHash();
        unsigned int k = hash(index, kapacitet);
        for (int i = k; i < kapacitet; i++) {
            if(elementi[i] && elementi[i]->first == index && elementi[i] != DEL) {
                return elementi[i]->second;
            }
        }
        for (int i = 0; i < k; i++) {
            if(elementi[i] && elementi[i]->first == index && elementi[i] != DEL) {
                return elementi[i]->second;
            }
        }
         return defaultVrijednost;
    }
    
};


unsigned int JosGlupljiHash(int ulaz, unsigned int max) {
	return max-1;
}

unsigned int mojHash(unsigned int x, unsigned int max) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x % max;
}

unsigned int basicHash(unsigned int x, unsigned int max) {
    return x % max;
}

int main() {

    

    srand (time(NULL));
    BinStabloMapa<int, int> s;
    NizMapa<int, int> m;
    HashMapa<unsigned int, int> h;
    HashMapa<int, int> h2;
    HashMapa<unsigned int, int> h3;
    h.definisiHashFunkciju(mojHash);
    int niz[4000];
    
    for (int i = 0; i < 4000; i++) {
        niz[i] = rand();
    }
    
    clock_t vrijeme1 = clock();
    for (int i = 0; i < 4000; i++) {
        s[i] = niz[i];
    }
    clock_t vrijeme2 = clock();
    int ukvrijeme1 = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Ukupno vrijeme dodavanja elemenata za binarno stablo je " << ukvrijeme1 << " [ms].\n";
    //79 ms
    
    
    clock_t vrijeme3 = clock();
    for (int i = 0; i < 4000; i++) {
        m[i] = niz[i];
    }
    clock_t vrijeme4 = clock();
    int ukvrijeme2 = (vrijeme4 - vrijeme3) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Ukupno vrijeme dodavanja elemenata za nizMapu je " << ukvrijeme2 << " [ms].\n";
    //20 ms
    
    //Vrijeme potrebno za dodavanje elemenata proporcijalno raste broju vec postjecih elemenata
    //O(n) postaje logn ukoliko koristimo stablo umjesto niza
    
    clock_t vrijeme5 = clock();
    for (int i = 0; i < 4000; i++) {
        h[i] = niz[i];
    }
    clock_t vrijeme6 = clock();
    int ukvrijeme3 = (vrijeme6 - vrijeme5) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Ukupno vrijeme dodavanja elemenata za HashMapu bez kolizija je " << ukvrijeme3 << " [ms].\n";
    //mapa bez kolizija ima priblizno duplo manje vrijeme dodavanja elemenata nego mapa sa mnogo kolizija
    //jer nije potrebno prolaziti kroz niz da bi se naslo slobodno mjesto
    
    //postavljanje hasha funkcije koja uvjek vraca isti broj
    h2.definisiHashFunkciju(JosGlupljiHash);
    vrijeme5 = clock();
    for (int i = 0; i < 4000; i++) {
        h2[i] = niz[i];
    }
    vrijeme6 = clock();
    ukvrijeme3 = (vrijeme6 - vrijeme5) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Ukupno vrijeme dodavanja elemenata za HashMapu sa kolizijama je " << ukvrijeme3 << " [ms].\n";
    //ova hash mapa uvijek ima koliziju pri svakom umetanju pa je ona najsporija jer je 
    //potrebno proci kroz sve zauzete elemennte da bi se naslo slobodno mjesto za novi element
    
    
    //nesigurni hash
    h3.definisiHashFunkciju(basicHash);
    vrijeme5 = clock();
    for (int i = 0; i < 4000; i++) {
        h3[i] = niz[i];
    }
    vrijeme6 = clock();
    ukvrijeme3 = (vrijeme6 - vrijeme5) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Ukupno vrijeme dodavanja elemenata za HashMapu sa najobicnijim (nesigurnim) hashom je " << ukvrijeme3 << " [ms].\n";
    //ova hash mapa ima hash funkciju koja je efektivno pretvara u niz mapu tako da je ona najbrza ali nije sigurna za koristenje
    
    
    
    std::cout << std::endl;
    
    vrijeme1 = clock();
    for (int i = 0; i < 4000; i++) {
        s[i] = 0;
    }
    vrijeme2 = clock();
    ukvrijeme1 = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Ukupno vrijeme pristupanja elementima za binarno stablo je " << ukvrijeme1 << " [ms].\n";
    //75 ms
    
    
    
    vrijeme3 = clock();
    for (int i = 0; i < 4000; i++) {
        m[i] = 0;
    }
    vrijeme4 = clock();
    ukvrijeme2 = (vrijeme4 - vrijeme3) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Ukupno vrijeme pristupanja elementima za nizMapu je " << ukvrijeme2 << " [ms].\n";
    //16 ms
    //Ista stvar se desava i pri pristupanju elemenata. Kod nizMape je O(n) 
    //jer je potreno proci kroz cijeli niz u najgorem slucaju dok je kod stabla logn jer se polovi svaki put
    
    
    
    
    vrijeme5 = clock();
    for (int i = 0; i < 4000; i++) {
        h[i] = 0;
    }
    vrijeme6 = clock();
    ukvrijeme3 = (vrijeme6 - vrijeme5) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Ukupno vrijeme pristupanja elementima za HashMapu bez kolizija je " << ukvrijeme3 << " [ms].\n";
    //skoro duplo brze vrijeme pristupa nego hash mapa sa kolizijom jer nije potrebno traziti
    //element nakon sto se ispostavi da je njegov hash zauzet 
    
    vrijeme5 = clock();
    for (int i = 0; i < 4000; i++) {
        h2[i] = 0;
    }
    vrijeme6 = clock();
    ukvrijeme3 = (vrijeme6 - vrijeme5) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Ukupno vrijeme pristupanja elementima za HashMapu sa kolizijama je " << ukvrijeme3 << " [ms].\n";
    //mnogo vece vrijeme pristupa elementima jer je potrebno proci kroz sve elemente prije trazenog
    //da bi se pronasao element - vrijeme pristupa raste eksponencijalno sa vecim brojem elemenata
    
    vrijeme5 = clock();
    for (int i = 0; i < 4000; i++) {
        h3[i] = 0;
    }
    vrijeme6 = clock();
    ukvrijeme3 = (vrijeme6 - vrijeme5) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Ukupno vrijeme dodavanja elemenata za HashMapu sa najobicnijim (nesigurnim) hashom je " << ukvrijeme3 << " [ms].\n";
    //najkrace vrijeme pristupa jer je hash funkcija efektivno pretvorila u niz
    //ova funkcija je najbrza ali i najmanje sigurna jer je hash kljuc jednak pravom kljucu te nema kolizija
    
    std::cout << std::endl;
    
    //BRISANJE
    //vrijeme brisanja je uvijek isto za sve klase bilo da se radi o pojedinacnom brisanju (razliak do 2ms) 
    //ili komplet brisanju svih elemenata(isto vrijeme)
    
    vrijeme5 = clock();
    s.obrisi(2000);
    vrijeme6 = clock();
    ukvrijeme3 = (vrijeme6 - vrijeme5) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Ukupno vrijeme brisanja elemenata za binarno stablo je " << ukvrijeme3 << " [ms].\n";
    
    vrijeme5 = clock();
    m.obrisi(2000);
    vrijeme6 = clock();
    ukvrijeme3 = (vrijeme6 - vrijeme5) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Ukupno vrijeme brisanja elemenata za nizMapu je " << ukvrijeme3 << " [ms].\n";
    
    vrijeme5 = clock();
    h.obrisi(2000);
    vrijeme6 = clock();
    ukvrijeme3 = (vrijeme6 - vrijeme5) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Ukupno vrijeme brisanja elemenata za HashMapu bez kolizija je " << ukvrijeme3 << " [ms].\n";
    
    vrijeme5 = clock();
    h2.obrisi(2000);
    vrijeme6 = clock();
    ukvrijeme3 = (vrijeme6 - vrijeme5) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Ukupno vrijeme brisanja elemenata za HashMapu sa kolizijama je " << ukvrijeme3 << " [ms].\n";
    
    vrijeme5 = clock();
    h3.obrisi(2000);
    vrijeme6 = clock();
    ukvrijeme3 = (vrijeme6 - vrijeme5) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Ukupno vrijeme brisanja elemenata za HashMapu sa najobicnijim (nesigurnim) hashom je " << ukvrijeme3 << " [ms].\n";
    
    return 0;
}

