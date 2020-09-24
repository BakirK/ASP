#include <iostream>
#include <utility> 


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
            elementi[i] = new std::pair<TipKljuca, TipVrijednosti>((mapa.elementi[i]));
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

void test1() {
    NizMapa<int, int> m;
    if(m.brojElemenata() != 0) throw;
    for (int i = 0; i < 10; i++) {
        m[i] = i;
    }
    if(m.brojElemenata() != 10) throw;
    NizMapa<int, int> m2(m);
    for (int i = 0; i < i; i++) {
        if(m2[i] != m[i]) throw;
    }
    m.obrisi();
    if(m.brojElemenata() != 0) throw;
    if(m2.brojElemenata() != 10) throw;
    std::cout << "test1 ok\n"; 
}

NizMapa<std::string, std::string> copyConstructor() {
    NizMapa<std::string, std::string> m(10);
    m["RS"] = "vrtic";
    m["RI"] = "kurs";
    m["AIE"] = "kompleks";
    m["TK"] = "oprat ruke";
    return m;
}

void test2() {
    NizMapa<std::string, std::string> m = copyConstructor();
    if(m.brojElemenata() != 4) throw;
    if(m["RS"] != "vrtic") throw;
    if(m["RI"] != "kurs") throw;
    if(m["AIE"] != "kompleks") throw;
    if(m["TK"] != "oprat ruke") throw;
    
    //move
    NizMapa<std::string, std::string> m2 = std::move(m);
    if(m.brojElemenata() != 0) throw;
    if(m2.brojElemenata() != 4) throw;
    if(m2["RS"] != "vrtic") throw;
    if(m2["RI"] != "kurs") throw;
    if(m2["AIE"] != "kompleks") throw;
    if(m2["TK"] != "oprat ruke") throw;
    std::cout << "test2 ok\n"; 
}

void test3() {
    const NizMapa<std::string, std::string> m(copyConstructor());
    if(m.brojElemenata() != 4) throw;
    if(m["benten"] != "") throw;
    //nije se kreirao benten
    if(m.brojElemenata() != 4) throw;
    std::cout << "test3 ok\n"; 
}

void test4() {
    NizMapa<int, std::string> m;
    m[0] = "Waikiki";
    m[1] = "LC";
    m[2] = "znaci";
    m[3] = "low casual";
    if(m.brojElemenata() != 4) throw;
    //brisanje prvog
    m.obrisi(0);
    if(m.brojElemenata() != 3) throw;
    if(m[1] != "LC") throw;
    if(m[3] != "low casual") throw;
    //brisanje zadnjeg
    m.obrisi(3);
    if(m.brojElemenata() != 2) throw;
    if(m[1] != "LC") throw;
    if(m[2] != "znaci") throw;
    if(m[3] != "") throw;
    std::cout << "test4 ok\n"; 
}

int main() {
    test1();
    test2();
    test3();
    test4();
    return 0;
}
