#include <iostream>
#include <utility> 
#include <time.h>
#include <stdlib.h>
#include <algorithm>


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
                    //std::cout << "praznp";
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








//AVL 
template<typename tipKljuca, typename tipVrijednost>
class AVLStabloMapa: public Mapa<tipKljuca, tipVrijednost> {
    struct Cvor {
        tipKljuca kljuc;
        tipVrijednost vrijednost;
        Cvor *lijevo, *desno, *roditelj;
        int balans = 0;
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
        ovaj->balans = drugi->balans;
        ovaj->roditelj = parent;
        copy(ovaj->desno, drugi->desno, ovaj);
        copy(ovaj->lijevo, drugi->lijevo, ovaj);
        brElem++;
    }
    int balans(Cvor* c) const{
        if(c) return c->balans;
        else return 0;
    }
    int razlika(Cvor* c) const{
        if(c) return balans(c->lijevo) - balans(c->desno);
        else return 0;
    }
    
    Cvor* desnaRotacija(Cvor* &c) {
        Cvor *child = c->lijevo;
        Cvor *grandChild = (c->lijevo)->desno;
        
        child->roditelj = c->roditelj;
        c->roditelj = child;
        if(grandChild) grandChild->roditelj = child;
        
        c->lijevo = grandChild;
        child->desno = c;
        
        int clb = balans(c->lijevo);
        int cdb = balans(c->desno);
        if(clb > cdb) {
            c->balans = clb;
        } else {
            c->balans = cdb;
        }
        
        int ylb = balans(child->lijevo);
        int ydb = balans(child->desno);
        if(ylb > ydb) {
            child->balans = ylb;
        } else {
            child->balans = ydb;
        }
        
        if(child->roditelj) {
            (child->roditelj)->desno = child;
        } else {
            groot = child;
        }
        return child;
    }
    
    Cvor* lijevaRotacija(Cvor* &c) {
        Cvor *child = c->desno;
        Cvor *grandChild = (c->desno)->lijevo;
        
        child->roditelj = c->roditelj;
        c->roditelj = child;
        if(grandChild) grandChild->roditelj = child;
        
        c->desno = grandChild;
        child->lijevo = c;
        
        int clb = balans(c->lijevo);
        int cdb = balans(c->desno);
        if(clb > cdb) {
            c->balans = clb;
        } else {
            c->balans = cdb;
        }
        
        int ylb = balans(child->lijevo);
        int ydb = balans(child->desno);
        if(ylb > ydb) {
            child->balans = ylb;
        } else {
            child->balans = ydb;
        }
        
        if(child->roditelj) {
            (child->roditelj)->desno = child;
        } else {
            groot = child;
        }
        return c;
    }
    
    void balsirajStablo(Cvor* &c) {
        Cvor* tempParent = c->roditelj;
        if(c && tempParent) {
            if(tempParent->desno == c) {
                tempParent->balans = tempParent->balans - 1;
            } else {
                tempParent->balans = tempParent->balans + 1;
            }
            
            int flag = 0;
            if(c->balans > 1 && tempParent->lijevo->kljuc > tempParent->kljuc) {
                desnaRotacija(tempParent);
                flag++;
            }
            
            if(c->balans < -1 && tempParent->desno->kljuc < tempParent->kljuc) {
                lijevaRotacija(tempParent);
                flag++;
            }
            
            if(c->balans > 1 && tempParent->lijevo->kljuc < tempParent->kljuc) {
                tempParent->lijevo = lijevaRotacija(tempParent->lijevo);
                desnaRotacija(tempParent);
                flag++;
            }
            
            if(c->balans < -1 && tempParent->desno->kljuc > tempParent->kljuc) {
                tempParent->desno = lijevaRotacija(tempParent->desno);
                lijevaRotacija(tempParent);
                flag++;
            }
            
            if(flag > 0) {
                balsirajStablo(c);
            } else {
                return;
            }
            
            
        } else {
            return;
        }
        
    }
    public:
    AVLStabloMapa() {
        tempVrijednost = tipVrijednost();
        groot = nullptr;
        brElem = 0;
    }
    AVLStabloMapa(const AVLStabloMapa<tipKljuca, tipVrijednost>& s) {
        tempVrijednost = tipVrijednost();
        groot = nullptr;
        brElem = 0;
        copy(groot, s.groot, nullptr);
    }
    AVLStabloMapa(AVLStabloMapa<tipKljuca, tipVrijednost>&& s) {
        std::swap(groot, s.groot);
        std::swap(brElem, s.brElem);
        tempVrijednost = tipVrijednost();
    }
    ~AVLStabloMapa() {
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
                    //std::cout << "praznp";
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
    
    AVLStabloMapa<tipKljuca, tipVrijednost> &operator =(const AVLStabloMapa<tipKljuca, tipVrijednost>& s) {
        if(&s != this) {
            deleteAll(groot);
            groot = nullptr;
            copy(groot, s.groot, nullptr);
        }
        return *this;
    }
    
    AVLStabloMapa<tipKljuca, tipVrijednost> &operator =(AVLStabloMapa<tipKljuca, tipVrijednost>&& s) {
        if(&s != this) {
            std::swap(groot, s.groot);
            std::swap(brElem, s.brElem);
        }
        return *this;
    }
    
    tipVrijednost& operator[](tipKljuca index) override {
        Cvor* temp = dodaj(index, tipVrijednost(), groot, nullptr);
        Cvor* tempParent = temp->roditelj;
        if(tempParent) {
            if(tempParent->desno && tempParent->lijevo) {
                tempParent->balans = 0;
            } else {
                balsirajStablo(temp);
            }
        }
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
    
    AVLStabloMapa <int, int> aa;
    int niz[] = {22, 34, 41, 26, 25, 48, 53, 29, 24, 38};
    for (int i = 0; i < 10; i++) {
        aa[i] = niz[i];
    }
    std::sort(&niz[0], &niz[10]);
    for (int i = 0; i < 10; i++) {
        std::cout << niz[i] << " ";
    }
    return 0;
}
