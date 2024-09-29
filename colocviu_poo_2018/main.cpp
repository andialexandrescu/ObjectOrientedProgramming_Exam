#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
// model 02
using namespace std;

class Adresa{
private:
    string strada;
    int numar, sector;
public:
    Adresa();
    Adresa(const string &strada, int numar, int sector);
    ~Adresa() = default;

    void read(istream &in);
    void display(ostream &out) const;
};

Adresa::Adresa(): strada("Unknown"), numar(-1), sector(-1) {}
Adresa::Adresa(const string &strada, int numar, int sector): strada(strada), numar(numar), sector(sector) {}

void Adresa::read(istream &in)
{
    cout<<"Introduceti adresa:";
    cout<<endl<<"\tStrada: "; in>>this->strada;
    cout<<endl<<"\tNumarul: "; in>>this->numar;
    cout<<endl<<"\tSectorul: "; in>>this->sector;
}

void Adresa::display(ostream &out) const
{
    out<<"Adresa este:";
    out<<endl<<"\tstrada: "<<this->strada;
    out<<endl<<"\tsumarul: "<<this->numar;
    out<<endl<<"\tsectorul: "<<this->sector;
    out<<endl;
}

class Statie{
private:
    Adresa a;// compunere
    static int id_sufix;
    string cod_sufix;
    string cod_prefix; // difera in functie de tipul statiei
    // <cod> == <cod_prefix = tip> � <id_sufix = nr>
    string cod;
    string nume;
    vector<string> v_transport;
public:
    Statie();
    Statie(const string &strada, int numar, int sector, const string &nume, const vector<string> &v_trans);
    Statie(const Statie &other_obj);
    Statie& operator=(const Statie &other_obj);
    virtual ~Statie();

    void setCod(const string &c);
    void setCodPrefix(const string &p);
    const string getCodSufix() const;
    const string getCodPrefix() const;
    const string getCod() const;
    const string getNume() const;

    friend istream& operator>>(istream &in, Statie &obj);
    friend ostream& operator<<(ostream &out, const Statie &obj);
    virtual void read(istream &in);
    virtual void display(ostream &out) const;

    bool hasDirectLine(const Statie &obj) const;
};

int Statie::id_sufix = 0;

Statie::~Statie()
{
    Statie::id_sufix--;
}

bool Statie::hasDirectLine(const Statie &obj) const
{
    for(const auto &tra: this->v_transport)
    {
        if(this->v_transport.end() != find(obj.v_transport.begin(), obj.v_transport.end(), tra))
        {
            return true;
        }
    }
    return false;
}

void Statie::setCod(const string &c)
{
    this->cod = c;
}

void Statie::setCodPrefix(const string &p)
{
    this->cod_prefix = p;
}

const string Statie::getCodSufix() const
{
    return this->cod_sufix;
}

const string Statie::getCodPrefix() const
{
    return this->cod_prefix;
}

const string Statie::getCod() const
{
    return this->cod;
}

const string Statie::getNume() const
{
    return this->nume;
}

void Statie::read(istream &in)
{
    cout<<"Introduceti detaliile statiei:"<<endl;
    a.read(in);
    cout<<endl<<"\tnume: "; in>>this->nume;
    int n;
    cout<<endl<<"Introduceti numarul de mijloace de transport ale statiei: "; in>>n;
    v_transport.resize(n);
    for(int i = 0; i < n; i++)
    {
        in>>this->v_transport[i];
    }
    cout<<endl;
}

void Statie::display(ostream &out) const
{
    out<<"Detaliile statiei:"<<endl;
    a.display(out);
    out<<endl<<"\tnume: "<<this->nume;
    out<<endl<<"Mijloacele de transport ale statiei: ";
    for(const auto &tra: this->v_transport)
    {
        out<<tra<<" ";
    }
    out<<endl;
}

istream& operator>>(istream &in, Statie &obj)
{
    obj.read(in);
    return in;
}

ostream& operator<<(ostream &out, const Statie &obj)\
{
    obj.display(out);
    return out;
}

Statie::Statie(): a(), cod_prefix(""), nume("Undefined")
{
    Statie::id_sufix++;
    this->cod_sufix = to_string(Statie::id_sufix);
    this->v_transport.clear();
}

Statie::Statie(const string &strada, int numar, int sector, const string &nume, const vector<string> &v_trans):
    a(strada, numar, sector), nume(nume)
{
    Statie::id_sufix++;
    this->cod_sufix = to_string(Statie::id_sufix);
    this->v_transport.clear();
    for(const auto &tra: v_trans)
    {
        this->v_transport.push_back(tra);
    }
}

Statie::Statie(const Statie &other_obj): a(other_obj.a), nume(other_obj.nume)
{
    Statie::id_sufix++;
    this->cod_sufix = to_string(Statie::id_sufix);
    this->v_transport.clear();
    for(const auto &tra: other_obj.v_transport)
    {
        this->v_transport.push_back(tra);
    }
}

Statie& Statie::operator=(const Statie &other_obj)
{
    Statie::id_sufix++;
    this->cod_sufix = to_string(Statie::id_sufix);
    if(this != &other_obj)
    {
        this->a = other_obj.a;
        this->nume = other_obj.nume;
        this->v_transport.clear();
        for(const auto &tra: other_obj.v_transport)
        {
            this->v_transport.push_back(tra);
        }
    }
    return *this;
}

class StatieUrbana: virtual public Statie{
private:
    bool punct_achizitionare;
public:
    StatieUrbana();
    StatieUrbana(const string &strada, int numar, int sector, const string &nume, const vector<string> &v_trans, bool punct_achizitionare);
    ~StatieUrbana() override = default;

    void setCodSU();

    void read(istream &in) override;
    void readSpecifications(istream &in);
    void display(ostream &out) const override;
    void displaySpecifications(ostream &out) const;
};

void StatieUrbana::setCodSU()
{
    this->setCodPrefix("SU");
    this->setCod(this->getCodPrefix() + "-" + this->getCodSufix());
}

void StatieUrbana::read(istream &in)
{
    this->setCodSU();
    Statie::read(in);
    this->readSpecifications(in);
}

void StatieUrbana::readSpecifications(istream &in)
{
    cout<<"Codul statiei urbane este: "<<this->getCod();
    int choice;
    cout<<endl<<"Are punct de achizitionare? 0/1"; in>>choice;
    if(choice == 0)
    {
        this->punct_achizitionare = false;
    } else if(choice == 1)
    {
        this->punct_achizitionare = true;
    }
    cout<<endl;
}

void StatieUrbana::display(ostream &out) const
{
    Statie::display(out);
    this->displaySpecifications(out);
}

void StatieUrbana::displaySpecifications(ostream &out) const
{
    out<<"Codul statiei urbane este: "<<this->getCod();
    out<<endl<<"Punct de achizitionare?"<<this->punct_achizitionare;
    out<<endl;
}

StatieUrbana::StatieUrbana(): Statie(), punct_achizitionare(false)
{
    this->setCodSU();
}
StatieUrbana::StatieUrbana(const string &strada, int numar, int sector, const string &nume, const vector<string> &v_trans, bool punct_achizitionare):
    Statie(strada, numar, sector, nume, v_trans), punct_achizitionare(punct_achizitionare)
    {
        this->setCodSU();
    }

class StatieExtraurbana: virtual public Statie{
public:
    StatieExtraurbana();
    StatieExtraurbana(const string &strada, int numar, int sector, const string &nume, const vector<string> &v_trans);
    ~StatieExtraurbana() override = default;

    void setCodSE();

    // nu suprascrie functia de read()
    void display(ostream &out) const override;
    void displaySpecifications(ostream &out) const;
};

void StatieExtraurbana::setCodSE()
{
    this->setCodPrefix("SE");
    this->setCod(this->getCodPrefix() + "-" + this->getCodSufix());
}

void StatieExtraurbana::display(ostream &out) const
{
    Statie::display(out);
    this->displaySpecifications(out);
}

void StatieExtraurbana::displaySpecifications(ostream &out) const
{
    out<<"Codul statiei urbane este: "<<this->getCod();
    out<<endl;
}

StatieExtraurbana::StatieExtraurbana(): Statie(){ this->setCodSE();}
StatieExtraurbana::StatieExtraurbana(const string &strada, int numar, int sector, const string &nume, const vector<string> &v_trans):
    Statie(strada, numar, sector, nume, v_trans){ this->setCodSE();}


class StatieImportanta: public StatieUrbana, public StatieExtraurbana{
private:
    vector<string> v_obiective;
public:
    StatieImportanta();
    StatieImportanta(const string &strada, int numar, int sector, const string &nume, const vector<string> &v_trans, bool punct_achizitionare, const vector<string> &obct);
    ~StatieImportanta() override = default;

    void read(istream &in) override;
    void display(ostream &out) const override;
};

void StatieImportanta::read(istream &in)
{
    Statie::read(in);
    StatieUrbana::readSpecifications(in);
    StatieExtraurbana::displaySpecifications(cout);

    int n;
    cout<<"Introduceti nr de obiective: "; in>>n;
    for(int i = 0; i < n; i++)
    {
        in>>this->v_obiective[i];
    }
    cout<<endl;
}

void StatieImportanta::display(ostream &out) const
{
    Statie::display(out);
    StatieUrbana::displaySpecifications(out);
    StatieExtraurbana::displaySpecifications(out);

    out<<"Vectorul de obiective este: "<<endl;
    for(const auto &o: this->v_obiective)
    {
        out<<o<<" ";
    }
    out<<endl;
}

StatieImportanta::StatieImportanta(): Statie(), StatieUrbana(), StatieExtraurbana()
{
    this->v_obiective.clear();
}
StatieImportanta::StatieImportanta(const string &strada, int numar, int sector, const string &nume, const vector<string> &v_trans, bool punct_achizitionare, const vector<string> &obct)
    : Statie(strada, numar, sector, nume, v_trans), StatieUrbana(strada, numar, sector, nume, v_trans, punct_achizitionare), StatieExtraurbana(strada, numar, sector, nume, v_trans)
{
    this->v_obiective.clear();
    for(const auto &o: obct) {
        this->v_obiective.push_back(o);
    }
}

// subpunctele A si D numai
class PunctInformare{
private:
    vector<Statie *> v_statii; // agregare
    float pret;
public:
    PunctInformare();
    PunctInformare(float pret, const vector<Statie *> &sta);
    PunctInformare(const PunctInformare &other_obj);
    PunctInformare& operator=(const PunctInformare &other_obj);
    ~PunctInformare();

    void read(istream &in);
    void display(ostream &out) const;

    const float getPret() const;
    float computePrice();
};

const float PunctInformare::getPret() const
{
    return this->pret;
}

float PunctInformare::computePrice()
{
    cout<<"Introduceti detalii legate de doua statii pentru a putea calcula pretul unei calatorii"<<endl;
    this->pret = 2;
    string s1, s2;
    cout<<"Introduceti prima statie:"<<endl;
    getline(cin, s1);
    cout<<"Introduceti a doua statie:"<<endl;
    getline(cin, s2);
    Statie *ptr_s1 = nullptr;
    Statie *ptr_s2 = nullptr;
    for(const auto &ptr_s: this->v_statii)
    {
         if(ptr_s->getNume() == s1) // e pointer
         {
             ptr_s1 = ptr_s;
             break;
         } else if(ptr_s->getNume() == s2)
         {
             ptr_s2 = ptr_s;
             break;
         }
    }
    if(dynamic_cast<StatieUrbana *>(ptr_s1) && dynamic_cast<StatieUrbana *>(ptr_s2))
    {
        if(ptr_s1->hasDirectLine(*ptr_s2) == false) // dereferentiere
        {
            this->pret += 0.15 * this->pret;
        }
    } else if(dynamic_cast<StatieExtraurbana *>(ptr_s1) && dynamic_cast<StatieExtraurbana *>(ptr_s2))
    {
        if(ptr_s1->hasDirectLine(*ptr_s2) == true)
        {
            this->pret += 0.2 * this->pret;
        } else
        {
            this->pret += 0.25 * this->pret;
        }
    } else
    {
        if(ptr_s1->hasDirectLine(*ptr_s2) == true)
        {
            this->pret += 0.3 * this->pret;
        } else
        {
            this->pret += 0.4 * this->pret;
        }
    }
    return this->pret;
}

void PunctInformare::read(istream &in) // adaugarea unei statii
{
    int n;
    cout<<endl<<"Nr de statii ale punctului de informare: "; in>>n;
    this->v_statii.resize(n);
    for(int i = 0; i < n; i++)
    {
        int choice;
        // downcasting folosind dynamic cast
        Statie *ptr_statie = nullptr;
        cout<<endl<<"Meniu alegere:\n1.Statie urbana\n2.Statie extra urbana\n3.Statie importanta\n";
        cin>>choice;
        cout<<endl<<"Statie indice ["<<i<<"]:"<<endl;
        switch(choice)
        {
            case 1:
                ptr_statie = new StatieUrbana();
                break;
            case 2:
                ptr_statie = new StatieExtraurbana();
                break;
            case 3:
                ptr_statie = new StatieImportanta();
                break;
            default:
                cout<<"Alegere invalida"<<endl;
                break;
        }
        ptr_statie->read(in); // dereferentiere
        this->v_statii.push_back(ptr_statie);
    }
}

void PunctInformare::display(ostream &out) const
{
    out<<"Vectorul de statii incluse la punctul de informare este:"<<endl;
    for(const auto &s: this->v_statii)
    {
        out<<*s<<endl; // dereferentiere
    }
    out<<endl;
}

PunctInformare::PunctInformare(): pret(2)
{
    this->v_statii.clear();
}

PunctInformare::PunctInformare(float pret, const vector<Statie *> &sta): pret(pret)
{
    this->v_statii.clear();
    for(const auto &s: sta)
    {
        this->v_statii.push_back(s);
    }
}

PunctInformare::PunctInformare(const PunctInformare &other_obj): pret(other_obj.pret)
{
    if(!this->v_statii.empty())
    {
        for(const auto &s: this->v_statii)
        {
            delete s;
        }
    }
    this->v_statii.clear();
    for(const auto &s: other_obj.v_statii)
    {
        this->v_statii.push_back(s);
    }
}

PunctInformare::~PunctInformare()
{
    if(!this->v_statii.empty())
    {
        for(const auto &s: this->v_statii)
        {
            delete s;
        }
    }
    this->v_statii.clear();
}

PunctInformare& PunctInformare::operator=(const PunctInformare &other_obj)
{
    if(this != &other_obj)
    {
        this->pret = other_obj.pret;
        if(!this->v_statii.empty())
        {
            for(const auto &s: this->v_statii)
            {
                delete s;
            }
        }
        this->v_statii.clear();
        for(const auto &s: other_obj.v_statii)
        {
            this->v_statii.push_back(s);
        }
    }
    return *this;
}

int main()
{
    // nu e in cerinta, dar e un requirement din subiecte din ani recenti:
    // citirea a n obiecte
    vector<PunctInformare> v_pcte_inf;
    int n;
    cout<<"Introduceti nr de puncte de informare detinute: "; cin>>n;
    v_pcte_inf.resize(n);
    for(int i = 0; i < n; i++)
    {
        PunctInformare p;
        p.read(cin); // A adaugarea unei statii completand corect datele
        int choice;
        cout<<endl<<"Doriti sa calculati pretul unei calatorii intre doua statii de la punctul de informare curent? 0\1"; cin>>choice;
        if(choice == 1)
        {
            p.computePrice();
        }
    }
    // afisarea a n obiecte
    cout<<"Vectorul de punct de informare din firma este:"<<endl;
    for(const auto &p: v_pcte_inf)
    {
        p.display(cout);
        cout<<endl;
    }
    return 0;
}