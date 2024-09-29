#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Drum{ // neinstantiabil
private:
    string nume;
    float lungime;
    int nr_tronsoane;
public:
    Drum();
    Drum(const string &nume, float lungime, int nr_tronsoane);
    Drum(const Drum &other_obj);
    Drum& operator=(const Drum &other_obj);
    virtual ~Drum() = default;

    int getNrTronsoane() const;
    string getNume() const;
    float getLungime() const;

    virtual void read(istream &in);
    virtual void display(ostream &out) const;
    friend istream& operator>>(istream &in, Drum &obj)
    {
        obj.read(in);
        return in;
    }
    friend ostream& operator<<(ostream &out, const Drum &obj)
    {
        obj.display(out);
        return out;
    }
};

void Drum::read(istream &in)
{
    cout<<"Introduceti datele despre drum:";
    cout<<endl<<"\tnume: "; in>>this->nume;
    cout<<endl<<"\tlungime: "; in>>this->lungime;
    int k;
    cout<<endl<<"\tnr tronsoane: "; in>>k;
    try{
        if(k < 1)
        {
            throw runtime_error("Nr tronsonului este un numar natural, care nu poate sa fie mai mic decat 1");
        }
        this->nr_tronsoane = k;
    } catch(const runtime_error &)
    {
        cout<<"Vei fi promptat sa introduci din nou datele"<<endl;
        Drum::read(in);
    }
    cout<<endl;
}

void Drum::display(ostream &out) const
{
    out<<"Datele despre drum:";
    out<<endl<<"\tnume: "<<this->nume;
    out<<endl<<"\tlungime: "<<this->lungime;
    out<<endl<<"\tnr tronsoane: "<<this->nr_tronsoane;
    out<<endl;
}

int Drum::getNrTronsoane() const
{
    return this->nr_tronsoane;
}

string Drum::getNume() const
{
    return this->nume;
}

float Drum::getLungime() const
{
    return this->lungime;
}

Drum::Drum(): nr_tronsoane(0), nume("Unspecified"), lungime(-1) {}
Drum::Drum(const string &nume, float lungime, int nr_tronsoane): nume(nume), lungime(lungime), nr_tronsoane(nr_tronsoane) {}
Drum::Drum(const Drum &other_obj)
{
    this->nume = other_obj.nume;
    this->lungime = other_obj.lungime;
    this->nr_tronsoane = other_obj.nr_tronsoane;
}
Drum& Drum::operator=(const Drum &other_obj)
{
    if(this != &other_obj)
    {
        this->nume = other_obj.nume;
        this->lungime = other_obj.lungime;
        this->nr_tronsoane = other_obj.nr_tronsoane;
    }
    return *this;
}

class DrumNational: virtual public Drum{
private:
    int nr_judete;
public:
    DrumNational();
    DrumNational(const string &nume, float lungime, int nr_tronsoane, int nr_judete);
    ~DrumNational() override = default;

    int getNrJudete() const;
    void setNrJudete(const int &n);

    void read(istream &in) override;
    void display(ostream &out) const override;
};

int DrumNational::getNrJudete() const
{
    return this->nr_judete;
}
void DrumNational::setNrJudete(const int &n)
{
    this->nr_judete = n;
}

void DrumNational::read(istream &in)
{
    Drum::read(in);
    cout<<"\tnr judete: "; in>>this->nr_judete;
    cout<<endl;
}
void DrumNational::display(ostream &out) const
{
    Drum::display(out);
    out<<"\tnr judete: "<<this->nr_judete;
    out<<endl;
}

DrumNational::DrumNational(): Drum(), nr_judete(0){}
DrumNational::DrumNational(const string &nume, float lungime, int nr_tronsoane, int nr_judete): Drum(nume, lungime, nr_tronsoane), nr_judete(nr_judete){}

class DrumEuropean: virtual public Drum{
private:
    int nr_tari;
public:
    DrumEuropean();
    DrumEuropean(const string &nume, float lungime, int nr_tronsoane, int nr_tari);
    ~DrumEuropean() override = default;

    int getNrTari() const;
    void setNrTari(const int &n);

    void read(istream &in) override;
    void display(ostream &out) const override;
};

int DrumEuropean::getNrTari() const
{
    return this->nr_tari;
}
void DrumEuropean::setNrTari(const int &n)
{
    this->nr_tari = n;
}

DrumEuropean::DrumEuropean(): Drum(), nr_tari(0){}
DrumEuropean::DrumEuropean(const string &nume, float lungime, int nr_tronsoane, int nr_tari): Drum(nume, lungime, nr_tronsoane), nr_tari(nr_tari){}

void DrumEuropean::read(istream &in)
{
    Drum::read(in);
    cout<<"\tnr tari: "; in>>this->nr_tari;
    cout<<endl;
}
void DrumEuropean::display(ostream &out) const
{
    Drum::display(out);
    out<<"\tnr tari: "<<this->nr_tari;
    out<<endl;
}

class Autostrada: public DrumNational, public DrumEuropean{
private:
    int nr_benzi;
public:
    Autostrada();
    Autostrada(const string &nume, float lungime, int nr_tronsoane, int nr_judete, int nr_tari, int nr_benzi);
    ~Autostrada() override = default;

    int getNrBenzi() const;

    void read(istream &in) override;
    void display(ostream &out) const override;
};

int Autostrada::getNrBenzi() const
{
    return this->nr_benzi;
}

void Autostrada::read(istream &in)
{
    Drum::read(in);
    int j, t;
    cout<<"\tnr judete: "; in>>j; setNrJudete(j);
    cout<<endl<<"\tnr tari: "; in>>t; setNrTari(t);
    int k;
    cout<<endl<<"\tnr benzi: "; in>>k;
    try{
        if(k < 2)
        {
            throw runtime_error("Nr tronsonului este un numar natural, minim egal cu 2");
        }
        this->nr_benzi = k;
    } catch(const runtime_error &)
    {
        cout<<"Vei fi promptat sa introduci din nou datele"<<endl;
        Autostrada::read(in);
    }
    cout<<endl;
}
void Autostrada::display(ostream &out) const
{
    Drum::display(out);
    out<<"\tnr judete: "<<this->getNrJudete();
    out<<endl<<"\tnr tari: "<<this->getNrTari();
    out<<endl<<"\tnr benzi: "<<this->nr_benzi;
    out<<endl;
}

Autostrada::Autostrada(): Drum(), DrumNational(), DrumEuropean(), nr_benzi(2){}
Autostrada::Autostrada(const string &nume, float lungime, int nr_tronsoane, int nr_judete, int nr_tari, int nr_benzi):
        Drum(nume, lungime, nr_tronsoane),
        DrumNational(nume, lungime, nr_tronsoane, nr_judete),
        DrumEuropean(nume, lungime, nr_tronsoane, nr_tari),
        nr_benzi(nr_benzi){}

class Firma{
private:
    string nume_firma;
    string cif;
public:
    Firma();
    Firma(const string &nume_firma, const string &cif);
    ~Firma() = default;

    void read(istream &in);
    void display(ostream &out) const;
};

Firma::Firma(): nume_firma("Unspecified"), cif("") {}
Firma::Firma(const string &nume_firma, const string &cif): nume_firma(nume_firma), cif(cif){}
void Firma::read(istream &in)
{
    cout<<"Introduceti detalii despre firma:";
    cout<<endl<<"\tnume firma: "; in>>this->nume_firma;
    cout<<endl<<"\tcif: "; in>>this->cif;
    cout<<endl;
}
void Firma::display(ostream &out) const
{
    out<<"Detalii despre firma:";
    out<<endl<<"\tnume firma: "<<this->nume_firma;
    out<<endl<<"\tcif: "<<this->cif;
    out<<endl;
}

// cade undeva
class Contract{
private:
    static int id;
    mutable int cod;
    Firma f; // compunere
    Drum *ptr_d;
    int tronson;
    float cost;
public:
    // trebuie definite the big 5 din cauza ca o data membra este un pointer
    Contract();
    Contract(const string &nume_firma, const string &cif);
    Contract(const Contract &other_obj);
    Contract& operator=(const Contract &other_obj);
    ~Contract();

    float getCost() const;
    Drum* getPtrDrum() const;
    void setPtrDrum(Drum *p);

    void read(istream &in);
    void display(ostream &out) const;

    void computeCost();
};

int Contract::id = 0;

float Contract::getCost() const
{
    return this->cost;
}
Drum* Contract::getPtrDrum() const
{
    return this->ptr_d;
}
void Contract::setPtrDrum(Drum *p)
{
    this->ptr_d = p;
}

void Contract::read(istream &in)
{
    cout<<"Introduceti date despre contract:";
    f.read(in);
    cout<<endl<<"\tdrum:";
    Drum *temp = nullptr;
    int choice;
    cout<<endl<<"1.Drum national\n2.Drum european\n3.Autostrada\n"; in>>choice;
    switch(choice)
    {
        case 1:
            temp = new DrumNational();
            break;
        case 2:
            temp = new DrumEuropean();
            break;
        case 3:
            temp = new Autostrada();
            break;
        default:
            cout<<"Alegere invalida"<<endl;
            break;
    }
    setPtrDrum(temp);
    ptr_d->read(in);
    cout<<endl<<"Introduceti nr de ordine al tronsonului asociat drumului introdus: "; in>>this->tronson;
    cout<<endl;
}

void Contract::display(ostream &out) const
{
    out<<"Date despre contract:";
    f.display(out);
    if(dynamic_cast<DrumNational *>(ptr_d))
    {
        auto temp = dynamic_cast<DrumNational *>(ptr_d);
        temp->display(out);
    } else if(dynamic_cast<DrumEuropean *>(ptr_d))
    {
        auto temp = dynamic_cast<DrumEuropean *>(ptr_d);
        temp->display(out);
    } else if(dynamic_cast<Autostrada *>(ptr_d))
    {
        auto temp = dynamic_cast<Autostrada *>(ptr_d);
        temp->display(out);
    }
    out<<endl<<"\ttronsonul asociat drumului: "<<this->tronson;
    out<<endl;
}

void Contract::computeCost()
{
    if(typeid(*ptr_d) == typeid(DrumNational) || typeid(*ptr_d) == typeid(DrumEuropean) || typeid(*ptr_d) == typeid(Drum))
    {
        this->cost = 3000 * ptr_d->getLungime();
    } else if(typeid(*ptr_d) == typeid(Autostrada))
    {
        this->cost = 2500 * (dynamic_cast<Autostrada *>(ptr_d))->getNrBenzi() * ptr_d->getLungime();
    }
    // in plus
    if(typeid(*ptr_d) == typeid(DrumEuropean))
    {
        this->cost += 500 * (dynamic_cast<DrumEuropean *>(ptr_d))->getNrTari();
    }
}

Contract::Contract(): f()
{
    Contract::id++;
    this->cod = Contract::id;
}
Contract::Contract(const string &nume_firma, const string &cif): f(nume_firma, cif)
{
    Contract::id++;
    this->cod = Contract::id;
}
Contract::Contract(const Contract &other_obj): f(other_obj.f)
{
    Contract::id++;
    this->cod = Contract::id;
    this->f = other_obj.f;
    this->ptr_d = other_obj.ptr_d;
    this->tronson = other_obj.tronson;
}
Contract& Contract::operator=(const Contract &other_obj)
{
    if(this != &other_obj)
    {
        Contract::id++;
        this->cod = Contract::id;
        this->f = other_obj.f;
        this->ptr_d = other_obj.ptr_d;
        this->tronson = other_obj.tronson;
    }
    return *this;
}
Contract::~Contract()
{
    delete ptr_d;
}


int main()
{
    vector<Contract> v_contracte;
    vector<Drum *> v_drumuri;
    int n, m;
    cout<<"Nr de elem v_contracte: "; cin>>n;
    v_contracte.resize(n);
    for(int i = 0; i < n; i++)
    {
        Contract c;
        c.read(cin);
        v_contracte.push_back(c);
    }
    cout<<endl<<"Vectorul de contracte este:"<<endl;
    for(const auto &c: v_contracte)
    {
        c.display(cout);
        cout<<endl;
    }
    cout<<"Nr de elem v_drumuri: "; cin>>n;
    v_drumuri.resize(n);
    int choice;
    for(int i = 0; i < n; i++)
    {
        Drum *ptr_d = nullptr;
        cout<<"Drum ["<<i<<"]:"<<endl;
        cout<<endl<<"1.Drum national\n2.Drum european\n3.Autostrada\n"; cin>>choice;
        switch(choice)
        {
            case 1:
                ptr_d = new DrumNational();
                break;
            case 2:
                ptr_d = new DrumEuropean();
                break;
            case 3:
                ptr_d = new Autostrada();
                break;
            default:
                cout<<"Alegere invalida"<<endl;
                break;
        }
        ptr_d->read(cin);
        v_drumuri.push_back(ptr_d);
    }
    cout<<endl<<"Vectorul de drumuri este:"<<endl;
    for(const auto &ptr_d: v_drumuri)
    {
        ptr_d->display(cout);
        cout<<endl;
    }
    string d1;
    cout<<endl<<"Introduceti numele drumului pentru care trebuie gasit costul la nivelul mai multor contracte: "; cin>>d1;
    float cost_total = 0;
    for(const auto &c: v_contracte)
    {
        Drum *ptr_d = c.getPtrDrum();
        Drum d = (*ptr_d);
        if(d.getNume() == d1)
        {
            cost_total += c.getCost();
        }
    }
    cout<<"Costul total din toate contractele pentru drumul "<<d1<<" este: "<<cost_total<<endl;
    // dezalocare dinamica
    for(const auto &ptr_d: v_drumuri)
    {
        delete ptr_d;
    }
    v_drumuri.clear();
    return 0;
}
