#include <iostream>
#include <vector>
#include <queue>

template <typename TipOznake>
class Grana;

template <typename TipOznake>
class Cvor;

template <typename TipOznake>
class GranaIterator;




template <typename TipOznake>
class UsmjereniGraf {
    
public:
    UsmjereniGraf(int brojCvorova){}
    virtual ~UsmjereniGraf(){}
    virtual int dajBrojCvorova() const = 0;
    virtual void postaviBrojCvorova(int brojCvorova) = 0;
    virtual void dodajGranu(int polazni, int dolazni, float tezina) = 0;
    virtual void obrisiGranu(int polazni, int dolazni) = 0;
    virtual void postaviTezinuGrane(int polazni, int dolazni, float tezina) = 0;
    virtual float dajTezinuGrane(int polazni, int dolazni) = 0;
    virtual bool postojiGrana(int polazni, int dolazni) = 0;
    
    virtual void postaviOznakuCvora(int cvor, TipOznake oznaka) = 0;
    virtual TipOznake dajOznakuCvora(int cvor) const = 0;
    
    virtual void postaviOznakuGrane(int polazni, int dolazni, TipOznake oznaka) = 0;
    virtual TipOznake dajOznakuGrane(int polazni, int dolazni) const = 0;
    
    virtual Grana<TipOznake> dajGranu(int polazni, int dolazni) {
        auto graf = this;
        return  Grana<TipOznake>(graf, polazni, dolazni);
    }
    virtual Cvor<TipOznake> dajCvor(int cvor) {
        auto graf = this;
        return  Cvor<TipOznake>(graf, cvor);
    }
    virtual GranaIterator<TipOznake> dajGranePocetak() = 0;
    virtual GranaIterator<TipOznake> dajGraneKraj() = 0;
    
    virtual GranaIterator<TipOznake> dajSljedecuGranu(int polazni, int dolazni) = 0;
};


template <typename TipOznake>
class Grana {
    UsmjereniGraf<TipOznake>* graf;
    int polazni, dolazni;
    public:
    Grana(UsmjereniGraf<TipOznake>* graf, int polazni, int dolazni) {
        this->graf = graf;
        this->polazni = polazni;
        this->dolazni = dolazni;
    }
    float dajTezinu() const {
        return graf->dajTezinuGrane(polazni, dolazni);
    }
    void postaviTezinu(float tezina) {
        graf->postaviTezinuGrane(polazni, dolazni, tezina);
    }
    TipOznake dajOznaku() const {
        return graf->dajOznakuGrane(polazni, dolazni);
    }
    void postaviOznaku(TipOznake oznaka) {
        graf->postaviOznakuGrane(polazni, dolazni, oznaka);
    }
    Cvor<TipOznake> dajPolazniCvor() const {
        return graf->dajCvor(polazni);
    }
    Cvor<TipOznake> dajDolazniCvor() const {
        return graf->dajCvor(dolazni);
    }
    
};

template <typename TipOznake>
class Cvor {
    UsmjereniGraf<TipOznake>* graf;
    int redniBroj;
    public:
    Cvor(UsmjereniGraf<TipOznake>* graf, int redniBroj) {
        this->graf = graf;
        this->redniBroj = redniBroj;
    }
    TipOznake dajOznaku() const {
        return graf->dajOznakuCvora(redniBroj);
    }
    void postaviOznaku(TipOznake oznaka) {
        graf->postaviOznakuCvora(redniBroj, oznaka);
    }
    int dajRedniBroj() const {
        return redniBroj;
    }
    
    
};



template <typename TipOznake>
class MatricaGraf: public UsmjereniGraf<TipOznake> {
    struct PodaciGrane {
        TipOznake oznaka;
        float tezina;
        bool postoji;
    };
    std::vector<std::vector<PodaciGrane>> matrica;
    std::vector<TipOznake> oznakeCvorova;
    PodaciGrane fake;
    void testIndex(int polazni, int dolazni) const {
        int size;
        size = matrica.size();
        if(polazni > size - 1 || dolazni > size - 1) {
            throw std::out_of_range("Nevalidan indeks");
        }
    }
public:
    MatricaGraf(int brojCvorova): UsmjereniGraf<TipOznake>(brojCvorova) {
        fake.postoji = false;
        matrica.resize(brojCvorova);
        for (int i = 0; i < matrica.size(); i++) {
            matrica.at(i).resize(brojCvorova, fake);
        }
        oznakeCvorova.resize(brojCvorova);
    }
    ~MatricaGraf() {}
    int dajBrojCvorova() const {
        return matrica.size();
    }
    void postaviBrojCvorova(int brojCvorova) override {
        if(matrica.size() == brojCvorova) {
            return;
        }
        if(matrica.size() > brojCvorova ) {
            throw "belaj";
        }
        
        for (int i = 0; i < matrica.size(); i++) {
            matrica.at(i).resize(brojCvorova, fake);
        }
        std::vector<PodaciGrane> temp;
        temp.resize(brojCvorova);
        for (int i = 0; i < temp.size(); i++) {
            temp[i] = fake;
        }
        matrica.resize(brojCvorova, temp);
        
        
    }
    void dodajGranu(int polazni, int dolazni, float tezina) override {
        testIndex(polazni, dolazni);
        PodaciGrane temp;
        temp.tezina = tezina;
        temp.postoji = true;
        matrica.at(polazni).at(dolazni) = temp;
    }
    void obrisiGranu(int polazni, int dolazni) override {
        testIndex(polazni, dolazni);
        matrica.at(polazni).at(dolazni).postoji = !true;
    }
    void postaviTezinuGrane(int polazni, int dolazni, float tezina) override {
        testIndex(polazni, dolazni);
        matrica.at(polazni).at(dolazni).tezina = tezina;
    }
    float dajTezinuGrane(int polazni, int dolazni) override {
        testIndex(polazni, dolazni);
        return matrica.at(polazni).at(dolazni).tezina;
    }
    bool postojiGrana(int polazni, int dolazni) override {
        testIndex(polazni, dolazni);
        return matrica.at(polazni).at(dolazni).postoji;
    }
    
    void postaviOznakuCvora(int cvor, TipOznake oznaka) override {
        oznakeCvorova.at(cvor) = oznaka;
    }
    TipOznake dajOznakuCvora(int cvor) const override {
        return oznakeCvorova.at(cvor);
    }
    
    void postaviOznakuGrane(int polazni, int dolazni, TipOznake oznaka) override {
        testIndex(polazni, dolazni);
        matrica.at(polazni).at(dolazni).oznaka = oznaka;
    }
    TipOznake dajOznakuGrane(int polazni, int dolazni) const override {
        testIndex(polazni, dolazni);
        return matrica.at(polazni).at(dolazni).oznaka;
    }
    
    virtual GranaIterator<TipOznake> dajSljedecuGranu(int polazni, int dolazni) override {
        for (int i = polazni; i < matrica.size(); i++) {
            for (int j = 0; j < matrica.size(); j++) {
                if(j <= dolazni && i == polazni) continue;
                if(matrica.at(i).at(j).postoji) {
                    return GranaIterator<TipOznake>(this, i, j);
                }
            }
        }
        return GranaIterator<TipOznake>(this, -1, -1);
    }
    
    GranaIterator<TipOznake> dajGranePocetak() override {
        GranaIterator<TipOznake> it(this, 0, -1);
        return ++it;
    }
    GranaIterator<TipOznake> dajGraneKraj() override {
        GranaIterator<TipOznake> it(this, -1, -1);
        return it;
    }
};
template <typename TipOznake>
class GranaIterator {
    
    UsmjereniGraf<TipOznake>* graf;
    int polazni, dolazni;
    public:
    GranaIterator(UsmjereniGraf<TipOznake>* graf, int polazni, int dolazni) {
        this->graf = graf;
        this->polazni = polazni;
        this->dolazni = dolazni;
    }
    GranaIterator() {}
    Grana<TipOznake> operator*() {
        return Grana<TipOznake>(graf, polazni, dolazni);
    }
    bool operator==(const GranaIterator &iter) const {
        bool temp = (dolazni == iter.dolazni);
        temp = temp && (polazni == iter.polazni);
        temp = temp && (graf == iter.graf);
        return temp;
    }
    bool operator!=(const GranaIterator &iter) const {
        bool temp = (dolazni == iter.dolazni);
        temp = temp && (polazni == iter.polazni);
        temp = temp && (graf == iter.graf);
        return !temp;
    }
    GranaIterator& operator++() {
        if(polazni == -1 && dolazni == -1) {
            throw std::out_of_range("Nevalidan indeks");
        }
        GranaIterator<TipOznake> g;
        g = graf->dajSljedecuGranu(polazni, dolazni);
        this->polazni = g.polazni;
        this->dolazni = g.dolazni;
        return *this;
    }
    GranaIterator operator++(int) {
        GranaIterator temp(*this);
        ++(*this);
        return temp;
    }
    
    template<typename T>
    friend class Grana;
};



template <typename TipOznake>
void dfs(UsmjereniGraf<TipOznake> *graf, std::vector<Cvor<TipOznake>> &dfs_obilazak, Cvor<TipOznake> cvor) {
    bool flag = false;
    int broj1, broj2 = cvor.dajRedniBroj();
    for (int i(dfs_obilazak.size()-1); i >=0; i--) {
        broj1 = dfs_obilazak.at(i).dajRedniBroj();
        if (broj1 == broj2) {
            flag = true;
            break;
        }
    }
    if (!flag) {
        dfs_obilazak.push_back(cvor);
    }
    int brCvorova = graf->dajBrojCvorova(); 
    for(int i(0); i < brCvorova; i++) {
        if (graf->postojiGrana(broj2, i) && !flag) {
            dfs(graf, dfs_obilazak, graf->dajCvor(i));
        }
    }
}

template <typename TipOznake>
void bfs(UsmjereniGraf<TipOznake> *graf, std::vector<Cvor<TipOznake> > &bfs_obilazak, Cvor<TipOznake> pocetni_cvor) {
    bfs_obilazak.push_back(pocetni_cvor);
    std::queue<Cvor<TipOznake>> red;
    red.push(pocetni_cvor);
    int brojCvorova = graf->dajBrojCvorova();
    int bfsVel = bfs_obilazak.size();
    while (!red.empty() && bfsVel != brojCvorova) {
        Cvor<TipOznake> cvor = red.front();
        int redniBrojCvora = cvor.dajRedniBroj();
        red.pop();
        bool pamti = false;
        std::vector<Cvor<TipOznake>> izlazeci;
        for (int i = 0; i < brojCvorova; i++) {
            if (graf->postojiGrana(redniBrojCvora, i)) {
                Cvor<TipOznake> temp = graf->dajCvor(i);
                izlazeci.push_back(temp);
                red.push(temp);
            }
        }
        int izlazeciVel = izlazeci.size();
        for (int i = 0; i < izlazeciVel; i++) {
            pamti = false;
            for (int j = 0; j < bfsVel; j++) {
                if (izlazeci[i].dajRedniBroj() == bfs_obilazak[j].dajRedniBroj()) {
                    pamti = true;
                }
            }
            if (!pamti) {
                bfs_obilazak.push_back(izlazeci[i]);
                bfsVel = bfs_obilazak.size();
            }
        }

    }
}

using namespace std;
int main() {
    UsmjereniGraf<bool> *g = new MatricaGraf<bool>(6);
    g->dodajGranu(0, 1, 2.5);
    g->dodajGranu(1, 2, 1.2);
    g->dodajGranu(1, 3, 0.1);
    g->dodajGranu(2, 4, 3.14);
    g->dodajGranu(2, 3, 2.73);
    g->dodajGranu(3, 5, 1);
    g->dodajGranu(5, 2, -5.0);
    std::vector<Cvor<bool> > bfs_obilazak;
    bfs(g, bfs_obilazak, g->dajCvor(0));
    for (int i = 0; i < bfs_obilazak.size(); i++)
      cout << bfs_obilazak[i].dajRedniBroj() << ",";
  delete g;
    return 0;
}
