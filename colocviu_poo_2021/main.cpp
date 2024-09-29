#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
// nu merge computeRating for some reason
using namespace std;

class Data{
private:
    int zi, luna, an;
public:
    Data();
    Data(int zi, int luna, int an);
    ~Data() = default;

    void read(istream &in);
    void display(ostream &out) const;
};

Data::Data(): zi(0), luna(0), an(0) {}
Data::Data(int zi, int luna, int an): zi(zi), luna(luna), an(an) {}

void Data::display(ostream &out) const
{
    out<<"Ziua calendaristica este: "<<endl;
    out<<"\tzi: "<<this->zi<<endl;
    out<<"\tluna: "<<this->luna<<endl;
    out<<"\tan: "<<this->an<<endl;
}

void Data::read(istream &in)
{
    cout<<"Introduceti informatii in legatura cu ziua calendaristica: "<<endl;
    cout<<"\tzi: "; in>>this->zi;
    cout<<endl<<"\tluna: "; in>>this->luna;
    cout<<endl<<"\tan: "; in>>this->an;
    cout<<endl;
}

class Malware{
private:
    mutable float rating;
    Data d; // compunere, instanta Malware dependenta parintele Data
    string nume;
    string metoda_infectare;
    vector<string> v_registrii;
public:
    Malware();
    Malware(float rating, int zi, int luna, int an, const string &nume, const string &metoda_verificare, const vector<string> &v_registrii);
    virtual ~Malware() = default;
    Malware(const Malware &other_obj);
    Malware& operator=(const Malware &other_obj);

    float getRating() const;
    void setRating(float r) const;

    friend ostream& operator<<(ostream &out, const Malware &obj);
    friend istream& operator>>(istream &in, Malware &obj);
    virtual void display(ostream &out) const;
    virtual void read(istream &in);

    virtual float computeRating() const = 0; // functie const pt ca nu modifica niciun parametru, si fct pur virtuala pt ca nu este cunoscuta inca implementarea
};

float Malware::getRating() const
{
    return this->rating;
}

void Malware::setRating(float rating) const
{
    this->rating = rating;
}

Malware::Malware(): rating(0), nume("Not defined"), metoda_infectare("Unknown"){ // se apeleaza constructoprul de initializare fara parametri pentru d, nu trebuie explicitat
    if(!this->v_registrii.empty())
    {
        v_registrii.clear();
    }
}

Malware::Malware(float rating, int zi, int luna, int an, const string &nume, const string &metoda_verificare, const vector<string> &v_reg): rating(rating), d(zi, luna, an), nume(nume), metoda_infectare(metoda_infectare)
{
    this->v_registrii.clear();
    if(!v_reg.empty())
    {
        for(const auto &reg: v_reg)
        {
            v_registrii.push_back(reg);
        }
    }
}

Malware::Malware(const Malware &other_obj): rating(other_obj.rating), d(other_obj.d), nume(other_obj.nume), metoda_infectare(other_obj.metoda_infectare)
{
    if(!this->v_registrii.empty())
    {
        v_registrii.clear();
    }
    for(const auto &reg: other_obj.v_registrii)
    {
        this->v_registrii.push_back(reg);
    }
}

Malware& Malware::operator=(const Malware &other_obj)
{
    if(this!=&other_obj) // evita self-asignarea
    {
        this->rating = other_obj.rating;
        this->d = other_obj.d;
        this->nume = other_obj.nume;
        this->metoda_infectare = other_obj.metoda_infectare;
        if(!this->v_registrii.empty())
        {
            v_registrii.clear();
        }
        for(const auto &reg: other_obj.v_registrii)
        {
            this->v_registrii.push_back(reg);
        }
    }
    return *this;
}

ostream& operator<<(ostream &out, const Malware &obj)
{
    obj.display(out);
    return out;
}

istream& operator>>(istream &in, Malware &obj)
{
    obj.read(in);
    return in;
}

void Malware::read(istream &in)
{
    cout<<"Introduceti urmatoarele informatii pentru un malware:"<<endl;
    cout<<"\tRating: "; in>>this->rating;
    cout<<endl<<"\tData: "; this->d.read(in);
    cout<<endl<<"\tNume: "; in>>this->nume;
    cout<<endl<<"\tMetoda infectare:"; in>>this->metoda_infectare; in.clear(); in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cout<<endl;
    int n;
    cout<<"Introduceti nr de elemente de adaugat in vectorul v_registrii: "; in>>n;
    v_registrii.resize(n);
    for(int i = 0; i < n; i++)
    {
        in>>v_registrii[i];
    }
}

void Malware::display(ostream &out) const
{
    out<<"Malware-ul are urmatoarele informatii:"<<endl;
    out<<"\tRating: "<<rating;
    out<<endl<<"\tData: "; d.display(out);
    out<<endl<<"\tNume: "<<nume;
    out<<endl<<"\tMetoda infectare:"<<metoda_infectare;
    out<<endl<<"Vectorul de registrii este:"<<endl;
    for(const auto &reg: this->v_registrii)
    {
        out<<reg<<" ";
    }
    out<<endl;
}

class Rootkit: virtual public Malware{
private:
    vector<string> v_importuri;
    vector<string> v_semnificative;
public:
    Rootkit();
    Rootkit(float rating, int zi, int luna, int an, const string &nume, const string &metoda_verificare, const vector<string> &v_reg, const vector<string> &v_imp, const vector<string> &v_sem);
    ~Rootkit() override = default;
    Rootkit(const Rootkit &other_obj);
    Rootkit& operator=(const Rootkit &other_obj);

    void read(istream &in) override;
    void readSpecifications(istream &in);
    void display(ostream &out) const override;
    void displaySpecifications(ostream &out) const;

    float computeRating() const override;
};

Rootkit::Rootkit(): Malware()
{
    if(!this->v_importuri.empty())
    {
        v_importuri.clear();
    }
    if(!this->v_semnificative.empty())
    {
        v_semnificative.clear();
    }
}

Rootkit::Rootkit(float rating, int zi, int luna, int an, const string &nume, const string &metoda_verificare, const vector<string> &v_reg, const vector<string> &v_imp, const vector<string> &v_sem): Malware(rating, zi, luna, an, nume, metoda_verificare, v_reg){
    this->v_importuri.clear();
    if(!v_imp.empty())
    {
        for(const auto &reg: v_reg)
        {
            v_importuri.push_back(reg);
        }
    }
    this->v_semnificative.clear();
    if(!v_sem.empty())
    {
        for(const auto &reg: v_reg)
        {
            v_semnificative.push_back(reg);
        }
    }
}

Rootkit::Rootkit(const Rootkit &other_obj): Malware(other_obj)
{
    if(!this->v_importuri.empty())
    {
        v_importuri.clear();
    }
    for(const auto &imp: other_obj.v_importuri)
    {
        this->v_importuri.push_back(imp);
    }
    if(!this->v_semnificative.empty())
    {
        v_semnificative.clear();
    }
    for(const auto &sem: other_obj.v_semnificative)
    {
        this->v_semnificative.push_back(sem);
    }
}

Rootkit& Rootkit::operator=(const Rootkit &other_obj)
{
    if(this!=&other_obj) // evita self-asignarea
    {
        Malware::operator=(other_obj);
        if(!this->v_importuri.empty())
        {
            v_importuri.clear();
        }
        for(const auto &imp: other_obj.v_importuri)
        {
            this->v_importuri.push_back(imp);
        }
        if(!this->v_semnificative.empty())
        {
            v_semnificative.clear();
        }
        for(const auto &sem: other_obj.v_semnificative)
        {
            this->v_semnificative.push_back(sem);
        }
    }
    return *this;
}

void Rootkit::read(istream &in)
{
    Malware::read(in);
    this->readSpecifications(in);
}

void Rootkit::readSpecifications(istream &in)
{
    int n1;
    cout<<"Introduceti nr de elemente de adaugat in vectorul v_importuri: "; in>>n1;
    v_importuri.resize(n1);
    for(int i = 0; i< n1; i++)
    {
        in>>v_importuri[i];
    }
    int n2;
    cout<<"Introduceti nr de elemente de adaugat in vectorul v_semnificative: "; in>>n2;
    v_semnificative.resize(n2);
    for(int i = 0; i < n2; i++)
    {
        in>>v_semnificative[i];
    }
}

void Rootkit::display(ostream &out) const
{
    Malware::display(out);
    this->displaySpecifications(out);
}

void Rootkit::displaySpecifications(ostream &out) const // pt rezolvarea problemei in diamant
{
    out<<endl<<"Vectorul de importuri este:"<<endl;
    for(const auto &imp: this->v_importuri)
    {
        out<<imp<<" ";
    }
    out<<endl;
    out<<endl<<"Vectorul de semnificative este:"<<endl;
    for(const auto &sem: this->v_semnificative)
    {
        out<<sem<<" ";
    }
    out<<endl;
}

float Rootkit::computeRating() const
{
    float temp_impact = this->getRating();
    for(const auto &sem: this->v_semnificative)
    {
        if(sem == "System Service Descriptor Table" || sem == "SSDT" || sem == "NtCreateFile")
        {
            temp_impact += 100;
        }
    }
    for(const auto &imp: this->v_importuri)
    {
        if(imp == "ntoskrnl.exe")
        {
            temp_impact *= 2;
        }
    }
    // nu doresc sa declar setRating() si toate clasele derivate, deci voi face upcasting
    this->Malware::setRating(temp_impact);
    return temp_impact;
}

class Keylogger: virtual public Malware{
private:
    vector<string> v_functii;
    vector<string> v_taste;
public:
    Keylogger();
    Keylogger(float rating, int zi, int luna, int an, const string &nume, const string &metoda_verificare, const vector<string> &v_reg, const vector<string> &v_fun, const vector<string> &v_tas);
    ~Keylogger() override = default;
    Keylogger(const Keylogger &other_obj);
    Keylogger& operator=(const Keylogger &other_obj);

    void read(istream &in) override;
    void readSpecifications(istream &in);
    void display(ostream &out) const override;
    void displaySpecifications(ostream &out) const;

    float computeRating() const override;
};

Keylogger::Keylogger(): Malware()
{
    if(!this->v_functii.empty())
    {
        v_functii.clear();
    }
    if(!this->v_taste.empty())
    {
        v_taste.clear();
    }
}

Keylogger::Keylogger(float rating, int zi, int luna, int an, const string &nume, const string &metoda_verificare, const vector<string> &v_reg, const vector<string> &v_fun, const vector<string> &v_tas): Malware(rating, zi, luna, an, nume, metoda_verificare, v_reg){
    this->v_functii.clear();
    if(!v_fun.empty())
    {
        for(const auto &fun: v_fun)
        {
            v_functii.push_back(fun);
        }
    }
    this->v_taste.clear();
    if(!v_tas.empty())
    {
        for(const auto &tas: v_tas)
        {
            v_taste.push_back(tas);
        }
    }
}

Keylogger::Keylogger(const Keylogger &other_obj): Malware(other_obj)
{
    this->v_functii.clear();
    for(const auto &fun: other_obj.v_functii)
    {
        v_functii.push_back(fun);
    }
    this->v_taste.clear();
    for(const auto &tas: other_obj.v_taste)
    {
        v_taste.push_back(tas);
    }
}

Keylogger& Keylogger::operator=(const Keylogger &other_obj)
{
    if(this!=&other_obj) // evita self-asignarea
    {
        Malware::operator=(other_obj);
        this->v_functii.clear();
        for(const auto &fun: other_obj.v_functii)
        {
            v_functii.push_back(fun);
        }
        this->v_taste.clear();
        for(const auto &tas: other_obj.v_taste)
        {
            v_taste.push_back(tas);
        }
    }
    return *this;
}

void Keylogger::read(istream &in)
{
    Malware::read(in);
    this->readSpecifications(in);
}

void Keylogger::readSpecifications(istream &in)
{
    int n1;
    cout<<"Introduceti nr de elemente de adaugat in vectorul v_functii: "; in>>n1;
    v_functii.resize(n1);
    for(int i = 0; i< n1; i++)
    {
        in>>v_functii[i];
    }
    int n2;
    cout<<"Introduceti nr de elemente de adaugat in vectorul v_taste: "; in>>n2;
    v_taste.resize(n2);
    for(int i = 0; i < n2; i++)
    {
        in>>v_taste[i];
    }
}

void Keylogger::display(ostream &out) const
{
    Malware::display(out);
    this->displaySpecifications(out);
}

void Keylogger::displaySpecifications(ostream &out) const
{
    out<<endl<<"Vectorul de functii este:"<<endl;
    for(const auto &fun: this->v_functii)
    {
        out<<fun<<" ";
    }
    out<<endl;
    out<<endl<<"Vectorul de semnificative este:"<<endl;
    for(const auto &tas: this->v_taste)
    {
        out<<tas<<" ";
    }
    out<<endl;
}

float Keylogger::computeRating() const
{
    float temp_impact = this->getRating();
    for(const auto &tas: this->v_taste)
    {
        if(tas == "[Up]" || tas == "[Down]" || tas == "[NumLock]" || tas == "[Right]" || tas == "[UP]" || tas == "[Left]", tas == "[PageDown]")
        {
            temp_impact += 10;
        }
    }
    for(const auto &fun: this->v_functii)
    {
        if(fun == "CreateFileW" || fun == "OpenProcess" || fun == "ReadFile" || fun == "WriteFile" || fun == "RegisterHotKey" || fun == "SetWindowsHookEx")
        {
            temp_impact += 30;
        }
    }
    this->Malware::setRating(temp_impact);
    return temp_impact;
}


class KernelKeylogger: public Rootkit, public Keylogger{// mostenire in diamant
private:
    bool fisiere;
    bool registrii;
public:
    KernelKeylogger();
    KernelKeylogger(float rating, int zi, int luna, int an, const string &nume, const string &metoda_verificare, const vector<string> &v_reg, const vector<string> &v_imp, const vector<string> &v_sem, const vector<string> &v_fun, const vector<string> &v_tas, bool fisiere, bool registrii);
    ~KernelKeylogger() override = default;

    // aici se reutilizeaza cod de tip readSpecifications(in) si displaySpecifications(out)
    void read(istream &in) override;
    void display(ostream &out) const override;

    float computeRating() const override;
};

KernelKeylogger::KernelKeylogger(): Malware(), Rootkit(), Keylogger(), fisiere(false), registrii(false) {}

KernelKeylogger::KernelKeylogger(float rating, int zi, int luna, int an, const string &nume, const string &metoda_verificare, const vector<string> &v_reg, const vector<string> &v_imp, const vector<string> &v_sem, const vector<string> &v_fun, const vector<string> &v_tas, bool fisiere, bool registrii):
        Malware(rating, zi, luna, an, nume, metoda_verificare, v_reg),
        Rootkit(rating, zi, luna, an, nume, metoda_verificare, v_reg, v_imp, v_sem),
        Keylogger(rating, zi, luna, an, nume, metoda_verificare, v_reg, v_fun, v_tas),
        fisiere(fisiere),
        registrii(registrii){}

void KernelKeylogger::read(istream &in)
{
    Malware::read(in);
    Rootkit::readSpecifications(in); // doar cu read ar fi insemnat suprascrierea informatiilor de la partea de Malware
    Keylogger::readSpecifications(in); // similar si aici

    int choice1;
    cout<<"Are fisiere? 0/1"<<endl; cin>>choice1;
    if(choice1 == 0)
    {
        this->fisiere = false;
    } else if(choice1 == 1)
    {
        this->fisiere = true;
    }
    int choice2;
    cout<<"Are registrii? 0/1"<<endl; cin>>choice2;
    if(choice2 == 0)
    {
        this->registrii = false;
    } else if(choice2 == 1)
    {
        this->registrii = true;
    }
}

void KernelKeylogger::display(ostream &out) const
{
    Malware::display(out);
    Rootkit::displaySpecifications(out);
    Keylogger::displaySpecifications(out);
    out<<"\tMemoreaza fisiere: "<<fisiere<<endl;
    out<<"\tMemoreaza registrii: "<<registrii<<endl;
}

float KernelKeylogger::computeRating() const
{
    float temp_impact = Rootkit::computeRating() + Keylogger::computeRating() + this->getRating();
    if (fisiere == true)
    {
        temp_impact += 20;
    }
    if (registrii == true)
    {
        temp_impact += 30;
    }
    this->Malware::setRating(temp_impact);
    return temp_impact;
}

class Ransomware: public Malware{
private:
    int rating_criptare; // de la 1 la 10
    float rating_obfuscare; // procent
public:
    Ransomware();
    Ransomware(float rating, int zi, int luna, int an, const string &nume, const string &metoda_verificare, const vector<string> &v_reg, int rating_criptare, float rating_obfuscare);
    ~Ransomware() override = default;

    void read(istream &in) override;
    void display(ostream &out) const override;

    float computeRating() const override;
};

Ransomware::Ransomware(): Malware(), rating_criptare(0), rating_obfuscare(0) {}

Ransomware::Ransomware(float rating, int zi, int luna, int an, const string &nume, const string &metoda_verificare, const vector<string> &v_reg, int rating_criptare, float rating_obfuscare): Malware(rating, zi, luna, an, nume, metoda_verificare, v_reg), rating_criptare(rating_criptare), rating_obfuscare(rating_obfuscare){}

void Ransomware::read(istream &in)
{
    Malware::read(in);
    int cript;
    cout<<"Rating de criptare: "; in>>cript;
    try{
        if(cript < 1 || cript > 10)
        {
            throw runtime_error("Nu e in [1, 10]");
        }
        this->rating_criptare = cript;
    } catch(const runtime_error &)
    {
        cout<<"Rating-ul de criptare introdus nu este o valoare intre 1 si 10"<<endl;
        Ransomware::read(in);
    }
    int obfus;
    cout<<"Rating de obfuscare: "; in>>obfus;
    try{
        if(cript < 0 || cript > 100)
        {
            throw runtime_error("Nu e un procent");
        }
        this->rating_obfuscare = obfus;
    } catch(const runtime_error &)
    {
        cout<<"Rating-ul de obfuscare introdus nu este o valoare intre 0 si 100"<<endl;
        Ransomware::read(in);
    }
}

void Ransomware::display(ostream &out) const
{
    Malware::display(out);
    out<<"Criptare: "<<this->rating_criptare;
    out<<endl<<"Obfuscare: "<<this->rating_obfuscare;
    out<<endl;
}

float Ransomware::computeRating() const
{
    return rating_obfuscare + (float)rating_criptare; // c-like type conversion
}

class Computer{
private:
    static int id;
    int cod;
    vector<Malware *> v_malware; // agregare
    // folosire pointeri - trebuie definiti toti the big 5
    float suma_rating;
public:
    Computer();
    Computer(int cod, float suma_rating, const vector<Malware *> &v_mal);
    Computer(const Computer &other_obj);
    Computer& operator=(const Computer &other_obj);
    ~Computer();

    float getSumaRating() const;

    void read(istream &in);
    void display(ostream &out) const;
};

int Computer::id = 0;

float Computer::getSumaRating() const
{
    return this->suma_rating;
}

Computer::Computer(): suma_rating(0){
    v_malware.clear();
    Computer::id++;
    this->cod = Computer::id;
}

Computer::Computer(int cod, float suma_rating, const vector<Malware *> &v_mal)
{
    Computer::id++;
    this->cod = Computer::id;
    this->v_malware.clear();
    for(const auto &mal: v_mal)
    {
        this->v_malware.push_back(mal);
    }
}

Computer::Computer(const Computer &other_obj)
{
    Computer::id++;
    this->cod = Computer::id;
    if(!this->v_malware.empty())
    {
        for(const auto &mal: v_malware)
        {
            delete mal; // eliberare memorie dinamica
        }
    }
    this->v_malware.clear();
    for(const auto &mal: other_obj.v_malware)
    {
        this->v_malware.push_back(mal);
    }
    this->suma_rating = other_obj.suma_rating;
}

Computer& Computer::operator=(const Computer &other_obj)
{
    if(this != &other_obj)
    {
        Computer::id++;
        this->cod = Computer::id;
        if(!this->v_malware.empty())
        {
            for(const auto &mal: v_malware)
            {
                delete mal; // eliberare memorie dinamica
            }
        }
        this->v_malware.clear();
        for(const auto &mal: other_obj.v_malware)
        {
            this->v_malware.push_back(mal);
        }
        this->suma_rating = other_obj.suma_rating;
    }
    return *this;
}

Computer::~Computer()
{
    if(!this->v_malware.empty())
    {
        for(const auto &mal: v_malware)
        {
            delete mal;
        }
    }
    this->v_malware.clear();
}

void Computer::read(istream &in)
{
    this->suma_rating = 0;
    this->cod = Computer::id;
    cout<<"Codul de identificare al computerului este: "<<this->cod;
    int n;
    cout<<endl<<"Nr de virusi computer infectat: "; in>>n;
    this->v_malware.resize(n);
    for(int i = 0; i < n; i++)
    {
        int choice_virus;
        // downcasting folosind dynamic cast
        Malware *ptr_malware = nullptr;
        cout<<endl<<"Meniu alegere:\n1.Rootkit\n2.Keylogger\n3.KernelKeylogger\n4.Ransomware\n";
        cin>>choice_virus;
        cout<<endl<<"Virus indice ["<<i<<"]:"<<endl;
        switch(choice_virus)
        {
            case 1:
                ptr_malware = new Rootkit();
                break;
            case 2:
                ptr_malware = new Keylogger();
                break;
            case 3:
                ptr_malware = new KernelKeylogger();
                break;
            case 4:
                ptr_malware = new Ransomware();
                break;
            default:
                cout<<"Alegere invalida"<<endl;
                break;
        }
        ptr_malware->read(in); // dereferentiere
        this->v_malware.push_back(ptr_malware);
        this->suma_rating += ptr_malware->computeRating();
    }
}

void Computer::display(ostream &out) const
{
    out<<"Codul de identificare al computerului este: "<<this->cod<<endl;
    out<<"Suma de rating a impactului asupra computerului este: "<<this->suma_rating<<endl;
    for(const auto &ptr_malware: this->v_malware)
    {
        if(dynamic_cast<Rootkit *>(ptr_malware))
        {
            Rootkit *ptr_virus = dynamic_cast<Rootkit *>(ptr_malware);
            cout<<"De tip rootkit";
            out<<*ptr_virus<<endl;
        } else if(dynamic_cast<Keylogger *>(ptr_malware))
        {
            Keylogger *ptr_virus = dynamic_cast<Keylogger *>(ptr_malware);
            cout<<"De tip keylogger";
            out<<*ptr_virus<<endl;
        } else if(dynamic_cast<KernelKeylogger *>(ptr_malware))
        {
            KernelKeylogger *ptr_virus = dynamic_cast<KernelKeylogger *>(ptr_malware);
            cout<<"De tip kernelkeylogger";
            out<<*ptr_virus<<endl;
        } else if(dynamic_cast<Ransomware *>(ptr_malware))
        {
            Ransomware *ptr_virus = dynamic_cast<Ransomware *>(ptr_malware);
            cout<<"De tip ransomware";
            out<<*ptr_virus<<endl;
        }
    }
}

int main()
{
    vector<Computer> v_comp;
    int n;
    cout<<"Introduceti nr de computere din firma"<<endl;
    cin>>n;
    v_comp.resize(n);
    for(int i = 0; i < n; i++) // citire n obiecte
    {
        Computer c;
        c.read(cin); // supraincarcare, meniu citire + introd virusi computer curent
    }
    cout<<"Vectorul de computere din firma este:"<<endl;
    for(const auto &comp: v_comp)
    {
        comp.display(cout);
        cout<<endl;
    }
    int k;
    cout<<"Introduceti nr de computere care trebuie ordonate descrescator in functie de impact:"<<endl;
    cin>>k;
    auto regula_sortare = [] (Computer const &c1, Computer const &c2) -> bool // regula lambda
    {
        return c1.getSumaRating() > c2.getSumaRating(); // desc
    };
    sort(v_comp.begin(), v_comp.end(), regula_sortare); // se sorteaza toate elem, dar se afiseaza doar primele k rezultate
    for(int i = 0; i < k; i++)
    {
        v_comp[i].display(cout);
        cout<<endl;
    }
    int nr_infectate = 0;
    for(const auto &comp: v_comp)
    {
        if(comp.getSumaRating() != 0)
        {
            nr_infectate++;
        }
    }
    cout<<"Procentul de infectare este: "<<(float)nr_infectate / n * 100 << "%";
    return 0;
}